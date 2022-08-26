
#include <vector>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <numbers>
#include <iostream>
#include <unordered_map>
#include <variant>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t
#include <format>
#include <utility>

#include "pch.h"

#include "pdqciflib.hpp"

using namespace row::cif;
using namespace row::util;

//TEST(TestCaseName, TestName) {
//
//	
//	EXPECT_EQ(1, 1);
//	EXPECT_TRUE(true);
//}


namespace utilTest {

	TEST(util_stode, numericStrings) {
		//numeric strings
		std::vector<std::string> strs{ "30", "-30", "+30", "30(3)", "-30(2)", "+30(1)",
									   "12.3", "-12.3", "+12.3", "12.3(2)", "-12.3(3)", "+12.3(4)",
									   "12.3e2", "-12.3e2", "+12.3e2", "12.3e2(2)", "-12.3e2(3)", "+12.3e2(4)",
									   "12.3e-2", "-12.3e-2", "+12.3e-2", "12.3e-2(2)", "-12.3e-2(3)", "+12.3e-2(4)",
									   "12.3e+2", "-12.3e+2", "+12.3e+2", "12.3e+2(2)", "-12.3e+2(3)", "+12.3e+2(4)",
									   "12.3E2", "-12.3E2", "+12.3E2", "12.3E2(2)", "-12.3E2(3)", "+12.3E2(4)",
									   "12.3E-2", "-12.3E-2", "+12.3E-2", "12.3E-2(2)", "-12.3E-2(3)", "+12.3E-2(4)",
									   "12.3E+2", "-12.3E+2", "+12.3E+2", "12.3E+2(2)", "-12.3E+2(3)", "+12.3E+2(4)" };

		std::vector<double> dbls{ 30, -30, 30, 30, -30, 30,
								  12.3,    -12.3,    12.3,    12.3,    -12.3,    12.3,
								  12.3e2,  -12.3e2,  12.3e2,  12.3e2,  -12.3e2,  12.3e2,
								  12.3e-2, -12.3e-2, 12.3e-2, 12.3e-2, -12.3e-2, 12.3e-2,
								  12.3e+2, -12.3e+2, 12.3e+2, 12.3e+2, -12.3e+2, 12.3e+2,
								  12.3E2,  -12.3E2,  12.3E2,  12.3E2,  -12.3E2,  12.3E2,
								  12.3E-2, -12.3E-2, 12.3E-2, 12.3E-2, -12.3E-2, 12.3E-2,
								  12.3E+2, -12.3E+2, 12.3E+2, 12.3E+2, -12.3E+2, 12.3E+2 };

		std::vector<double> errs{ 0.0, 0.0, 0.0, 3.0, 2.0, 1.0,
								  0.0, 0.0, 0.0, 0.2, 0.3, 0.4,
								  0.0, 0.0, 0.0, 20.0, 30.0, 40.0,
								  0.0, 0.0, 0.0, 0.002, 0.003, 0.004,
								  0.0, 0.0, 0.0, 20.0, 30.0, 40.0,
								  0.0, 0.0, 0.0, 20.0, 30.0, 40.0,
								  0.0, 0.0, 0.0, 0.002, 0.003, 0.004,
								  0.0, 0.0, 0.0, 20.0, 30.0, 40.0 };

		for (size_t i{ 0 }; i < strs.size(); ++i) {
			auto [d, e] = stode(strs[i]);
			EXPECT_DOUBLE_EQ(dbls[i], d) << "Should be the value from \"" << strs[i] << "\" (index " << i << ")";
			EXPECT_DOUBLE_EQ(errs[i], e) << "Should be the error from \"" << strs[i] << "\" (index " << i << ")";
		}

	}

	TEST(util_stode, nonNumericStrings) {
		std::vector<std::string> nan_strs{ "a string", "e", "E", "(12)" };
		for (size_t i{ 0 }; i < nan_strs.size(); ++i) {
			auto [d, e] = stode(nan_strs[i]);
			EXPECT_TRUE(std::isnan(d));
			EXPECT_TRUE(std::isnan(d));
		}
	}

	TEST(util_stode, unknownValueStrings) {
		//missing or unknown value strings
		std::vector<std::string> missing_strs{ ".", "?" };
		for (size_t i{ 0 }; i < missing_strs.size(); ++i) {
			auto [d, e] = stode(missing_strs[i]);
			EXPECT_TRUE(std::isnan(d));
			EXPECT_DOUBLE_EQ(e, 0.0);
		}



	}

	TEST(util_getIndexOf, elementExists) {
		std::vector<int> v{ 1,2,3 };
		std::vector<int> r{ 0,1,2 };

		for (size_t i{ 0 }; i < v.size(); ++i) {
			EXPECT_EQ(getIndexOf(v, v[i]), r[i]);
		}
	}

	TEST(util_getIndexOf, elementNotExists) {
		std::vector<int> v{ 1,2,3 };
		EXPECT_EQ(getIndexOf(v, 17), -1);
	}


	TEST(util_makeInRange, ints) {
		int i{ 5 };
		makeInRange(i, 0, 10);
		EXPECT_EQ(i, 5); // i  is in range: leave it alone

		makeInRange(i, 7, 10);
		EXPECT_EQ(i, 7); // i is too low: make it equal to the lower bound

		makeInRange(i, 2, 4);
		EXPECT_EQ(i, 4); // i is too high: make it equal to the upper bound
	}


	TEST(util_contains, ints) {
		std::vector<int> v{ 1,2,3 };

		EXPECT_TRUE(contains(v, 1));
		EXPECT_FALSE(contains(v, 5));
	}


	TEST(util_toLower, strings) {
		std::vector<std::string> uppers{ "HELLO", "HeLlO", "Hello", "hellO", "hello", "123<>%" };
		std::vector<std::string> lowers{ "hello", "hello", "hello", "hello", "hello", "123<>%" };

		for (size_t i{ 0 }; i < uppers.size(); ++i) {
			EXPECT_EQ(toLower(uppers[i]), lowers[i]);
		}
	}

	TEST(util_toLower, vectorstrings) {
		std::vector<std::string> uppers{ "HELLO", "HeLlO", "Hello", "hellO", "hello", "123<>%" };
		std::vector<std::string> lowers{ "hello", "hello", "hello", "hello", "hello", "123<>%" };

		EXPECT_EQ(toLower(uppers), lowers);
	}




}


