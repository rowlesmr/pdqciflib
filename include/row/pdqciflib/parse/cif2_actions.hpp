
#pragma once


#include <iostream>
#include <stdexcept>
#include "tao/pegtl.hpp"

//#include "ciffile.hpp"
//#include "cifexcept.hpp"
#include "cif2_rules.hpp"
#include "cif2_states.hpp"
#include "cif2_states.hpp"
#include "..\structure\structure.hpp"

namespace row::cif::actions
{

	namespace pegtl = tao::pegtl;
	namespace pegtl8 = tao::pegtl::utf8;
	namespace rules = row::cif::rules;
	namespace states = row::cif::states;


#if 0

	/*TODO: things to buffer
	container_code
	text_content
	apostrophe3_content
	quote3_content
	apostrophe1_content
	quote1_content
	unquoted_string
	is_unknown
	is_notApplicable

	triple_quote_string start line and pos
	text_field start line and pos
	loop start line and pos
	*/

	////to keep track of whether the quote string has been sent to file
	//struct Status
	//{
	//    bool is_loop{ false };
	//    bool is_quote{ false };
	//    bool is_printed{ false };
	//    bool is_fsctf{ false };

	//    void reset()
	//    {
	//        is_loop = false;
	//        is_quote = false;
	//        is_printed = false;
	//        is_fsctf = false;
	//    }

	//    void get_ready_to_print()
	//    {
	//        is_quote = true;
	//        is_printed = false;
	//    }

	//    void just_printed()
	//    {
	//        is_printed = true;
	//        is_fsctf = false;
	//    }

	//    void finished_printing()
	//    {
	//        is_quote = false;
	//        is_printed = false;
	//    }

	//    void loop()
	//    {
	//        is_loop = !is_loop;
	//    }

	//};


	////to temporarily store data before putting it in the Cif object
	//struct Buffer
	//{
	//    dataname tag{};

	//    std::string fsctf_string{};
	//    std::vector<dataname> tags{};
	//    std::vector<Datavalue> values{};
	//    size_t loopNum{};
	//    size_t maxLoop{};
	//    size_t totalValues{};
	//    size_t tagNum{};

	//    void initialiseValues()
	//    {
	//        if (values.empty())
	//        {
	//            maxLoop = tags.size();
	//            values = std::vector<Datavalue>(maxLoop);
	//        }
	//    }

	//    void appendTag(std::string in_tag)
	//    {
	//        tags.push_back(std::move(in_tag));
	//        ++tagNum;
	//    }

	//    void appendValue(std::string val)
	//    {
	//        values[loopNum].push_back(std::move(val));
	//        loopNum = ++loopNum % maxLoop;
	//        ++totalValues;
	//    }

	//    void clear()
	//    {
	//        tag.clear();
	//        fsctf_string.clear();
	//        tags.clear();
	//        values.clear();
	//        loopNum = 0;
	//        maxLoop = 0;
	//        totalValues = 0;
	//        tagNum = 0;
	//    }
	//};


	//template<typename Input>
	//void divert_action_to_value(const Input& in, Cif& out, Status& status, Buffer& buffer)
	//{
	//    status.get_ready_to_print();
	//    if (status.is_loop)
	//    {
	//        Action<rules::loopvalue>::apply(in, out, status, buffer);
	//    }
	//    else
	//    {
	//        Action<rules::itemvalue>::apply(in, out, status, buffer);
	//    }
	//}




#endif


	//********************
	// Parsing Actions to populate the values in the ciffile
	//********************
	template<typename Rule>
	struct action : pegtl::nothing<Rule> {};

	
	//error correction actions  ec_illegal_unquote_char

