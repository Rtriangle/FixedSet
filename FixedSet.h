#include <vector>
#include <random>
#include <vector>
#include <iostream>
#include <random>
#include <limits>
//#include "FixedSet.h"

std::random_device rd;
std::mt19937 Generator(rd());
std::uniform_int_distribution<long> ValuesA(1, 2000000010);
const long FreePlace = std::numeric_limits<long>::max();
static const long MinusIgnore = 1000000000;

/*
static std::default_random_engine Generator;
static std::uniform_int_distribution<long> ValuesA(1, 2000000010);
static std::uniform_int_distribution<long> ValuesA(0, 2000000010);
const long FreePlace = std::numeric_limits<long>::max();
*/

class NodeHash
{
private:
    long A, B;
    static const long PRIME = 2000000011;
public:
    void reInit()
    {
        A = ValuesA(Generator);
        B = ValuesA(Generator);
    }
    long F(const long key) const
	{
		return (static_cast<long long> (A) * (key + MinusIgnore) + B) % PRIME;
	};
};

struct SimpleTable
{
    std::vector<long> Values;
    NodeHash Hash;
    void AddNumbers(long key)
	{
		Values[Hash.F(key) % Values.size()] = key;
	};
    bool Contains(long key) const
	{
		if (Values.size() != 0)
			return Values[Hash.F(key) % Values.size()] == key;
		else
			return false;
	};
};

class FixedSet
{
public:
    explicit FixedSet(const std::vector<long> &numbers);
    bool Contains(long number) const;
private:
    std::vector<SimpleTable> Table;
    void generateA_B(const std::vector<long> &numbers, std::vector<long> &Count);
    NodeHash Hash;
};


bool FixedSet::Contains(long number) const
{
    if (Table.size() != 0)
        return Table[Hash.F(number) % Table.size()].Contains(number);
    else
        return false;
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

    }
	while (CollisionsCount > 4 * (numbers.size()));

}

//Construct our
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
            Table[i].Values.assign(Count[i] * Count[i], FreePlace);
            success = true;

            for (size_t k = 0; k < temp[i].size(); ++k)
            {
                long current_hash = Table[i].Hash.F(temp[i][k]) % Table[i].Values.size();
                if ((Table[i].Values[current_hash] == temp[i][k]) || (Table[i].Values[current_hash] == FreePlace))
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
