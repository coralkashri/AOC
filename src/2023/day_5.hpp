#ifndef AOC_2023_5
#define AOC_2023_5

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

class transformation_details
{
public:
	transformation_details(std::string_view trasformation_str)
	{
		str_number<size_t> number;
		unsigned char current_number_index = 0;

		for (char c : trasformation_str)
		{
			if (c == ' ')
			{
				if (number.is_number_ready())
				{
					numbers[current_number_index++] = number.get_number();
					number.reset();
				}
				continue;
			}
			number += c;
		}
		numbers[current_number_index++] = number.get_number();
	}

	size_t destination() const
	{
		return numbers[0];
	}

	size_t source() const
	{
		return numbers[1];
	}

	size_t range() const
	{
		return numbers[2];
	}

private:
	std::array<size_t, 3> numbers;
};

class seeds
{
public:
	seeds(std::string_view str)
	{
		str = str.substr(str.find(':') + 1);

		str_number<size_t> number;
		for (char c : str)
		{
			if (c == ' ')
			{
				if (number.is_number_ready())
				{
					current_seeds.push_back(std::pair{ number.get_number(), true });
					number.reset();
				}
				continue;
			}
			number += c;
		}
		current_seeds.push_back(std::pair{ number.get_number(), true });
	}

	/**
	 * Returns true if all seeds transformed, otherwise returns false
	 */
	bool apply_transformation(std::string_view trasformation_str)
	{
		if (trasformation_str.empty()) return false;

		if (trasformation_str.find(':') != std::string::npos)
		{
			reset_all_seeds_ready_status();
			return false;
		}

		transformation_details td(trasformation_str);
		bool is_all_seeds_ready = true;

		for (auto&[seed, is_transformed] : current_seeds)
		{
			if (is_transformed) continue;

			if (seed >= td.source() && seed <= td.source() + td.range() - 1)
			{
				size_t debug = seed;
				seed = seed - td.source() + td.destination();
				is_transformed = true;
			}
			else
			{
				is_all_seeds_ready = false;
			}
		}

		return is_all_seeds_ready;
	}

	size_t get_lowest_seed()
	{
		size_t lowest_seed = -1;

		for (auto[seed, status] : current_seeds)
		{
			lowest_seed = std::min(lowest_seed, seed);
		}

		return lowest_seed;
	}

private:
	std::vector<std::pair<size_t, bool>> current_seeds;

	void reset_all_seeds_ready_status()
	{
		for (auto& seed : current_seeds)
		{
			seed.second = false;
		}
	}
};

int first_part_2023_5()
{
	std::string seeds_str;
	std::getline(std::cin, seeds_str);

	seeds all_seeds(seeds_str);

	std::string transformation_str;
	bool is_all_seeds_ready = false;
	while (std::getline(std::cin, transformation_str))
	{
		if (!is_all_seeds_ready || transformation_str.find(':') != std::string::npos)
		{
			is_all_seeds_ready = all_seeds.apply_transformation(transformation_str);
		}
	}

	std::cout << all_seeds.get_lowest_seed() << std::endl;
	return EXIT_SUCCESS;
}

class range
{
private:
	struct sub_range
	{
		size_t start;
		size_t size;
	};

public:
	range(size_t start, size_t size)
	{
		sub_ranges.push_back({ start, size });
	}

