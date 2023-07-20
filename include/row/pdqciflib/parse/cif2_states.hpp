
#pragma once


#include <iostream>
#include <stdexcept>
#include "tao_dev/pegtl.hpp"

//#include "ciffile.hpp"
//#include "cifexcept.hpp"
#include "cif2_rules.hpp"

namespace row::cif::states
{

	namespace pegtl = tao::pegtl;
	namespace pegtl8 = tao::pegtl::utf8;
	namespace rules = row::cif::rules;

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

	//to temporarily store data before putting it in the Cif object
	struct Buffer
	{
		std::string source{};
		std::string content{};
		bool is_unknown{ false };
		bool is_notapplicable{ false };
		size_t position_byte{};
		size_t position_column{};
		size_t position_line{};

		void clear()
		{
		    content.clear();
			is_unknown = false;
			is_unknown = false;
			position_byte = 0;
			position_column = 0;
			position_line = 0;
		}
	};





#if 0


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



}
