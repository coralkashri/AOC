#ifndef ADVENTOFCODE_STR_NUMBER_HPP
#define ADVENTOFCODE_STR_NUMBER_HPP

template <typename T = long long>
class str_number
{
public:
	void operator+=(char c)
	{
		is_ready = true;
		number *= 10;
		number += c - '0';
	}

	void reset()
	{
		is_ready = false;
		number = 0;
	}

	bool is_number_ready() const
	{
		return is_ready;
	}

	T get_number() const
	{
		return number;
	}

private:
	T number = 0;
	bool is_ready = false;
};

#endif //ADVENTOFCODE_STR_NUMBER_HPP