	template<> struct action<rules::ec_illegal_unquote_char>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("[, ], {, or } found in unquoted string.", in.position());
		}
	};

	template<> struct action<rules::ec_no_loop_tags>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("No loop tags found.", in.position());
		}
	};

	template<> struct action<rules::ec_missing_tag_name>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("Data name missing a name.", in.position());
		}
	};

	template<> struct action<rules::ec_data_block_name_missing>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("Data block missing a name.", in.position());
		}
	};

	template<> struct action<rules::ec_triple_close_quote>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("Triple-quote missing closing quotes.", in.position());
		}
	};

	template<> struct action<rules::ec_list_comma>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("List contains comma as separator.", in.position());
		}
	};

	template<> struct action<rules::ec_list_missing_close>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("List missing closing character.", buffer.position);
		}
	};

	template<> struct action<rules::ec_list_missing_open>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("List missing opening character.", in.position());
		}
	};

	template<> struct action<rules::ec_table_missing_close>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("Table missing closing character.", buffer.position);
		}
	}; 

	template<> struct action<rules::ec_table_colon_missing>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("Table entry missing colon.", in.position());
		}
	};

	template<> struct action<rules::ec_table_last_value_missing>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("Table value missing.", in.position());
		}
	};

	template<> struct action<rules::ec_table_key_format>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("Table key must be single- or triple-delimited string.", in.position());
		}
	};

	template<> struct action<rules::ec_table_ws>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("Whitespace between table key and colon.", in.position());
		}
	};

	template<> struct action<rules::ec_single_close_quote>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("Missing closing single-quote delimiter.", in.position());
		}
	};

	template<> struct action<rules::ec_extra_data_values>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("Extra values after data value.", in.position());
		}
	}; 

	template<> struct action<rules::ec_multiple_data_name>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.add_error("Extra data name after data name.", in.position());
		}
	};

	template<> struct action<rules::ec_multiple_data_names>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{			
			buffer.add_error("Extra data names after data name.", in.position());
		}
	};

	template<> struct action<rules::ec_data_heading>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			//todo: get copy of in.string. Replace all ' ' and '\t' with '_' and store in buffer.content.
			buffer.add_error("Extra names after data block name.", in.position());
		}
	};




	//normal actions

	//######################################################################
	template<> struct action<rules::text_delim_open>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.position = in.position();
			std::cout << "text_delim_open: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::text_delim_close>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "text_delim_close: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::text_content>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.content = in.string();
			std::cout << "text_content: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::text_field>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "text_field: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::apostrophe3_delim_open>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.position = in.position();
			std::cout << "apostrophe3_delim_open: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::apostrophe3_delim_close>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe3_delim_close: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::apostrophe3_content>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.content = in.string();
			std::cout << "apostrophe3_content: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::apostrophe3>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe3: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::quote3_delim_open>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.position = in.position();
			std::cout << "quote3_delim_open: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::quote3_delim_close>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "quote3_delim_close: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::quote3_content>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.content = in.string();
			std::cout << "quote3_content: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::quote3>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "quote3: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::apostrophe1_delim_open>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.position = in.position();
			std::cout << "apostrophe1_delim_open: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::apostrophe1_delim_close>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe1_delim_close: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::apostrophe1_content>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.content = in.string();
			std::cout << "apostrophe1_content: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::apostrophe1>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe1: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::quote1_delim_open>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.position = in.position();
			std::cout << "quote1_delim_open: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::quote1_delim_close>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "quote1_delim_close: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::quote1_content>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.content = in.string();
			std::cout << "quote1_content: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::quote1>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "quote1: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::unquoted_string>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.content = in.string();
			std::cout << "unquoted_string: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::not_applicable>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.is_notapplicable = true;
			std::cout << "not_applicable: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::unknown>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.is_unknown = true;
			std::cout << "not_applicable: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::special>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "special: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::value>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "value: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::table_begin>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			++buffer.table_depth;
			buffer.is_table_or_list = true;
			buffer.position = in.position();
			std::cout << "table_begin: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::table_end>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			--buffer.table_depth;
			std::cout << "table_end: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::table_key>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_key: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::table_value>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_value: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::table_entry>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_entry: |" << in.string() << "|\n";
		}
	};
	template<> struct action<rules::table>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::lst_begin>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			++buffer.list_depth;
			buffer.is_table_or_list = true;
			buffer.position = in.position();
			std::cout << "lst_begin: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::lst_end>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			--buffer.list_depth;
			std::cout << "lst_end: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::lst_value>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "lst_value: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::lst_values>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "lst_values: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::lst>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "lst: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::data_name>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "data_name: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::loop_begin>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.position = in.position();
			std::cout << "loop_begin: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::loop_end>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "loop_end: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::loop_data_name>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			++buffer.loop_tags;
			std::cout << "loop_data_name: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::loop_value>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			++buffer.loop_values;
			std::cout << "loop_value: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::loop_data_names>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "loop_data_names: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::loop_data_values>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "loop_data_values: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::loop>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			if(buffer.loop_tags > 0)
			{
				if (buffer.loop_values == 0)
				{
					buffer.add_error("No values in loop.", buffer.position);
					std::cout << "No values in loop.\n";
				}

				size_t should_be_zero = buffer.loop_values % buffer.loop_tags;
				if(should_be_zero != 0)
				{
					buffer.add_error("Number of loop values inconsistent with number of loop tags.", buffer.position);
					std::cout << "Number of loop values inconsistent with number of loop tags: |" << should_be_zero << "|\n";
				}
			}
			std::cout << "loop: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::data_value>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "data_value: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::pair>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "pair: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::data>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.reset();
			std::cout << "data: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::container_code>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.content = in.string();
			std::cout << "container_code: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::frame_content>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "frame_content: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::save_heading>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "save_heading: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::save_frame>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "save_frame: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::block_content>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
		std::cout << "block_content: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::data_heading>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.curr_block = buffer.content;
			std::cout << "data_heading: |" << in.string() << "| (" << buffer.content << ")\n";
		}
	};

	template<> struct action<rules::data_block>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "data_block: |" << in.string() << "|\n";
		}
	};

	//######################################################################
	template<> struct action<rules::magic_code>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "magic_code: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::file_heading>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "file_heading: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::CIF2_file>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "CIF2_file: |" << in.string() << "|\n";
		}
	};
}