	void apply_transformation(const transformation_details& td)
	{
		std::vector<sub_range> new_sub_ranges;
		for (auto& r : sub_ranges)
		{
			std::cout << "Start: " << r.start << " Size: " << r.size << "\n";
			std::cout << "TStart: " << td.source() << " Size: " << td.size() << "\n";

			if (r.start + r.size < td.source() || r.start > td.source() + td.size())
			{
				std::cout << "Outside of range\n\n";
				continue;
			}

			if (r.start >= td.source() && r.start + r.size <= td.source() + td.size())
			{
				r.start = r.start - td.source() + td.destination();
				std::cout << "Inside of range\n";
				std::cout << "Start: " << r.start << " Size: " << r.size << "\n\n";
				continue;
			}

			if (r.start >= td.source())
			{
				size_t inside_range_size = td.size() - (r.start - td.source());
				size_t outside_range_size = r.size - inside_range_size;
				size_t outside_range_start = r.start + inside_range_size;
				r.size = inside_range_size;
				r.start = r.start - td.source() + td.destination();
				new_sub_ranges.push_back({ outside_range_start, outside_range_size });

				std::cout << "Partaily inside of range (1)\n";
				std::cout << "Start1: " << r.start << " Size1: " << r.size << "\n";
				std::cout << "Start2: " << outside_range_start << " Size2: " << outside_range_size << "\n\n";

				continue;
			}

			if (r.start < td.source() && r.start + r.size <= td.source() + td.size())
			{
				size_t outside_range_size = r.size - (td.source() - r.start);
				size_t outside_range_start = r.start;
				size_t inside_range_start = r.start + outside_range_size;
				size_t inside_range_size = r.size - outside_range_size;
				r.size = inside_range_size;
				r.start = inside_range_start - td.source() + td.destination();
				new_sub_ranges.push_back({ outside_range_start, outside_range_size });

				std::cout << "Partaily inside of range (2)\n";
				std::cout << "Start1: " << r.start << " Size1: " << r.size << "\n";
				std::cout << "Start2: " << outside_range_start << " Size2: " << outside_range_size << "\n\n";
				continue;
			}

			if (r.start < td.source() && r.start + r.size > td.source() + td.size())
			{
				size_t left_size = td.source() - r.start;
				size_t left_start = r.start;

				size_t right_size = r.start + r.size - (td.source() + td.size());
				size_t right_start = r.start + r.size - right_size;

				size_t inside_start = td.destination();
				size_t inside_size = td.size();

				r.start = inside_start;
				r.size = inside_size;

				new_sub_ranges.push_back({ left_start, left_size });
				new_sub_ranges.push_back({ right_start, right_size });

				std::cout << "Partaily inside of range (3)\n";
				std::cout << "Start1: " << r.start << " Size1: " << r.size << "\n";
				std::cout << "Start2: " << left_start << " Size2: " << left_size << "\n";
				std::cout << "Start2: " << right_start << " Size2: " << right_size << "\n\n";
				continue;
			}
		}

		sub_ranges.insert(sub_ranges.end(), new_sub_ranges.begin(), new_sub_ranges.end());
	}

	size_t get_lowest()
	{
		size_t lowest = -1;
		for (auto& r : sub_ranges)
		{
			lowest = std::min(lowest, r.start);
		}
		return lowest;
	}

private:
	std::vector<sub_range> sub_ranges;
};

class ranged_seeds
{
public:
	ranged_seeds(std::string_view str)
	{
		str = str.substr(str.find(':') + 1);

		std::array<size_t, 2> current_range{ 0, 0 };
		size_t current_number = 0;
		str_number<size_t> number;
		for (char c : str)
		{
			if (c == ' ')
			{
				if (number.is_number_ready())
				{
					current_range[current_number++] = number.get_number();
					number.reset();

					if (current_number % 2 == 0)
					{
						current_seeds.emplace_back(current_range[0], current_range[1]);
					}
				}
				continue;
			}
			number += c;
		}
		current_seeds.emplace_back(current_range[0], number.get_number());
	}

	void apply_transformation(std::string_view trasformation_str)
	{
		if (trasformation_str.empty()) return;

		if (trasformation_str.find(':') != std::string::npos)
		{
			//reset_all_seeds_ready_status();
			return;
		}

		transformation_details td(trasformation_str);
		bool is_all_seeds_ready = true;

		for (auto& seed : current_seeds)
		{
			seed.apply_transformation(td);
		}
	}

	size_t get_lowest_seed()
	{
		size_t lowest_seed = -1;

		for (auto& seed : current_seeds)
		{
			lowest_seed = std::min(lowest_seed, seed.get_lowest());
		}

		return lowest_seed;
	}

private:
	std::vector<range> current_seeds;
};

int second_part_2023_5()
{
	std::string seeds_str;
	std::getline(std::cin, seeds_str);

	ranged_seeds all_seeds(seeds_str);

	std::string transformation_str;
	while (std::getline(std::cin, transformation_str))
	{
		all_seeds.apply_transformation(transformation_str);
	}

	std::cout << all_seeds.get_lowest_seed() << std::endl;
	return EXIT_SUCCESS;
}

#endif