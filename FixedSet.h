#include <vector>
#include <random>
#include <vector>
#include <iostream>
#include <random>
#include <limits>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<long> dist(1, 2000000000);
const long FreePlace = std::numeric_limits<long>::max();
const long MinusIgnore = 1000000000;

class NodeHash
{
private:
    static const long PRIME = 2000000011;
	long A, B;
public:
    void reInit()
    {
        A = dist(gen);
        B = dist(gen);
    }
    long F(const long key) const
	{
		return (static_cast<long long> (A) * (key + MinusIgnore) + B) % PRIME;
	};
};

struct HashTable
{
	NodeHash Hash;
    std::vector<long> Elements;
    void AddElement(long key)
	{
		Elements[Hash.F(key) % Elements.size()] = key;
	};
    bool Contains(long key) const
	{
		if (Elements.size() != 0)
			return (Elements[Hash.F(key) % Elements.size()] == key);
		else
			return false;
	};
};

class FixedSet
{
public:
	bool Contains(long element) const;
    explicit FixedSet(const std::vector<long> &elements);
private:
    std::vector<HashTable> Table;
	NodeHash Hash;
    void BuildAB(const std::vector<long> &elements, std::vector<long> &Count);
};

bool FixedSet::Contains(long element) const
{
    if (Table.size() != 0)
        return Table[Hash.F(element) % Table.size()].Contains(element);
    else
        return false;
}

void FixedSet::BuildAB(const std::vector<long> &Input, std::vector<long> &Count)
{
    long SquareColl = 0;
    do
    {
		Hash.reInit();
		Count.assign(Input.size(), 0);
		for(auto it = Input.begin(); it != Input.end(); ++it)
            ++Count[Hash.F(*it) % Count.size()];
        SquareColl = 0;
        for (auto sizecol = Count.begin(); sizecol != Count.end(); ++sizecol)
            SquareColl += (static_cast<long long>(*sizecol)) * (*sizecol);
    }
	while (SquareColl > 3 * (Input.size()));
}

//Construct our
FixedSet::FixedSet(const std::vector<long> &Input)
{
    std::vector<long> Count;
    BuildAB(Input, Count);
    Table.resize(Input.size());
    std::vector<std::vector<long>> temp(Input.size());
    for (auto it = Input.begin(); it != Input.end(); ++it)
        temp[Hash.F(*it) % Input.size()].push_back(*it);
    for (size_t i = 0; i < temp.size(); ++i)
    {
        bool nocollisio;
        do{
            Table[i].Hash.reInit();
            Table[i].Elements.assign(Count[i] * Count[i], FreePlace);
            nocollisio = true;
            for (size_t j = 0; j < temp[i].size(); ++j)
            {
                long hashtmp = Table[i].Hash.F(temp[i][j]) % Table[i].Elements.size();
                if (Table[i].Elements[hashtmp] == temp[i][j] || 
					Table[i].Elements[hashtmp] == FreePlace)
                    Table[i].Elements[hashtmp] = temp[i][j];
                else
                {
                    nocollisio = false;
                    break;
                }
            }
        }
		while (!nocollisio);
    }
}