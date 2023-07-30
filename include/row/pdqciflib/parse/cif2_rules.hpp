
#pragma once

#include <iostream>
#include <stdexcept>
#include "tao/pegtl.hpp"

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
	struct sc_text_delim_open : pegtl::seq<pegtl::bol, pegtl::one<';'>> {};
	struct sc_text_delim_close : pegtl::seq<pegtl::eol, pegtl::one<';'>> {};
	struct sc_text_content : pegtl::star<pegtl::not_at<sc_text_delim_close>, allchars> {};
	struct sc_text_field : pegtl::if_must<sc_text_delim_open, sc_text_content, sc_text_delim_close> {};



	//rules for folded semicolon textfields
	struct f_text_continuation : pegtl::seq<pegtl::one<'\\'>, pegtl::star<ws>, pegtl::eol> {};
	struct f_text_delim_open : pegtl::seq<sc_text_delim_open, f_text_continuation> {};
	struct f_text_delim_close : sc_text_delim_close {};
	struct f_text_content_for_parsing : sc_text_content {};
	struct folded_text_field : pegtl::if_must<f_text_delim_open, f_text_content_for_parsing, f_text_delim_close> {};


	struct fsctf_text : pegtl::plus<pegtl::not_at<f_text_continuation>, allchars> {};
	struct fsctf_line_without_continuation : fsctf_text {};
	struct fsctf_line_with_continuation : pegtl::seq<fsctf_text, f_text_continuation> {};
	struct fsctf_grammar : pegtl::seq<pegtl::star<pegtl::plus<fsctf_line_with_continuation>, pegtl::star<fsctf_line_without_continuation>>, fsctf_line_without_continuation> {};


	struct text_field : sc_text_field {};



    //triple-quote block
	struct ec_triple_close_quote : pegtl::success {};

	struct apostrophe3_delim : TAO_PEGTL_STRING("'''") {};
	struct apostrophe3_delim_open : apostrophe3_delim {};
	struct apostrophe3_delim_close : apostrophe3_delim {};
	struct apostrophe3_content : pegtl::star<pegtl::opt<pegtl::seq<pegtl::one<'\''>, pegtl::opt<pegtl::one<'\''>>>>, pegtl::plus<not_3apostrophe>> {};
	struct apostrophe3 : pegtl::if_must<apostrophe3_delim_open, apostrophe3_content, pegtl::sor<apostrophe3_delim_close, ec_triple_close_quote>> {};

	struct quote3_delim : TAO_PEGTL_STRING("\"\"\"") {};
	struct quote3_delim_open : quote3_delim {};
	struct quote3_delim_close : quote3_delim {};
	struct quote3_content : pegtl::star<pegtl::opt<pegtl::seq<pegtl::one<'"'>, pegtl::opt<pegtl::one<'"'>>>>, pegtl::plus<not_3quote>> {};
	struct quote3 : pegtl::if_must<quote3_delim_open, quote3_content, pegtl::sor<quote3_delim_close, ec_triple_close_quote>> {};

    struct triple_quoted_string : pegtl::sor<quote3, apostrophe3> {};

    //single-quote block
	struct ec_single_close_quote : pegtl::success {};

	struct apostrophe1_delim : TAO_PEGTL_STRING("'") {};
	struct apostrophe1_delim_open : apostrophe1_delim {};
	struct apostrophe1_delim_close : apostrophe1_delim {};
	struct apostrophe1_content : pegtl::star<not_1apostrophe> {};
	struct apostrophe1 : pegtl::if_must<apostrophe1_delim_open, apostrophe1_content, pegtl::sor<apostrophe1_delim_close, ec_single_close_quote>> {};

	struct quote1_delim : TAO_PEGTL_STRING("\"") {};
	struct quote1_delim_open : quote1_delim {};
	struct quote1_delim_close : quote1_delim {};
	struct quote1_content : pegtl::star<not_1quote> {};
	struct quote1 : pegtl::if_must<quote1_delim_open, quote1_content, pegtl::sor<quote1_delim_close, ec_single_close_quote>> {};

	struct single_quoted_string : pegtl::sor<quote1, apostrophe1> {};

    //unquoted string
	struct ec_illegal_unquote_char : pegtl::seq<non_blank_char, restrict_char> {};
    struct unquoted_string : pegtl::seq<pegtl::not_at<pegtl::sor<pegtl::seq<pegtl::sor<DATA, SAVE>, pegtl::star<non_blank_char>>, LOOP, GLOBAL, STOP>>, 
		                                lead_char, pegtl::star<pegtl::sor<restrict_char, ec_illegal_unquote_char>>
	                                   > {};

	//special data values
	struct not_applicable : pegtl::one<'.'> {};
	struct unknown : pegtl::one<'?'> {};
	struct special : pegtl::sor<not_applicable, unknown> {};

	//forward declarations for rules
	struct lst;
	struct table;
	struct data_name;

	//single value
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
	struct table_entries: pegtl::seq<pegtl::opt<whitespace>, table_entry, pegtl::star<whitespace, table_entry>>{};
	struct table_begin : pegtl::one<'{'> {};
	struct table_end : pegtl::one<'}'> {};
	struct table : pegtl::if_must <
	                               table_begin,
	                               pegtl::opt<table_entries>,
	                               pegtl::opt<whitespace, pegtl::not_at<pegtl::sor<pegtl::one<'_'>, reserved>>>,
	                               pegtl::sor<table_end, ec_table_missing_close>
	                              > {};

	//List
	struct ec_list_missing_close : pegtl::success {};
	struct ec_list_comma : pegtl::one<','> {};

	struct lst_begin : pegtl::one<'['> {};
	struct lst_end : pegtl::one<']'> {};
	struct lst_value : value {};
	struct lst_values : pegtl::seq<pegtl::opt<whitespace>, lst_value, pegtl::opt<ec_list_comma>, pegtl::star<whitespace, lst_value, pegtl::opt<ec_list_comma>>> {};
	struct lst : pegtl::if_must<
		lst_begin,
		pegtl::opt<lst_values>,
		pegtl::opt<whitespace, pegtl::not_at<pegtl::sor<pegtl::one<'_'>, reserved>>>,
		pegtl::sor<lst_end, ec_list_missing_close>
	>
	{};

    //Data name  
	struct ec_missing_tag_name : pegtl::success {};
	struct data_name : pegtl::seq<pegtl::one<'_'>, pegtl::sor<pegtl::plus<non_blank_char>, ec_missing_tag_name>> {};

    //Loop
	struct ec_no_loop_tags : pegtl::success {};
	struct loop_begin : pegtl::seq<LOOP, whitespace> {};
	struct loop_end : pegtl::opt<ws_or_eof> {};
	struct loop_data_name : data_name {};
	struct loop_value : value {};
	struct loop_data_names : pegtl::sor<pegtl::plus<pegtl::seq<loop_data_name, whitespace, pegtl::discard>>, ec_no_loop_tags> {};
	struct loop_data_values : pegtl::sor<pegtl::plus<pegtl::seq<loop_value, ws_or_eof, pegtl::discard>>, pegtl::at<pegtl::sor<reserved, pegtl::eof>>> {};
	struct loop : pegtl::if_must<loop_begin, loop_data_names, loop_data_values, loop_end> {};

    //Data
	struct ec_list_missing_open : pegtl::one<']'> {};
	struct ec_extra_data_value : value {};
	struct ec_extra_data_values : pegtl::plus<pegtl::sor<ec_extra_data_value, ec_list_missing_open>, pegtl::star<whitespace>> {};
	struct ec_multiple_data_name : data_name {};
	struct ec_multiple_data_names : pegtl::seq<ec_multiple_data_name, pegtl::star<whitespace, ec_multiple_data_name>> {};

	struct data_value : pegtl::sor<value, ec_list_missing_open> {};
	struct pair : pegtl::if_must<data_name, whitespace, pegtl::sor<data_value, ec_multiple_data_names>, ws_or_eof, pegtl::opt<ec_extra_data_values>, pegtl::opt<ws_or_eof>> {};
	struct data : pegtl::sor<pair, loop> {};

    //Container code
    struct container_code : pegtl::plus<non_blank_char> {};

    //saveframe
	struct save_frame;
    struct frame_content : pegtl::sor<data, save_frame> {};
    struct save_heading : pegtl::seq<SAVE, container_code> {};
    struct save_frame : pegtl::if_must<save_heading, whitespace, pegtl::star<frame_content>, SAVE, ws_or_eof> {};

    //data block
	struct ec_data_heading : pegtl::seq<pegtl::plus<ws>, pegtl::plus<value, pegtl::star<ws>>> {};
	struct ec_data_block_name_missing : pegtl::success {};

	struct block_content : pegtl::sor<data, save_frame> {};
	struct data_heading : pegtl::if_must<DATA, pegtl::sor<container_code, ec_data_block_name_missing>, pegtl::opt<ec_data_heading>> {};
	struct data_block : pegtl::if_must<data_heading, whitespace, pegtl::star<block_content>> {};

    //CIF2 file
	struct magic_code_prefix : TAO_PEGTL_STRING("#\\#CIF_") {};
	struct magic_code_version : TAO_PEGTL_STRING("2.0") {};
    struct magic_code : pegtl::seq<magic_code_prefix, magic_code_version> {};
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

