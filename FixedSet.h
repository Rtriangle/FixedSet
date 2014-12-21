#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <cstdlib>
#pragma once

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(1, 1000000000);
static const long OFFSET = 1000000000;
const long FreePlace = std::numeric_limits<long>::max();

class NodeHash
{
private:
	static const long P = 1000000011;
    long A, B;
public:
	long F(const long key) const
	{
		return  (static_cast<long long>(A) * (key + OFFSET) + B) % P;
	}
    void BuildAB()
    {
        A = dist(gen);
        B = dist(gen);
    }
};

struct HashTable
{
	NodeHash Hash;
    std::vector<long> Values; 
    void AddElement(long key)
	{
		Values[Hash.F(key) % Values.size()] = key;
	};
    bool Contains(long key) const;
};

class FixedSet
{
public:
    explicit FixedSet(const std::vector<long> &Input);
    bool Contains(long number) ;
private:
    std::vector<HashTable> Table;
    void BuildAB(const std::vector<long> &Input, std::vector<long> &Count);
    NodeHash Hash;
};

bool FixedSet::Contains(long number) 
{
    if (Table.size() != 0)
        return Table[Hash.F(number) % Table.size()].Contains(number);
    else
        return false;
}

void FixedSet::BuildAB(const std::vector<long> &Input, std::vector<long> &Count)
{
    long SquareColl = 0;
    do
    {
        Hash.BuildAB();
        Count.assign(Input.size(), 0);
        for(auto it = Input.begin(); it != Input.end(); ++it)
        {
            ++Count[Hash.F(*it) % Count.size()];
        }
        SquareColl = 0;
        for (auto it = Count.begin(); it != Count.end(); ++it)
        {
            SquareColl += ((long) (*it)) * (*it);
        }
    }
	while (SquareColl > 4 * (Input.size()));

}

//Constr
FixedSet::FixedSet(const std::vector<long> &Input)
{
    std::vector<long> Count;
    BuildAB(Input, Count);
    Table.resize(Input.size());

    std::vector<std::vector<long>> temp(Input.size());

    for (auto it = Input.begin(); it != Input.end(); ++it)
    {
        temp[Hash.F(*it) % Input.size()].push_back(*it);
    }

    for (size_t i = 0; i < temp.size(); ++i)
    {
        bool nocollisio = true;
		while(!nocollisio)
            Table[i].Hash.BuildAB();
            Table[i].Values.assign(Count[i] * Count[i], FreePlace);
            nocollisio = true;
            for (size_t k = 0; k < temp[i].size() && nocollisio; ++k)
            {
                long current_hash = Table[i].Hash.F(temp[i][k]) % Table[i].Values.size();
                if ((Table[i].Values[current_hash] == temp[i][k]) || (Table[i].Values[current_hash] == FreePlace))
                    Table[i].Values[current_hash] = temp[i][k];
                else
					nocollisio = false;
			}
	}
}

bool HashTable::Contains(long key) const
{
    if (Values.size() != 0)
        return Values[Hash.F(key) % Values.size()] == key;
    else
        return false;
}
