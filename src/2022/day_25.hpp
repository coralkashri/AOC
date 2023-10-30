#ifndef AOC_2022_25
#define AOC_2022_25

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

uint64_t parse_snafu_to_decimal(const std::string& snafu_number)
{
	int64_t res = 0;
	std::for_each(snafu_number.rbegin(), snafu_number.rend(), [&res, current_base = 1ll](char c) mutable
	{
		switch (c)
		{
		case '0':
		case '1':
		case '2':
			res += current_base * (c - '0');
			break;
		case '=':
			res -= current_base;
		case '-':
			res -= current_base;
			break;
		}
		current_base *= 5;
	});
	return res;
}

std::string parse_decimal_to_snafu(int64_t decimal_number)
{
	std::string res;
	int64_t temp = decimal_number;
	int reminder;
	uint32_t current_index = 0;
	while (temp)
	{
		reminder = temp % 5;
		temp /= 5;

		if (res.size() == current_index)
		{
			if (reminder <= 2)
			{
				res = (char)(reminder + '0') + res;
			}
			else
			{
				res = (char)(reminder == 3 ? '=' : '-') + res;
				res = '1' + res;
			}
		}
		else
		{
			switch (res.front())
			{
			case '0':
			case '1':
			case '2':
				reminder += (res.front() - '0');
				if (reminder > 5) exit(500);

				if (reminder <= 2)
				{
					res.front() = reminder + '0';
				}
				else if (reminder < 5)
				{
					res.front() = reminder == 3 ? '=' : '-';
					res = '1' + res;
				}
				else
				{
					res.front() = '0';
					res = '1' + res;
				}
				break;

			default:
				exit(501);
			}
		}

		++current_index;
	}

	return res;
}

int first_part_2022_25() {
	int64_t sum = std::accumulate(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), 0ll, [&](int64_t res, auto line)
	{
		return res + parse_snafu_to_decimal(line);
	});

	std::cout << parse_decimal_to_snafu(sum) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2022_25() {
	// TODO

    return EXIT_SUCCESS;
}

#endif