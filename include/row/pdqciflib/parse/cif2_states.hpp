
#pragma once


#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "tao/pegtl.hpp"
#include "tao/json.hpp"

//#include "ciffile.hpp"
//#include "cifexcept.hpp"
#include "cif2_rules.hpp"

namespace row::cif::states
{

	namespace pegtl = tao::pegtl;
	namespace pegtl8 = tao::pegtl::utf8;
	namespace json = tao::json;
	namespace rules = row::cif::rules;


	struct ErrorCorrection
	{
		std::string message{};
		pegtl::position position;

		ErrorCorrection() 
			: position(0, 0, 0, "unk")
		{};

		ErrorCorrection(std::string message, pegtl::position position) 
			: message(std::move(message)), position(std::move(position))
		{};

	};
	inline std::ostream& operator<<(std::ostream& os, const ErrorCorrection& ec)
	{
		return os << ec.position << ": " << ec.message;
	}

	[[nodiscard]] inline std::string to_string(const ErrorCorrection& ec)
	{
		std::ostringstream oss;
		oss << ec;
		return std::move(oss).str();
	}



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
		std::string curr_block{};
		std::string curr_tag{};
		
		std::string content{};
		pegtl::position position;
		std::vector<ErrorCorrection> errors{};

		bool is_unknown{ false };
		bool is_notapplicable{ false };
		
		bool is_table_or_list{ false };

		size_t list_depth{ 0 };
		size_t table_depth{ 0 };

		size_t loop_tags{ 0 };
		size_t loop_values{ 0 };

		json::events::to_value build_value;

		Buffer() 
			: position(0, 0, 0, "unknown") 
		{};

		explicit Buffer(std::string source) 
			: source(std::move(source)), position(0,0,0,"unknown") 
		{};

		void add_error(std::string message, pegtl::position position)
		{
			std::cout << "### " << message << ":" << position << '\n';
			errors.push_back({ std::move(message), std::move(position) });
		}


		void reset()
		{
		    content.clear();
			is_unknown = false;
			is_unknown = false;
			position.byte = 0;
			position.column = 0;
			position.line = 0;
			is_table_or_list = false;
			list_depth = 0;
			table_depth = 0;
			loop_tags = 0;
			loop_values = 0;
			curr_tag = "";
			build_value.reset();
			//build_value.stack_.clear();
			//build_value.keys_.clear();
			//build_value.value_.set_uninitialized();
		}

		void clear()
		{
			reset();
			errors.clear();
			source.clear();
			position.source = "unknown";
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
