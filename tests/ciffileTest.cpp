
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

import pdqciflib;

using namespace row::cif;
using namespace row::util;

//TEST(TestCaseName, TestName) {
//
//	
//	EXPECT_EQ(1, 1);
//	EXPECT_TRUE(true);
//}


namespace ciffileTest {

	TEST(ciffile_Datavalue, constructors) {
		Datavalue d1{};
		EXPECT_EQ(d1.size(), 0);
		EXPECT_FALSE(d1.isConverted());

		std::string in2{ "123.4e2(5)" };
		Datavalue d2{in2};
		EXPECT_EQ(d2.size(), 1);
		EXPECT_FALSE(d2.isConverted());

		std::vector<std::string> in3{ "123.4e2(5)", "456.7e2(4)" };
		Datavalue d3{ in3 };
		EXPECT_EQ(d3.size(), 2);
		EXPECT_FALSE(d3.isConverted());

		//Datavalue d4{ { "123.4", "456.7" } };
		//EXPECT_EQ(d4.size(), 2);
		//EXPECT_FALSE(d4.isConverted());
	}


	TEST(ciffile_Datavalue, vectorAccess) {
		Datavalue d1{ { "123.4(5)", "456.7(4)", "456.7" } };
		std::vector<std::string> str1_can{ "123.4(5)", "456.7(4)", "456.7" };
		std::vector<double> dbl1_can{ 123.4, 456.7, 456.7 };
		std::vector<double> err1_can{ 0.5, 0.4, 0.0 };

		std::vector<std::string> str1{ d1.getStrings() };
		std::vector<double> dbl1{ d1.getDoubles() };
		std::vector<double> err1{ d1.getErrors() };

		for (size_t i{ 0 }; i < d1.size(); ++i) {
			EXPECT_EQ(str1[i], str1_can[i]);
			EXPECT_DOUBLE_EQ(dbl1[i], dbl1_can[i]);
			EXPECT_DOUBLE_EQ(err1[i], err1_can[i]);
		}
		
		EXPECT_EQ(str1.size(), 3);
		EXPECT_EQ(dbl1.size(), 3);
		EXPECT_EQ(err1.size(), 3);




		Datavalue d2{ { ".", "value", "456.7(4)fake", "a string" }};
		std::vector<std::string> str2_can{ ".", "value", "456.7(4)fake", "a string" };
		
		std::vector<std::string> str2{ d2.getStrings() };
		std::vector<double> dbl2{ d2.getDoubles() };
		std::vector<double> err2{ d2.getErrors() };

		for (size_t i{ 0 }; i < d1.size(); ++i) {
			EXPECT_EQ(str2[i], str2_can[i]);
			EXPECT_TRUE(std::isnan(dbl2[i]));
			if (i == 0) {
				EXPECT_DOUBLE_EQ(err2[i], 0.0);
			}
			else {
				EXPECT_TRUE(std::isnan(err2[i]));
			}
		}

		EXPECT_EQ(str2.size(), 4);
		EXPECT_EQ(dbl2.size(), 4);
		EXPECT_EQ(err2.size(), 4);
	}

	TEST(ciffile_Datavalue, elementAccess) {
		Datavalue d1{ { "123.4(5)", "456.7(4)", "456.7" } };
		std::vector<std::string> str1_can{ "123.4(5)", "456.7(4)", "456.7" };
		std::vector<double> dbl1_can{ 123.4, 456.7, 456.7 };
		std::vector<double> err1_can{ 0.5, 0.4, 0.0 };


		std::string s11{ d1.at(0) };
		std::string& s12{ d1.at(0) };
		const std::string s13{ d1.at(0) };
		const std::string& s14{ d1.at(0) };
		EXPECT_EQ(s11, str1_can[0]);
		EXPECT_EQ(s12, str1_can[0]);
		EXPECT_EQ(s13, str1_can[0]);
		EXPECT_EQ(s14, str1_can[0]);
		EXPECT_THROW(d1.at(10), std::out_of_range);


		std::string s21{ d1.str_at(0) };
		std::string& s22{ d1.str_at(0) };
		const std::string s23{ d1.str_at(0) };
		const std::string& s24{ d1.str_at(0) };
		EXPECT_EQ(s21, str1_can[0]);
		EXPECT_EQ(s22, str1_can[0]);
		EXPECT_EQ(s23, str1_can[0]);
		EXPECT_EQ(s24, str1_can[0]);
		EXPECT_THROW(d1.str_at(10), std::out_of_range);


		double dbl11{ d1.dbl_at(0) };
		const double dbl12{ d1.dbl_at(0) };
		const double& dbl13{ d1.dbl_at(0) };
		EXPECT_EQ(dbl11, dbl1_can[0]);
		EXPECT_EQ(dbl12, dbl1_can[0]);
		EXPECT_EQ(dbl13, dbl1_can[0]);
		EXPECT_THROW(d1.dbl_at(10), std::out_of_range);


		double err11{ d1.err_at(0) };
		const double err12{ d1.err_at(0) };
		const double& err13{ d1.err_at(0) };
		EXPECT_EQ(err11, err1_can[0]);
		EXPECT_EQ(err12, err1_can[0]);
		EXPECT_EQ(err13, err1_can[0]);
		EXPECT_THROW(d1.err_at(10), std::out_of_range);
	}




