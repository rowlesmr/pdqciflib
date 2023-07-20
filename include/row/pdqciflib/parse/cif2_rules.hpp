
#pragma once

#include <iostream>
#include <stdexcept>
#include "tao_dev/pegtl.hpp"

//#include "ciffile.hpp"
//#include "cifexcept.hpp"

namespace row::cif::rules
{

    namespace pegtl = tao::pegtl;
    namespace pegtl8 = tao::pegtl::utf8;


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
	struct line_term : pegtl::sor<pegtl::seq<pegtl::one<'\r'>, pegtl::opt<pegtl::one<'\n'>>>, pegtl::one<'\n'>> {};
	struct comment : pegtl::if_must<pegtl::one<'#'>, pegtl::until<pegtl::eolf>> {};
	//struct comment : pegtl::if_must<pegtl::one<'#'>, pegtl::star<char_>> {};
	struct ws : pegtl::blank {}; // pegtl::one<' ', '\t'>
	struct wschar : pegtl::sor<ws, line_term> {}; // pegtl::space{}; //pegtl::one<' ', '\n', '\r', '\t', '\v', '\f'>
	struct whitespace : pegtl::plus<pegtl::sor<wschar, comment>> {};
	struct ws_or_eof : pegtl::sor<whitespace, pegtl::eof> {};

    //text block
	struct text_delim : pegtl::seq<pegtl::bol, pegtl::one<';'>> {};
	struct end_text_delim : pegtl::seq<pegtl::eol, pegtl::one<';'>> {};
	struct text_content : pegtl::star<pegtl::not_at<end_text_delim>, allchars> {};
	struct text_field : pegtl::if_must<text_delim, text_content, end_text_delim> {};

    //triple-quote block
	struct ec_triple_close_quote : pegtl::success {};
	struct apostrophe3_delim : TAO_PEGTL_STRING("'''") {};
	struct apostrophe3_content : pegtl::star<pegtl::opt<pegtl::seq<pegtl::one<'\''>, pegtl::opt<pegtl::one<'\''>>>>, pegtl::plus<not_3apostrophe>> {};
	struct apostrophe3 : pegtl::if_must<apostrophe3_delim, apostrophe3_content, pegtl::sor<apostrophe3_delim, ec_triple_close_quote>> {};

	struct quote3_delim : TAO_PEGTL_STRING("\"\"\"") {};
	struct quote3_content : pegtl::star<pegtl::opt<pegtl::seq<pegtl::one<'"'>, pegtl::opt<pegtl::one<'"'>>>>, pegtl::plus<not_3quote>> {};
	struct quote3 : pegtl::if_must<quote3_delim, quote3_content, pegtl::sor<quote3_delim, ec_triple_close_quote>> {};

    struct triple_quoted_string : pegtl::sor<quote3, apostrophe3> {};

    //single-quote block
	struct ec_single_close_quote : pegtl::success {};
	struct apostrophe1_delim : TAO_PEGTL_STRING("'") {};
	struct apostrophe1_content : pegtl::star<not_1apostrophe> {};
	struct apostrophe1 : pegtl::if_must<apostrophe1_delim, apostrophe1_content, pegtl::sor<apostrophe1_delim, ec_single_close_quote>> {};

	struct quote1_delim : TAO_PEGTL_STRING("\"") {};
	struct quote1_content : pegtl::star<not_1quote> {};
	struct quote1 : pegtl::if_must<quote1_delim, quote1_content, pegtl::sor<quote1_delim, ec_single_close_quote>> {};

	struct single_quoted_string : pegtl::sor<quote1, apostrophe1> {};

    //unquoted string
    struct unquoted_string : pegtl::seq<pegtl::not_at<pegtl::sor<pegtl::seq<pegtl::sor<DATA, SAVE>, pegtl::star<non_blank_char>>, LOOP, GLOBAL, STOP>>, lead_char, pegtl::star<restrict_char>> {};

	//other values
	struct lst;
	struct table;
	struct data_name;

	//special data values
	struct not_applicable : pegtl::one<'.'> {};
	struct unknown : pegtl::one<'?'> {};
	struct special : pegtl::sor<not_applicable, unknown> {};

	struct value : pegtl::sor<special, text_field, unquoted_string, triple_quoted_string, single_quoted_string, lst, table> {};

