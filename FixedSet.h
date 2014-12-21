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
static const long MinusIgnore = 1000000000;
const long FreePlace = std::numeric_limits<long>::max();

class NodeHash
{
private:
	static const long P = 1000000011;
    long A, B;
public:
	void BuildAB()
    {
        A = dist(gen);
        B = dist(gen);
    }
	long F(const long key) const
	{
		return  (static_cast<long long>(A) * (key + MinusIgnore) + B) % P;
	}
};

struct HashTable
{
	NodeHash Hash;
    std::vector<long> Elements; 
    void AddElement(long key)
	{
		Elements[Hash.F(key) % Elements.size()] = key;
	};
    bool Contains(long key)
	{
    if (Elements.size() != 0)
        return Elements[Hash.F(key) % Elements.size()] == key;
    else
        return false;
	};
};

/*
bool HashTable::Contains(long key) const
{
    if (Elements.size() != 0)
        return Elements[Hash.F(key) % Elements.size()] == key;
    else
        return false;
}
*/

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

//Construct our
FixedSet::FixedSet(const std::vector<long> &Input)
{
    std::vector<long> Count;
    BuildAB(Input, Count);
    Table.resize(Input.size());
    std::vector<std::vector<long>> tempHash(Input.size());
    for (auto it = Input.begin(); it != Input.end(); ++it)
        tempHash[Hash.F(*it) % Input.size()].push_back(*it);
	bool nocollisio;
    for (size_t i = 0; i < tempHash.size(); ++i)
    {  
		do{
            Table[i].Hash.BuildAB();
            Table[i].Elements.assign(Count[i] * Count[i], FreePlace);
            nocollisio = true;
            for (size_t k = 0; k < tempHash[i].size() && nocollisio; ++k)
            {
                long hashtmp = Table[i].Hash.F(tempHash[i][k]) % Table[i].Elements.size();
                if (Table[i].Elements[hashtmp] == FreePlace || 
					Table[i].Elements[hashtmp] == tempHash[i][k] )
                    Table[i].Elements[hashtmp] = tempHash[i][k];
                else
					nocollisio = false;
			}
		}
		while(!nocollisio);
	}
}