	//data pointer
	TEST(ciffile_Datavalue, datapointers) {
		Datavalue d1{ { "123.4(5)", "456.7(4)", "456.7" } };
		std::vector<std::string> str1_can{ "123.4(5)", "456.7(4)", "456.7" };
		std::vector<double> dbl1_can{ 123.4, 456.7, 456.7 };
		std::vector<double> err1_can{ 0.5, 0.4, 0.0 };
		size_t last{ 2 };

		EXPECT_EQ(d1.data()[0], str1_can[0]);
		EXPECT_EQ(d1.data()[last], str1_can[last]);

		EXPECT_EQ(d1.str_data()[0], str1_can[0]);
		EXPECT_EQ(d1.str_data()[last], str1_can[last]);

		EXPECT_DOUBLE_EQ(d1.dbl_data()[0], dbl1_can[0]);
		EXPECT_DOUBLE_EQ(d1.dbl_data()[last], dbl1_can[last]);

		EXPECT_DOUBLE_EQ(d1.err_data()[0], err1_can[0]);
		EXPECT_DOUBLE_EQ(d1.err_data()[last], err1_can[last]);
	}





	TEST(ciffile_Datavalue, capacity) {
		Datavalue d1{ { "123.4(5)", "456.7(4)", "456.7" } };
		Datavalue d2{};

		//empty
		EXPECT_FALSE(d1.empty());
		EXPECT_FALSE(d1.isEmpty());

		EXPECT_TRUE(d2.empty());
		EXPECT_TRUE(d2.isEmpty());

		//size
		EXPECT_EQ(d1.size(), 3);
		EXPECT_EQ(d2.size(), 0);


		//reserve
		d2.reserve(1000);
		EXPECT_EQ(d2.capacity(), 1000);
		EXPECT_EQ(d2.getStrings().capacity(), 1000);
		EXPECT_EQ(d2.getDoubles().capacity(), 1000);
		EXPECT_EQ(d2.getErrors().capacity(), 1000);


	}

	TEST(ciffile_Datavalue, modifiers_clear) {
		Datavalue d1{ { "123.4(5)", "456.7(4)", "456.7" } };

		//clear
		EXPECT_FALSE(d1.empty());
		EXPECT_FALSE(d1.isConverted());
		d1.convert();
		EXPECT_TRUE(d1.isConverted());
		EXPECT_EQ(d1.getStrings().size(), 3);
		EXPECT_EQ(d1.getDoubles().size(), 3);
		EXPECT_EQ(d1.getErrors().size(), 3);
		d1.clear();
		EXPECT_FALSE(d1.isConverted());
		EXPECT_EQ(d1.getStrings().size(), 0);
		EXPECT_EQ(d1.getDoubles().size(), 0);
		EXPECT_EQ(d1.getErrors().size(), 0);

	}


	TEST(ciffile_Datavalue, modifiers_back) {

		// *_back
		Datavalue d2{ { "123.4(5)", "456.7(4)", "456.7" } };
		d2.push_back("987");
		EXPECT_EQ(d2.back(), "987");

		std::string val{ "654" };
		d2.push_back(val);
		EXPECT_EQ(d2.back(), "654");

		d2.emplace_back("321");
		EXPECT_EQ(d2.back(), "321");


	}

