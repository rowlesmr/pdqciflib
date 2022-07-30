#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numbers>

export module util;


export namespace row::util {




	std::string toLower(std::string str) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}
	void toLower_i(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	}


	std::vector<std::string> toLower(std::vector<std::string> strs) {
		for (std::string& str : strs) {
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		}
		return strs;
	}
	void toLower_i(std::vector<std::string>& strs) {
		for (std::string& str : strs) {
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		}
	}


	template<typename C, typename F>
	bool contains(const C& c, const F& f) {
		return std::find(c.cbegin(), c.cend(), f) != c.cend();
	}

	template <typename T>
	void makeInRange(T& val, const T& minVal, const T& maxVal) {
		if (val > maxVal) {
			val = maxVal;
		}
		if (val < minVal) {
			val = minVal;
		}
	}


	//https://stackoverflow.com/a/57399634/36061
	template <typename T> void move_element(std::vector<T>& v, size_t oldIndex, size_t newIndex)
	{
		makeInRange<size_t>(newIndex, 0, v.size() - 1);
		if (oldIndex > newIndex)
			std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
		else
			std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
	}


	template<typename T>
	int getIndexOf(const std::vector<T>& v, const T& f) {
		auto it = std::find(v.cbegin(), v.cend(), f);
		if (it == v.cend()) {
			return -1;
		}
		return it - v.cbegin();
	}



	constexpr double NaN{ std::numeric_limits<double>::quiet_NaN() };
	constexpr double deg{ std::numbers::pi / 180.0 };
	constexpr double rad{ 1.0 / deg };

	std::pair<double, double> stode(const char* s, const size_t len)
	{
		double v{ 0.0 }; //value
		double e{ 0.0 }; //the error in the value

		int sgn{ 1 }; // what is sign of the double?
		int p{ 0 }; // what is the effective power for the value and error terms?
		char c{ *s };
		bool hasDigits{ false };
		
		//this is the address of the start of the const char*. I need it as a long, so I can
		// do the same when I get to the end. If the difference is the length of the string, 
		// then I know I've converted the entire string, and it is valid.
		// I need to cast to void* to get around interpreting the char* as a string. I then reinterpret 
		// the address as a long, so I can do maths with it.
		unsigned long long beginning{ reinterpret_cast<unsigned long long>(static_cast<const void*>(s)) };
		size_t extraChar{ 0 };

		//get the sign of the double
		if (c == '-') {
			sgn = -1;
			++s;
		}
		else if (c == '+') {
			++s;
		}
		//get the digits before the decimal point
		while ((c = *s++) != '\0' && std::isdigit(c)) {
			v = v * 10.0 + (c - '0');
			hasDigits = true;
		}
		//get the digits after the decimal point
		if (c == '.' || c == '?') {
			if (len == 1) {
				return std::pair<double, double>({ NaN, 0 });
			}
			while ((c = *s++) != '\0' && std::isdigit(c)) {
				v = v * 10.0 + (c - '0');
				p--;
				hasDigits = true;
			}
		}
		//get the digits that belong to the exponent
		if ((c == 'e' || c == 'E') && hasDigits) {
			int sign = 1;
			int m = 0;
			c = *s++;
			if (c == '+') {
				c = *s++;
			}
			else if (c == '-') {
				c = *s++;
				sign = -1;
			}
			while (isdigit(c)) {
				m = m * 10 + (c - '0');
				c = *s++;
			}
			p += sign * m;
		}
		// get the digits that belong to the error
		if (c == '(' && hasDigits) {
			while ((c = *s++) != '\0' && std::isdigit(c)) { //implicitly breaks out of loop on the trailing ')'
				e = e * 10.0 + (c - '0');
			}
			extraChar = 1;
		}
		//scale the value and error
		while (p > 0) {
			v *= 10.0;
			e *= 10.0;
			p--;
		}
		while (p < 0) {
			v *= 0.1;
			e *= 0.1;
			p++;
		}
		//apply the correct sign to the value
		v *= sgn;

		unsigned long long end{ reinterpret_cast<unsigned long long>(static_cast<const void*>(s)) };
		size_t used_len{ static_cast<size_t>((end - beginning) / sizeof(char)) };
		size_t numChars{ extraChar + used_len - 1 };

		if (numChars != len) {
			return std::pair<double, double>({ NaN, NaN });
		}

		return std::pair<double, double>({ v, e });;
	}
	std::pair<double, double> stode(const std::string& s) {
		return stode(s.c_str(), s.size());
	}



}