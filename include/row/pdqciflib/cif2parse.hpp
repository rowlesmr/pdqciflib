
#ifndef ROW_CIFPARSE_HPP
#define ROW_CIFPARSE_HPP


#include <iostream>
#include <stdexcept>
#include "tao/pegtl.hpp"

#include "ciffile.hpp"
#include "cifexcept.hpp"

namespace row::cif
{

    namespace pegtl = tao::pegtl;
    namespace pegtl8 = tao::pegtl::utf8;

    namespace rules
    {
        //reserved words
        struct DATA : TAO_PEGTL_ISTRING("data_") {};
        struct LOOP : TAO_PEGTL_ISTRING("loop_") {};
        struct GLOBAL : TAO_PEGTL_ISTRING("global_") {};
        struct SAVE : TAO_PEGTL_ISTRING("save_") {};
        struct STOP : TAO_PEGTL_ISTRING("stop_") {};
		struct reserved : pegtl::sor<DATA, LOOP, SAVE, GLOBAL, STOP> {};

        
        //character sets
		//almost all of unicode, as per CIF2.0 grammar
		struct allchars : pegtl8::ranges<0x0009, 0x0009, 0x000A, 0x000A, 0x000D, 0x000D, 0x0020, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};
		//allchars - ('\r', '\n')
		struct char_ : pegtl8::ranges<0x0009, 0x0009, 0x0020, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD>{};
		//char_ - (' ', '\t')
		struct non_blank_char : pegtl8::ranges<0x0021, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD>{};
		//non_blank_char - ('[', ']', '{', '}')
		struct restrict_char : pegtl8::ranges<0x0021, 0x005A, 0x005C, 0x005C, 0x005E, 0x007A, 0x007C, 0x007C, 0x007E, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD>{};
		//restrict_char - ('"', '#', '$', "'", '_')
		struct lead_char : pegtl8::ranges<0x0021, 0x0021, 0x0025, 0x0026, 0x0028, 0x005A, 0x005C, 0x005C, 0x005E, 0x005E, 0x0060, 0x007A, 0x007C, 0x007C, 0x007E, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD>{};
    
