#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

export module util;


export namespace row::util {



	class Success {
	private:
		int ec{ 0 };

	public:
		constexpr Success() {}

		constexpr Success(int ec) : ec(ec) {}

		void assign(int ec) noexcept {
			this->ec = ec;
		}

		void clear() noexcept {
			ec = 0;
		}

		int value() const noexcept {
			return ec;
		}

		explicit operator bool() const noexcept {
			return (ec <= 0);
		}
	};


	bool operator==(const Success& lhs, const Success& rhs) noexcept {
		return lhs.value() == rhs.value();
	}


	std::strong_ordering operator<=>(const Success& lhs, const Success& rhs) noexcept {
		return lhs.value() <=> rhs.value();
	}


	std::string toLower(std::string str) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}


	std::vector<std::string> toLower(std::vector<std::string> strs) {
		for (std::string& str : strs) {
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		}
		return strs;
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
		makeInRange(newIndex, 0, v.size() - 1);
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

}