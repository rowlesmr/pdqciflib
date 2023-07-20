
#pragma once


#include <iostream>
#include <stdexcept>
#include "tao_dev/pegtl.hpp"

//#include "ciffile.hpp"
//#include "cifexcept.hpp"
#include "cif2_rules.hpp"
#include "cif2_states.hpp"

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

	template<typename Rule>
	struct Action_inner : pegtl::nothing<Rule> {};

	template<> struct action<rules::ec_triple_close_quote>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "### Missing closing \"\"\" or '''. |" << in.position() << "|\n";
		}
	};

	template<> struct action<rules::ec_list_comma>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "### List uses ',' to separate items.\n";
		}
	};

	template<> struct action<rules::ec_list_missing_close>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "### Missing list close character.\n";
		}
	};

	template<> struct action<rules::ec_list_missing_open>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "### Missing list begin character.\n";
		}
	};

	template<> struct action<rules::ec_table_last_value_missing>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "### Table last value missing.\n";
		}
	};

	template<> struct action<rules::ec_table_key_format>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "### Table keys to be delimited with single- or triple-quotes only.\n";
		}
	};

	template<> struct action<rules::ec_table_ws>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "### Whitespace between table key and ':'\n";
		}
	};

	template<> struct action<rules::ec_single_close_quote>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "### Missing closing \" or '.\n";
		}
	};

	template<> struct action<rules::ec_extra_data_values>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "### ec_data_values: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::ec_data_heading>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "### ec_data_heading: |" << in.string() << "|\n";
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
			std::cout << "text_field concluded.\n";
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
			std::cout << "apostrophe3 concluded.\n";
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
			std::cout << "quote3 concluded.\n";
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
			std::cout << "apostrophe1 concluded.\n";
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
			std::cout << "quote1 concluded.\n";
		}
	};

	template<> struct action<rules::unquoted_string>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.content = in.string();
			std::cout << "unquoted_string: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::not_applicable>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "NA\n";
		}
	};

	template<> struct action<rules::unknown>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "UNK\n";
		}
	};

	template<> struct action<rules::value>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "value concluded.\n";
		}
	};

	template<> struct action<rules::table_key>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_key concluded.\n";
		}
	};

	template<> struct action<rules::table_value>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_value concluded.\n";
		}
	};

	template<> struct action<rules::table_entry>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_entry concluded.\n";
		}
	};

	template<> struct action<rules::table_begin>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "TABLE BEGINS.\n";
		}
	};

	template<> struct action<rules::table>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "TABLE ENDS.\n";
		}
	};

	template<> struct action<rules::lst_begin>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "LIST BEGINS.\n";
		}
	};

	template<> struct action<rules::lst_value>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "list value concluded.\n";
		}
	};

	template<> struct action<rules::lst>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "LIST ENDS.\n";
		}
	};

	template<> struct action<rules::lst_values>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Got all list values.\n";
		}
	};

	template<> struct action<rules::data_name>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "data_name: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::loop_begin>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "LOOP BEGINS.\n";
		}
	};

	template<> struct action<rules::loop_data_name>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "loop_data_name: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::loop_value>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "loop_value: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::loop_data_names>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Loop header concluded.\n";
		}
	};

	template<> struct action<rules::loop_data_values>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Loop data values concluded.\n";
		}
	};

	template<> struct action<rules::loop>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "LOOP ENDS.\n";
		}
	};

	template<> struct action<rules::data_value>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "data_value: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::item>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Pair concluded.\n";
		}
	};

	template<> struct action<rules::data>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Data concluded.\n";
		}
	};

	template<> struct action<rules::container_code>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			buffer.content = in.string();
			std::cout << "container_code: |" << in.string() << "|\n";
		}
	};

	template<> struct action<rules::frame_content>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Frame content concluded.\n";
		}
	};

	template<> struct action<rules::save_heading>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "SAVEFRAME BEGINS.\n";
		}
	};

	template<> struct action<rules::save_frame>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "SAVEFRAME ENDS.\n";
		}
	};

	//template<> struct Action<rules::block_content>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "block_content: |" << in.string() << "|\n";
	//	}
	//};

	template<> struct action<rules::data_heading>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "DATA BLOCK BEGINS.\n";
		}
	};

	template<> struct action<rules::data_block>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "DATA BLOCK ENDS.\n";
		}
	};

	//template<> struct Action<rules::magic_code>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "magic_code: |" << in.string() << "|\n";
	//	}
	//};

	//template<> struct Action<rules::file_heading>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "file_heading: |" << in.string() << "|\n";
	//	}
	//};

	template<> struct action<rules::CIF2_file>
	{
		template<typename Input> static void apply(const Input& in, [[maybe_unused]] states::Buffer& buffer) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "CIF2 file concluded.\n";
		}
	};



}