	TEST(ciffile_Datavalue, modifiers_swap) {

		//swap
		Datavalue d3{ { "123.4(5)", "456.7(4)", "456.7" } };
		std::vector<std::string> str3_can{ "123.4(5)", "456.7(4)", "456.7" };
		std::vector<double> dbl3_can{ 123.4, 456.7, 456.7 };
		std::vector<double> err3_can{ 0.5, 0.4, 0.0 };

		Datavalue d4{ { "456.7(4)", "123.4(5)", "456.7" } };
		d4.convert();
		std::vector<std::string> str4_can{ "456.7(4)", "123.4(5)", "456.7" };
		std::vector<double> dbl4_can{ 456.7, 123.4, 456.7 };
		std::vector<double> err4_can{ 0.4, 0.5,0.0 };


		EXPECT_FALSE(d3.isConverted());
		EXPECT_EQ(d3.getStrings(), str3_can);

		EXPECT_TRUE(d4.isConverted());
		EXPECT_EQ(d4.getStrings(), str4_can);

		for (size_t i{ 0 }; i < d4.size(); ++i) {
			EXPECT_DOUBLE_EQ(d4.getDoubles()[i], dbl4_can[i]);
			EXPECT_DOUBLE_EQ(d4.getErrors()[i], err4_can[i]);
		}

		d3.swap(d4);

		EXPECT_FALSE(d4.isConverted());
		EXPECT_EQ(d4.getStrings(), str3_can);

		EXPECT_TRUE(d3.isConverted());
		EXPECT_EQ(d3.getStrings(), str4_can);
		for (size_t i{ 0 }; i < d4.size(); ++i) {
			EXPECT_DOUBLE_EQ(d3.getDoubles()[i], dbl4_can[i]);
			EXPECT_DOUBLE_EQ(d3.getErrors()[i], err4_can[i]);
		}




		Datavalue d5{ { "123.4(5)", "456.7(4)", "456.7" } };
		d5.convert();
		std::vector<std::string> str5_can{ "123.4(5)", "456.7(4)", "456.7" };
		std::vector<double> dbl5_can{ 123.4, 456.7, 456.7 };
		std::vector<double> err5_can{ 0.5, 0.4, 0.0 };

		Datavalue d6{ { "456.7(4)", "123.4(5)", "456.7" } };
		std::vector<std::string> str6_can{ "456.7(4)", "123.4(5)", "456.7" };
		std::vector<double> dbl6_can{ 456.7, 123.4, 456.7 };
		std::vector<double> err6_can{ 0.4, 0.5,0.0 };


		EXPECT_TRUE(d5.isConverted());
		EXPECT_EQ(d5.getStrings(), str5_can);

		EXPECT_FALSE(d6.isConverted());
		EXPECT_EQ(d6.getStrings(), str6_can);


		for (size_t i{ 0 }; i < d5.size(); ++i) {
			EXPECT_DOUBLE_EQ(d5.getDoubles()[i], dbl5_can[i]);
			EXPECT_DOUBLE_EQ(d5.getErrors()[i], err5_can[i]);
		}

		d5.swap(d6);

		EXPECT_FALSE(d5.isConverted());
		EXPECT_EQ(d5.getStrings(), str6_can);

		EXPECT_TRUE(d6.isConverted());
		EXPECT_EQ(d6.getStrings(), str5_can);
		for (size_t i{ 0 }; i < d4.size(); ++i) {
			EXPECT_DOUBLE_EQ(d6.getDoubles()[i], dbl5_can[i]);
			EXPECT_DOUBLE_EQ(d6.getErrors()[i], err5_can[i]);
		}

	}

	TEST(ciffile_Datavalue, nonmember) {
		Datavalue d1{ { "123.4(5)", "456.7(4)", "78.9" } };
		Datavalue d2{ { "123.4(5)", "456.7", "78.9" } };
		Datavalue d3{ { "123.4(5)", "456.7(4)", "78.9" } };

		EXPECT_TRUE(d1 == d3);
		EXPECT_FALSE(d1 == d2);

		swap(d2, d3);

		EXPECT_FALSE(d1 == d3);
		EXPECT_TRUE(d1 == d2);

	}




}