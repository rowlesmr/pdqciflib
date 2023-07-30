
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
#include "../structure/structure.hpp"

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
		Block curr_block{ nullptr };
		std::string curr_tag{};
		
		std::string content{};
		std::vector<ErrorCorrection> errors{};

		pegtl::position value_position;
		pegtl::position tag_position;
		pegtl::position loop_position;
		pegtl::position block_position;

		bool is_unknown{ false };
		bool is_notapplicable{ false };
		
		bool is_table_or_list{ false };
		bool just_closed_table_or_list{ false };

		size_t list_depth{ 0 };
		size_t table_depth{ 0 };

		std::vector<dataname> loop_tags{};
		std::vector<datavalue> loop_values{};

		bool missing_tag_name{ false };

		bool first_extra_data_value{ true };


		json::events::to_value build_value;

		Buffer() 
			: value_position(0, 0, 0, "unknown"),
			tag_position(0, 0, 0, "unknown"),
			loop_position(0, 0, 0, "unknown"),
			block_position(0, 0, 0, "unknown")
		{};

		explicit Buffer(std::string source) 
			: source(std::move(source)), 
			value_position(0, 0, 0, "unknown"),
			tag_position(0, 0, 0, "unknown"),
			loop_position(0, 0, 0, "unknown"),
			block_position(0, 0, 0, "unknown")
		{};

		void add_error(std::string message, pegtl::position position)
		{
			//std::cout << "### " << message << ":" << position << '\n';
			errors.push_back({ std::move(message), std::move(position) });
		}

		void reset_value()
		{
			content.clear();
			value_position.byte = 0;
			value_position.column = 0;
			value_position.line = 0;
			is_unknown = false;
			is_notapplicable = false;
			is_table_or_list = false;
			just_closed_table_or_list = false;
			list_depth = 0;
			table_depth = 0;
			build_value.reset();
			missing_tag_name = false;
			first_extra_data_value = true;
		}

		void reset_tag()
		{
			tag_position.byte = 0;
			tag_position.column = 0;
			tag_position.line = 0;
			curr_tag = "";
		}

		void reset_loop()
		{
			loop_tags.clear();
			loop_values.clear();
			loop_position.byte = 0;
			loop_position.column = 0;
			loop_position.line = 0;
		}

		void clear_all()
		{
			reset_value();
			reset_tag();
			reset_loop();
			errors.clear();
			source.clear();
			value_position.source = "unknown";
			tag_position.source = "unknown";
			loop_position.source = "unknown";
			block_position.source = "unknown";
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
