#include <iostream>
#include <vector>
#include <algorithm>

enum {SIZE = 5};

int main()
{
	std::vector<int> numbers;
	int input(0);
	for(int i(0); i < SIZE; ++i)
	{
		std::cin >> input;
		numbers.push_back(input);
	}

	sort(numbers.begin(), numbers.end());

	std::cout << "Elements after sorting:" << std::endl;

	for (auto i : numbers)
	   std::cout << i << ' ';
	std::cout << std::endl;

	int sum(0);


    while (numbers.size() != 1) {
        if (numbers.front() <= 0) {
            if (numbers.at(numbers.size() - 1) >= 0) 
            {

                int min = (numbers.front() * numbers.at(numbers.size() - 1));
                sum += min;

                numbers.pop_back();
                numbers.erase(numbers.begin());
                numbers.push_back(min);

                sort(numbers.begin(), numbers.end());
            }   else {
                int min = (numbers.at(numbers.size() - 2) * numbers.at(numbers.size() - 1));
                sum += min;

                numbers.pop_back();
                numbers.pop_back();
                numbers.push_back(min);

                sort(numbers.begin(), numbers.end());
            }
        }   else { // all values are positive
            int min = (numbers.at(0) * numbers.at(1));
            sum += min;

            numbers.erase(numbers.begin());
            numbers.erase(numbers.begin());
            numbers.push_back(min);

            sort(numbers.begin(), numbers.end());
        }
    }
    
    std::cout << sum << std::endl;

	return 0;
}