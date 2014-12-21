#include <iostream>
#include <cmath>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <random>
#include "FixedSet.h"

int main()
{
    size_t n;
    std::cin >> n;
	std::vector <long> InputNumbers;
    for(size_t i = 0; i < n; ++i)
    {
        long long Tmp;
        std::cin >> Tmp;
		InputNumbers.push_back(Tmp);
    }
	FixedSet OurSet(InputNumbers);
    size_t q;
    std::cin >> q;
    for (size_t j = 0; j < q; ++j)
    {
        long long Tmp;
        std::cin >> Tmp;
		if(OurSet.Contains(Tmp))
			std::cout << "Yes\n";
		else
			std::cout << "No\n";
    }
    return 0;
}