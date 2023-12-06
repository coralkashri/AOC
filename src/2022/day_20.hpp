#ifndef AOC_2022_20
#define AOC_2022_20

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <variant>
#include <array>
#include <numeric>
#include <set>

#include "../input_source.hpp"

template <typename T>
void print_vector(T v)
{
	std::copy_n(v.begin(), v.size(), std::ostream_iterator<int64_t>(std::cout, " "));
	std::cout << std::endl;
}

int first_part_2022_20()
{
	std::list<int64_t> numbers;
	std::vector<decltype(numbers)::iterator> nodes;
	std::transform(std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>(), std::back_inserter(numbers), [](const std::string & str)
	{
		return std::stoll(str);
	});

	nodes.reserve(numbers.size());
	for (auto it = numbers.begin(); it != numbers.end(); ++it)
	{
		nodes.push_back(it);
	}

	decltype(numbers)::iterator zero_node;

	for (auto n : nodes)
	{
		auto number = *n;
		int64_t index = std::distance(numbers.begin(), n);
		int64_t new_index = 0;

		if (number != 0)
		{
			numbers.erase(n);
		}

		if (number > 0)
		{
			new_index = (index + number) % numbers.size();
		}
		else if (number < 0)
		{
			new_index = index + number - 1;
			if (new_index < 0)
			{
				new_index += numbers.size() + 1;
			}
			else if (new_index == numbers.size())
			{
				exit(1);
			}
		}
		else
		{
			zero_node = n;
		}

		if (number != 0)
		{
			numbers.insert(std::next(numbers.begin(), new_index), number);
		}

		//print_vector(numbers);
	}

	size_t zero_index = std::distance(numbers.begin(), zero_node);
	size_t first_index = (zero_index + 1000) % numbers.size();
	size_t second_index = (zero_index + 2000) % numbers.size();
	size_t third_index = (zero_index + 3000) % numbers.size();

	auto first_node = std::next(numbers.begin(), first_index);
	auto second_node = std::next(numbers.begin(), second_index);
	auto third_node = std::next(numbers.begin(), third_index);

	std::cout << *first_node << " " << *second_node << " " << *third_node << std::endl;

	std::cout << (*first_node + *second_node + *third_node) << std::endl;
	return EXIT_SUCCESS;
}

int second_part_2022_20()
{
	// TODO

	return EXIT_SUCCESS;
}

#endif