    //Table
	struct ec_table_ws : whitespace {};
	struct ec_table_key_format : pegtl::sor<text_field, unquoted_string> {};
	struct ec_table_last_value_missing : pegtl::success {};
	struct ec_table_colon_missing : pegtl::success {};
	struct ec_table_missing_close : pegtl::success {};;


	//struct item : pegtl::if_must<data_name, whitespace, data_value, ws_or_eof, pegtl::opt<ec_data_values>, pegtl::opt<ws_or_eof>> {};

	struct table_key : pegtl::sor<triple_quoted_string, single_quoted_string, ec_table_key_format> {};
	struct table_value : value {};
    struct table_entry : pegtl::if_must<table_key, pegtl::opt<ec_table_ws>, pegtl::sor<pegtl::one<':'>, ec_table_colon_missing>, pegtl::opt<whitespace>, pegtl::sor<table_value, ec_table_last_value_missing>> {};
	struct table_begin : pegtl::one<'{'> {};
	struct table_end : pegtl::one<'}'> {};
	struct table : pegtl::if_must <
			                    table_begin,
			                    pegtl::opt<pegtl::opt<whitespace>, table_entry, pegtl::star<whitespace, table_entry>>,
			                    pegtl::opt<whitespace, pegtl::not_at<pegtl::one<'_'>>>,
			                    pegtl::sor<table_end, ec_table_missing_close>
		                        >{};

	//List
	struct ec_list_missing_close : pegtl::success {};
	struct ec_list_comma : pegtl::one<','> {};
	struct lst_begin : pegtl::one<'['> {};
	struct lst_end : pegtl::one<']'> {};
	struct lst_value : value {};
	struct lst_values : pegtl::seq<pegtl::opt<whitespace>, lst_value, pegtl::opt<ec_list_comma>, pegtl::star<whitespace, lst_value, pegtl::opt<ec_list_comma>>> {};
	struct lst : pegtl::if_must<lst_begin, pegtl::opt<lst_values>, pegtl::opt<whitespace, pegtl::not_at<pegtl::one<'_'>>>, pegtl::sor<lst_end, ec_list_missing_close>> {};

    //Data name    
	struct data_name : pegtl::seq<pegtl::one<'_'>, pegtl::sor<pegtl::plus<non_blank_char>, TAO_PEGTL_RAISE_MESSAGE("Malformed data name.")>> {};

    //Loop
	struct loop_begin : pegtl::seq<LOOP, whitespace> {};
	struct loop_end : pegtl::opt<ws_or_eof> {};
	struct loop_data_name : data_name {};
	struct loop_value : value {};
	struct loop_data_names : pegtl::plus<pegtl::seq<loop_data_name, whitespace, pegtl::discard>> {};
	struct loop_data_values : pegtl::sor<pegtl::plus<pegtl::seq<loop_value, ws_or_eof, pegtl::discard>>, pegtl::at<pegtl::sor<reserved, pegtl::eof>>> {};
	struct loop : pegtl::if_must<loop_begin, loop_data_names, loop_data_values, loop_end> {};

    //Data
	struct ec_list_missing_open : pegtl::one<']'> {};;
	struct ec_extra_data_values : pegtl::plus<pegtl::sor<value, ec_list_missing_open>, pegtl::star<whitespace>> {};
	struct data_value : pegtl::sor<value, ec_list_missing_open> {};
	struct item : pegtl::if_must<data_name, whitespace, data_value, ws_or_eof, pegtl::opt<ec_extra_data_values>, pegtl::opt<ws_or_eof>> {};
	//struct item : pegtl::if_must<data_name, whitespace, pegtl::if_then_else<data_value, ws_or_eof, TAO_PEGTL_RAISE_MESSAGE("Malformed or missing value.")>, pegtl::discard> {};
	struct data : pegtl::sor<item, loop> {};

    //Container code
    struct container_code : pegtl::plus<non_blank_char> {};

    //saveframe
	struct save_frame;
    struct frame_content : pegtl::sor<data, save_frame> {};
    struct save_heading : pegtl::seq<SAVE, container_code> {};
    struct save_frame : pegtl::if_must<save_heading, whitespace, pegtl::star<frame_content>, SAVE, ws_or_eof> {};

    //data block
	struct ec_data_heading : pegtl::seq<pegtl::plus<ws>, pegtl::plus<value, pegtl::star<ws>>> {};

	struct block_content : pegtl::sor<data, save_frame> {};
	struct data_heading : pegtl::if_must<DATA, container_code, pegtl::opt<ec_data_heading>> {};
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


} //end namespace 

