
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


TEST(Datavalue, DefaultConstruction) {

	row::cif::Datavalue d{};
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
	EXPECT_FALSE(d.isConverted());
}



//// Tests factorial of 0.
//TEST(FactorialTest, HandlesZeroInput) {
//	EXPECT_EQ(Factorial(0), 1);
//}
//
//// Tests factorial of positive numbers.
//TEST(FactorialTest, HandlesPositiveInput) {
//	EXPECT_EQ(Factorial(1), 1);
//	EXPECT_EQ(Factorial(2), 2);
//	EXPECT_EQ(Factorial(3), 6);
//	EXPECT_EQ(Factorial(8), 40320);
//}