#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <cstdlib>
#pragma once

const long EMPTY_ELEMENT = std::numeric_limits<long>::max();

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(1, 1000000000);
static const long OFFSET = 1000000000;

struct HashUniversalType
{
    long long A, B;
    static const long P = 2000000011;
    void reInit()
    {
        A = dist(gen);
        B = dist(gen);
    }
    long F(const long key) const;
};

long HashUniversalType::F(long Key) const
{
    return (((long long) A) * (Key + OFFSET) + B) % P;
}

struct SimpleTable
{
    std::vector<long> Values;
    HashUniversalType Hash;
    void AddNumbers(long key);
    bool Contains(long key) const;
};

class FixedSet
{

private:
    std::vector<SimpleTable> Table;
    void generateA_B(const std::vector<long> &numbers, std::vector<long> &Count);
    HashUniversalType Hash;
public:
    explicit FixedSet(const std::vector<long> &numbers);
    bool Contains(long number) const;
};

void SimpleTable::AddNumbers(long key)
{
    Values[Hash.F(key) % Values.size()] = key;
}

void FixedSet::generateA_B(const std::vector<long> &numbers, std::vector<long> &Count)
{
    long CollisionsCount = 0;

    do
    {
        Hash.reInit();
        Count.assign(numbers.size(), 0);

        for(auto number : numbers)
        {
            ++Count[Hash.F(number) % Count.size()];
        }

        CollisionsCount = 0;

        for (auto it = Count.begin(); it != Count.end(); ++it)
        {
            CollisionsCount += ((long long) (*it)) * (*it);
        }

    }while (CollisionsCount > 4 * (numbers.size()));

}

FixedSet::FixedSet(const std::vector<long> &numbers)
{
    std::vector<long> Count;
    generateA_B(numbers, Count);
    Table.resize(numbers.size());

    std::vector<std::vector<long>> temp(numbers.size());

    for (auto it = numbers.begin(); it != numbers.end(); ++it)
    {
        temp[Hash.F(*it) % numbers.size()].push_back(*it);
    }

    for (size_t i = 0; i < temp.size(); ++i)
    {
        bool success;

        do{

            Table[i].Hash.reInit();
            Table[i].Values.assign(Count[i] * Count[i], EMPTY_ELEMENT);
            success = true;

            for (size_t k = 0; k < temp[i].size(); ++k)
            {
                long current_hash = Table[i].Hash.F(temp[i][k]) % Table[i].Values.size();
                if ((Table[i].Values[current_hash] == temp[i][k]) || (Table[i].Values[current_hash] == EMPTY_ELEMENT))
                {
                    Table[i].Values[current_hash] = temp[i][k];
                }
                else
                {
                    success = false;
                    continue;
                }
            }
        }while (!success);
    }

}

bool SimpleTable::Contains(long key) const
{
    if (Values.size() != 0)
        return Values[Hash.F(key) % Values.size()] == key;
    else
        return false;
}

bool FixedSet::Contains(long number) const
{
    if (Table.size() != 0)
        return Table[Hash.F(number) % Table.size()].Contains(number);
    else
        return false;
}