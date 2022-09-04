
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

//TEST(TestCaseName, TestName) {
//
//	
//	EXPECT_EQ(1, 1);
//	EXPECT_TRUE(true);
//}


namespace cifparseTest {

	TEST(cifparseTest, unquoted_strings) {
		std::string str{ 
R"(#\#CIF_1.1
data_cif11_unquoted
# This value is the one described in GitHub issue 22:
_bracket_mid Fc^*^=kFc[1+0.001xFc^2^\l^3^/sin(2\q)]^-1/4^

_bracket_end a[42]
_brace_begin {foo}bar
_brace_mid   bar{foo}bar
_brace_end   bar{foo}
)" };


		Cif cif;
		EXPECT_NO_THROW({
			cif = read_string(str);
		});


		EXPECT_EQ(
			std::string("Fc^*^=kFc[1+0.001xFc^2^\\l^3^/sin(2\\q)]^-1/4^"), 
			cif.get("cif11_unquoted").get("_bracket_mid")[0] 
		);
		EXPECT_EQ(
			std::string("a[42]"),
			cif.get("cif11_unquoted").get("_bracket_end")[0]
		);
		EXPECT_EQ(
			std::string("{foo}bar"),
			cif.get("cif11_unquoted").get("_brace_begin")[0]
		);
		EXPECT_EQ(
			std::string("bar{foo}bar"),
			cif.get("cif11_unquoted").get("_brace_mid")[0]
		);
		EXPECT_EQ(
			std::string("bar{foo}"),
			cif.get("cif11_unquoted").get("_brace_end")[0]
		);

	}

	TEST(cifparseTest, quoted_strings) {
		std::string str{ 
R"(#\#CIF_1.1

data_cif1_quoting

_sq 'don't rock the boat'
_dq "What's this ab\"out?" 
)" }; //this is an o with an umlaut: ö

		Cif cif;
		EXPECT_NO_THROW({
			cif = read_string(str);
		});

		EXPECT_EQ(
			std::string("don't rock the boat"),
			cif.get("cif1_quoting").get("_sq")[0]
		);
		EXPECT_EQ(
			std::string("What's this ab\\\"out?"),
			cif.get("cif1_quoting").get("_dq")[0]
		);

	}

	TEST(cifparseTest, comment_only) {
		std::string str{ 
R"(# Nothing but comments in this CIF.
# Without a leading CIF 2.0 version comment, it should be parsed as CIF 1.1,
# but the(lack of) data content would be the same if it were parsed as CIF 2.0
)" };

		EXPECT_NO_THROW({
			Cif cif = read_string(str);
			});
	}

	TEST(cifparseTest, block_names) {
		std::string str{ 
R"(#\#CIF_1.1
data_with[1]
_item1 hello

data_with{2}
_item2 world
)" };

		EXPECT_NO_THROW({
			Cif cif = read_string(str);
			});
	}

	TEST(cifparseTest, simple_loops) {
		std::string str{ 
R"(#\#CIF_1.1
data_simple_loops

loop_
_col1
_col2
_col3
1 v1 ?
2
; v2
; 1.0
3 'v3' 12.5(2)

loop_
_single
1 2 3

loop_
_scalar_a _scalar_b
'a'
'b'
)" };

		EXPECT_NO_THROW({
			Cif cif = read_string(str);
			});
	}






	TEST(cifparseTest, empty_file) {
		std::string str = "";

		EXPECT_NO_THROW({
			Cif cif = read_string(str);
			});
	}
	TEST(cifparseTest, invalid_file) {

		{
			std::string str{ 
R"(#\#CIF_1.1

data_d
# valid in CIF 1: the starting '[' is in quotes
_goodname '['k']'
# invalid in CIF 1: can't start with a '['.
_badname ['k']
)" };
			EXPECT_THROW({
				try {
					Cif cif = read_string(str);
				}
				catch (const std::runtime_error& e) {
					EXPECT_STREQ("Parsing error -> string:7:10: Malformed or missing value.", e.what());
					throw;
				}

				}, std::runtime_error);
		}

		{
			std::string str{
R"(#\#CIF_1.1
data_simple_loops

loop_
_col1
_col2
_col3
1 v1 ?
2
; v2
; 1.0
3 'v3' 
)" };

			EXPECT_THROW({
				try {
					Cif cif = read_string(str);
				}
				catch (const std::runtime_error& e) {
					EXPECT_STREQ("Parsing error -> string:4:1: 1 too few, or 2 too many, values in loop.", e.what());
					throw;
				}

				}, std::runtime_error);
		}

		{
			std::string str{
R"(#\#CIF_1.1

data_d
# max line length of 2048
_badlength "This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long. This line is too long."
)" };
			//Currently, I don't see any reason to not support long lines
			EXPECT_NO_THROW({ Cif cif = read_string(str); });
		}

	}

	//TEST(ciffile_Datavalue, nonmember) {
	//	Datavalue d1{ { "123.4(5)", "456.7(4)", "78.9" } };
	//	Datavalue d2{ { "123.4(5)", "456.7", "78.9" } };
	//	Datavalue d3{ { "123.4(5)", "456.7(4)", "78.9" } };

	//	EXPECT_TRUE(d1 == d3);
	//	EXPECT_FALSE(d1 == d2);

	//	swap(d2, d3);

	//	EXPECT_FALSE(d1 == d3);
	//	EXPECT_TRUE(d1 == d2);

	//}




}
