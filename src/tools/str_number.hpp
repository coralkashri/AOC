#ifndef ADVENTOFCODE_STR_NUMBER_HPP
#define ADVENTOFCODE_STR_NUMBER_HPP

#include <vector>

template<typename T = long long>
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

template<typename ValT = long long, template <typename...> typename ContT = std::vector, char Sep = ' '>
class str_numbers
{
public:
	str_numbers() = default;

	str_numbers(std::string_view str)
	{
		set(str);
	}

	void set(std::string_view str)
	{
		for (char c : str)
		{
			*this += c;
		}
	}

	void operator+=(char c)
	{
		if (c == Sep)
		{
			is_prev_number_ready = true;
			return;
		}

		if (!std::isdigit(c)) return;

		if (is_prev_number_ready)
		{
			is_prev_number_ready = false;
			data.push_back(0);
		}

		data.back() *= 10;
		data.back() += c - '0';
	}

	auto& get()
	{
		return data;
	}

private:
	bool is_prev_number_ready = true;
	ContT<ValT> data;
};

template<typename ValT = long long, template<typename...> typename ContT = std::vector, char Sep = ' '>
class str_numbers {
public:
    str_numbers() = default;

    str_numbers(std::string_view str) {
        set(str);
    }

    void set(std::string_view str) {
        for (char c: str) {
            *this += c;
        }
    }

    void operator+=(char c) {
        if (c == Sep) {
            is_prev_number_ready = true;
            return;
        }

        if (!std::isdigit(c)) return;

        if (is_prev_number_ready) {
            is_prev_number_ready = false;
            data.push_back(0);
        }

        data.back() *= 10;
        data.back() += c - '0';
    }

    auto &get() {
        return data;
    }

private:
    bool is_prev_number_ready = true;
    ContT<ValT> data;
};

#endif //ADVENTOFCODE_STR_NUMBER_HPP