		//allchars - ("'")
        struct not_3apostrophe : pegtl8::ranges<0x0009, 0x0009, 0x000A, 0x000A, 0x000D, 0x000D, 0x0020, 0x0026, 0x0028, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};
		//allchars - ('"')
		struct not_3quote : pegtl8::ranges<0x0009, 0x0009, 0x000A, 0x000A, 0x000D, 0x000D, 0x0020, 0x0021, 0x0023, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};

		//char_ - ("'")
		struct not_1apostrophe : pegtl8::ranges<0x0009, 0x0009, 0x0020, 0x0026, 0x0028, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};
		//char_ - ('"')
		struct not_1quote : pegtl8::ranges<0x0009, 0x0009, 0x0020, 0x0021, 0x0023, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};




        //Whitespace, and comments
		//struct line_term : pegtl::sor<pegtl::seq<pegtl::one<'\r'>, pegtl::opt<pegtl::one<'\n'>>>, pegtl::one<'\n'>> {};
		struct comment : pegtl::if_must<pegtl::one<'#'>, pegtl::until<pegtl::eolf>> {};
		struct ws : pegtl::blank {}; // pegtl::one<' ', '\t'>
		struct wschar : pegtl::space {}; //pegtl::one<' ', '\n', '\r', '\t', '\v', '\f'>
		struct whitespace : pegtl::plus<pegtl::sor<wschar, comment>> {};
		struct ws_or_eof : pegtl::sor<whitespace, pegtl::eof> {};

		//special data values
		struct not_applicable : pegtl::one<'.'> {};
		struct unknown : pegtl::one<'?'> {};
		struct special : pegtl::sor<not_applicable, unknown> {};

        //text block
		struct text_delim : pegtl::seq<pegtl::bol, pegtl::one<';'>> {};
		struct end_text_delim : pegtl::seq<pegtl::eol, pegtl::one<';'>> {};
		struct text_content : pegtl::star<pegtl::not_at<end_text_delim>, allchars> {};
		struct text_field : pegtl::if_must<text_delim, text_content, end_text_delim> {};

        //triple-quote block
        struct apostrophe3_delim : TAO_PEGTL_STRING("'''") {};
		struct apostrophe3_content : pegtl::star<pegtl::opt<pegtl::seq<pegtl::one<'\''>, pegtl::opt<pegtl::one<'\''>>>>, pegtl::plus<not_3apostrophe>> {};
		//struct apostrophe3 : pegtl::seq<apostrophe3_delim, pegtl::until<apostrophe3_delim, apostrophe3_content>> {};
		struct apostrophe3 : pegtl::if_must<apostrophe3_delim, apostrophe3_content, apostrophe3_delim> {};

		struct quote3_delim : TAO_PEGTL_STRING("\"\"\"") {};
		struct quote3_content : pegtl::star<pegtl::opt<pegtl::seq<pegtl::one<'"'>, pegtl::opt<pegtl::one<'"'>>>>, pegtl::plus<not_3quote>> {};
		struct quote3 : pegtl::if_must<quote3_delim, quote3_content, quote3_delim> {};

        struct triple_quoted_string : pegtl::sor<quote3, apostrophe3> {};

        //single-quote block
		struct apostrophe1_delim : TAO_PEGTL_STRING("'") {};
		struct apostrophe1_content : pegtl::star<not_1apostrophe> {};
		struct apostrophe1 : pegtl::if_must<apostrophe1_delim, apostrophe1_content, apostrophe1_delim> {};

		struct quote1_delim : TAO_PEGTL_STRING("\"") {};
		struct quote1_content : pegtl::star<not_1quote> {};
		struct quote1 : pegtl::if_must<quote1_delim, quote1_content, quote1_delim> {};

		struct single_quoted_string : pegtl::sor<quote1, apostrophe1> {};

        //unquoted string
        struct unquoted_string : pegtl::seq<
			                               pegtl::not_at<
			                                             pegtl::sor<
			                                                        pegtl::seq<
			                                                                   pegtl::sor<DATA, SAVE>, pegtl::star<non_blank_char>
																	          >, 
			                                                        LOOP, GLOBAL, STOP
			                                                       >
										                >, 
			                               lead_char, 
			                               pegtl::star<restrict_char>
		                                  > {};

		//other values
		struct lst;
		struct table;

		struct value : pegtl::sor<special, text_field, unquoted_string, triple_quoted_string, single_quoted_string, lst, table> {};

        //table
		struct table_key : pegtl::sor<triple_quoted_string, single_quoted_string> {};
		struct table_value : value {};
        struct table_entry : pegtl::if_must<table_key, pegtl::one<':'>, pegtl::opt<whitespace>, table_value> {};
		struct table_begin : pegtl::one<'{'> {};
		struct table_end : pegtl::one<'}'> {};
		struct table : pegtl::if_must <
			                      table_begin,
			                      pegtl::opt<pegtl::opt<whitespace>, table_entry, pegtl::star<whitespace, table_entry>>,
			                      pegtl::opt<whitespace>,
			                      table_end
		                         >{};

		//list
		struct lst_begin : pegtl::one<'['> {};
		struct lst_end : pegtl::one<']'> {};
		struct lst_value : value {};
		struct lst_values : pegtl::opt<pegtl::opt<whitespace>, lst_value, pegtl::star<whitespace, lst_value>> {};
		struct lst : pegtl::if_must<
			                    lst_begin,
			                    lst_values, 
			                    pegtl::opt<whitespace>,
			                    lst_end
		                       > {};


        //Data name    
		struct data_name : pegtl::seq<pegtl::one<'_'>, pegtl::sor<pegtl::plus<non_blank_char>, TAO_PEGTL_RAISE_MESSAGE("Malformed data name.")>> {};

        //Loop
		struct loop_begin : pegtl::seq<LOOP, whitespace> {};
		struct loop_end : pegtl::opt<ws_or_eof> {};
		struct loop_data_name : data_name {};
		struct loop_value : value {};
		struct loop_data_names : pegtl::plus<pegtl::seq<loop_data_name, whitespace, pegtl::discard>> {};
		struct loop_data_values : pegtl::sor<pegtl::plus<pegtl::seq<loop_value, ws_or_eof, pegtl::discard>>, pegtl::at<pegtl::sor<reserved, pegtl::eof>>> {};
		struct loop : pegtl::if_must<
		                             loop_begin,
		                             loop_data_names,
		                             loop_data_values,
		                             loop_end
		                            > {};

        //Data
		struct data_value : value {};
		struct item : pegtl::if_must<data_name, whitespace, pegtl::if_then_else<data_value, ws_or_eof, TAO_PEGTL_RAISE_MESSAGE("Malformed or missing value.")>, pegtl::discard> {};
		struct data : pegtl::sor<item, loop> {};

        //Container code
        struct container_code : pegtl::plus<non_blank_char> {};

        //saveframe
		struct save_frame;
        struct frame_content : pegtl::seq<data> {};
        struct save_heading : pegtl::if_must<SAVE, container_code> {};
        struct save_frame : pegtl::if_must<save_heading, whitespace, pegtl::star<frame_content>, SAVE, ws_or_eof> {};

        //data block
		struct block_content : pegtl::sor<data, save_frame> {};
		struct data_heading : pegtl::if_must<DATA, container_code> {};
		struct data_block : pegtl::if_must<data_heading, whitespace, pegtl::star<block_content>> {};

        //CIF2 file
        struct magic_code : TAO_PEGTL_STRING("#\\#CIF_2.0") {};
        struct file_heading : pegtl::seq<pegtl::opt<pegtl8::bom>, magic_code, pegtl::star<ws>> {};
        struct CIF2_file : pegtl::if_must<file_heading, pegtl::opt<pegtl::eol, 
			                                                       pegtl::opt<pegtl::opt<whitespace>,
			                                                                  pegtl::plus<data_block>
															                 >, 
			                                                       pegtl::opt<whitespace>
		                                                          >,
		                                  pegtl::eof
		                                 > {};
    } //end namespace rules


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


    //********************
    // Parsing Actions to populate the values in the ciffile
    //********************
    template<typename Rule>
    struct Action : pegtl::nothing<Rule> {};

    template<typename Rule>
    struct Action_inner : pegtl::nothing<Rule> {};

	template<> struct Action<rules::text_content>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "text_content: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::text_field>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "text_field concluded.\n";
		}
	};

	template<> struct Action<rules::apostrophe3_content>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe3_content: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::apostrophe3>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe3 concluded.\n";
		}
	};

	template<> struct Action<rules::quote3_content>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "quote3_content: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::quote3>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "quote3 concluded.\n";
		}
	};

	template<> struct Action<rules::apostrophe1_content>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe1_content: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::apostrophe1>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe1 concluded.\n";
		}
	};

	template<> struct Action<rules::quote1_content>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "quote1_content: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::quote1>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "quote1 concluded.\n";
		}
	};

	template<> struct Action<rules::unquoted_string>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "unquoted_string: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::not_applicable>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "NA\n";
		}
	};

	template<> struct Action<rules::unknown>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "UNK\n";
		}
	};

	template<> struct Action<rules::value>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "value concluded.\n";
		}
	};

	template<> struct Action<rules::table_key>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_key concluded.\n";
		}
	};

	template<> struct Action<rules::table_value>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_value concluded.\n";
		}
	};

	template<> struct Action<rules::table_entry>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_entry concluded.\n";
		}
	};

	template<> struct Action<rules::table_begin>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "TABLE BEGINS.\n";
		}
	};

	template<> struct Action<rules::table>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "TABLE ENDS.\n";
		}
	};

	template<> struct Action<rules::lst_begin>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "LIST BEGINS.\n";
		}
	};

	template<> struct Action<rules::lst_value>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "list value concluded.\n";
		}
	};

	template<> struct Action<rules::lst>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "LIST ENDS.\n";
		}
	};

	template<> struct Action<rules::lst_values>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Got all list values.\n";
		}
	};

	template<> struct Action<rules::data_name>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "data_name: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::loop_begin>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "LOOP BEGINS.\n";
		}
	};

	template<> struct Action<rules::loop_data_name>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "loop_data_name: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::loop_value>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "loop_value: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::loop_data_names>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Loop header concluded.\n";
		}
	};

	template<> struct Action<rules::loop_data_values>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Loop data values concluded.\n";
		}
	};

	template<> struct Action<rules::loop>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "LOOP ENDS.\n";
		}
	};

	template<> struct Action<rules::data_value>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "data_value: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::item>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Pair concluded.\n";
		}
	};

	template<> struct Action<rules::data>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Data concluded.\n";
		}
	};

	template<> struct Action<rules::container_code>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "container_code: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::frame_content>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "Frame content concluded.\n";
		}
	};

	template<> struct Action<rules::save_heading>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "SAVEFRAME BEGINS.\n";
		}
	};

	template<> struct Action<rules::save_frame>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
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

	template<> struct Action<rules::data_heading>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "DATA BLOCK BEGINS.\n";
		}
	};

	template<> struct Action<rules::data_block>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
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

	template<> struct Action<rules::CIF2_file>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "CIF2 file concluded.\n";
		}
	};


    template<typename Input>
    void parse_input(Input&& in, bool printErr = true)
    {
        try
        {
            //Status status{};
            //Buffer buffer{};
			pegtl::parse<rules::CIF2_file, Action>(in); // , d, status, buffer);
        }
        catch (pegtl::parse_error& e)
        {
            const auto p = e.positions().front();
            //pretty-print the error msg and the line that caused it, with an indicator at the token that done it.
            if (printErr)
            {
                std::cerr << e.what() << '\n'
                    << in.line_at(p) << '\n'
                    << std::setw(p.column) << '^' << std::endl;
            }
            throw std::runtime_error("Parsing error -> " + std::string(e.what()));
        }
    }

    template<typename Input>
	/*Cif*/ void read_input(Input&& in, bool printErr = true)
    {
        parse_input(in, printErr);
    }

    //read in a file into a Cif. Will throw std::runtime_error if it encounters problems
    inline /*Cif*/ void read_file(const std::string& filename, bool overwrite = false, bool printErr = true) noexcept(false)
    {
        pegtl::file_input<pegtl::tracking_mode::eager, pegtl::eol::lf_crlf> in(filename);
        return read_input(in, printErr);
    }

    //read a string into a Cif. Will throw std::runtime_error if it encounters problems
    inline /*Cif*/ void read_string(const std::string& cifstring, bool printErr = true, const std::string& source = "string") noexcept(false)
    {
        pegtl::string_input<pegtl::tracking_mode::eager, pegtl::eol::lf_crlf> in(cifstring, source);
        read_input(in, printErr);
    }

}
#endif // !ROW_CIFPARSE_HPP
