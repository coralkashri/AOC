#ifndef AOC_2022_13
#define AOC_2022_13

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <variant>
#include <array>
#include <set>

#include "../input_source.hpp"

namespace aoc_2022_13_tools
{
	template<class... Ts>
	struct overloaded : Ts...
	{
		using Ts::operator()...;
	};
	// explicit deduction guide (not needed as of C++20)
	template<class... Ts>
	overloaded(Ts...)->overloaded<Ts...>;

	class str_number
	{
	public:
		void operator+=(char c)
		{
			number += c;
		}

		void reset()
		{
			number = "";
		}

		bool is_number_ready()
		{
			return !number.empty();
		}

		int get_number()
		{
			return std::stol(number);
		}

	private:
		std::string number;

		int next_number_multiplier(int number) const
		{
			int multiplier = 1;
			while (number)
			{
				number /= 10;
				multiplier *= 10;
			}
			return multiplier;
		}
	};

	class section
	{
	public:
		enum class comparison_result
		{
			LOWER,
			EQUAL,
			BIGGER
		};

	public:
		section(section* parent_section) : parent_section(parent_section)
		{
		}

		section(int number)
		{
			numbers.emplace_back(number);
		}

		section* get_parent_section()
		{
			return parent_section;
		}

		void operator+=(char c)
		{
			switch (c)
			{
			case ',':
			case ']':
				if (current_numer.is_number_ready())
				{
					numbers.emplace_back(current_numer.get_number());
					current_numer.reset();
				}
				break;
			default:
				current_numer += c;
			}
		}

		void operator+=(section* s)
		{
			numbers.emplace_back(s);
		}

		comparison_result compare(const section& other) const
		{
			size_t i = 0;
			for (; i < numbers.size() && i < other.numbers.size(); ++i)
			{
				comparison_result res = std::visit(overloaded
					{
						[this, &other, i](int num1)
						{
							return std::visit(overloaded
							{
								[this, &other, num1](int num2)
								{
									if (num1 < num2) return comparison_result::LOWER;
									if (num1 == num2) return comparison_result::EQUAL;
									return comparison_result::BIGGER;
								},
								[this, &other, num1](section * s2)
								{
									return section(num1).compare(*s2);
								}
							}, other.numbers[i]);
						},
						[this, &other, i](section * s1)
						{
							return std::visit(overloaded
							{
								[this, &other, s1](section * s2)
								{
									return s1->compare(*s2);
								},
								[this, &other, s1](int num2)
								{
									return s1->compare(section(num2));
								}
							}, other.numbers[i]);
						}
					}, numbers[i]);

				if (res != comparison_result::EQUAL) return res;
			}
			return i < other.numbers.size() ? comparison_result::LOWER : numbers.size() == other.numbers.size() ? comparison_result::EQUAL : comparison_result::BIGGER;
		}

		void print(std::ostream& cout)
		{
			for (auto& num : numbers)
			{
				std::visit(overloaded
					{
						[&cout](int n)
						{
							cout << n;
						},
						[&cout](section * s)
						{
							cout << "[";
							s->print(cout);
							cout << "]";
						}
					}, num);

				if (&num != &numbers.back()) cout << ",";
			}
		}

	private:
		str_number current_numer;
		std::vector<std::variant<int, section*>> numbers;
		section* parent_section;
	};

	class packet
	{
	public:
		packet(const std::string& data) : original_packet_string(data)
		{
			size_t index = 0;
			for (char c : data)
			{
				switch (c)
				{
				case '[':
					if (sections.empty())
					{
						sections.emplace_back(nullptr);
						current_section = &sections.front();
						index = 0;
					}
					else
					{
						auto& new_section = sections.emplace_back(current_section);
						*current_section += &new_section;
						current_section = &sections.back();
						index = sections.size() - 1;
					}
					break;
				case ']':
					*current_section += c;
					if (current_section != &sections.front())
					{
						current_section = current_section->get_parent_section();
						--index;
					}
					break;
				default:
					*current_section += c;
					break;
				}
			}
		}

		bool operator<(const packet& other) const
		{
			return sections.front().compare(other.sections.front()) == section::comparison_result::LOWER;
		}

		const std::string& get_original_packet_string() const
		{
			return original_packet_string;
		}

		void print(std::ostream& cout)
		{
			cout << "[";
			sections.front().print(cout);
			cout << "]" << std::endl;
		}

	private:
		std::list<section> sections;
		section* current_section = nullptr;
		std::string original_packet_string;
	};

	class pair_packets
	{
	public:
		pair_packets(const std::string& packet_str1, const std::string& packet_str2)
			: packets(
				{
					packet_str1, packet_str2
				})
		{}

		bool compare()
		{
			return packets[0] < packets[1];
		}

		void print(std::ostream& cout)
		{
			for (auto& packet : packets)
				packet.print(cout);
			cout << std::endl;
		}

	private:
		std::array<packet, 2> packets;
	};
}

int first_part_2022_13() {
	std::vector<aoc_2022_13_tools::pair_packets> all_packets;
	all_packets.reserve(10000);
	std::string current_packet, prev_packet;
	size_t current_packets_index = 0;
	size_t sum = 0;
	while (std::getline(INPUT_SOURCE, current_packet, '\n'))
	{
		if (current_packet.empty()) continue;
		if (++current_packets_index % 2 == 0)
		{
			all_packets.emplace_back(prev_packet, current_packet);
			sum += all_packets.back().compare() ? current_packets_index / 2 : 0;
		}
		prev_packet = current_packet;
	}

	std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2022_13() {
	using namespace std::string_literals;

	std::set<aoc_2022_13_tools::packet> all_packets;
	std::string current_packet, prev_packet;
	while (std::getline(INPUT_SOURCE, current_packet, '\n'))
	{
		if (current_packet.empty()) continue;
		all_packets.insert(current_packet);
	}

	all_packets.insert("[[2]]"s);
	all_packets.insert("[[6]]"s);

	size_t mul = 1;
	size_t index = 0;

	for (const auto& packet : all_packets)
	{
		++index;
		if (packet.get_original_packet_string() == "[[2]]" || packet.get_original_packet_string() == "[[6]]")
		{
			mul *= index;
		}
	}

	std::cout << mul << std::endl;

    return EXIT_SUCCESS;
}

#endif