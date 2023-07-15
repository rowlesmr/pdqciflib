
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

        
        //character sets
		//almost all of unicode, as per CIF2.0 grammar
		struct allchars : pegtl8::ranges<0x0009, 0x0009, 0x000A, 0x000A, 0x000D, 0x000D, 0x0020, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};
		//allchars - ('\n', '\r')
		struct char_ : pegtl8::ranges<0x0009, 0x0009, 0x0020, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD>{};
		//char_ - (' ', '\t')
		struct non_blank_char : pegtl8::ranges<0x0021, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD>{};
		//non_blank_char - ('[', ']', '{', '}')
		//struct restrict_char : pegtl8::ranges<0x0021, 0x005A, 0x005C, 0x005C, 0x005E, 0x007A, 0x007C, 0x007C, 0x007E, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD>{};
		//restrict_char - ('"', '#', '$', "'", '_')
		//struct lead_char : pegtl8::ranges<0x0021, 0x0021, 0x0025, 0x0026, 0x0028, 0x005A, 0x005C, 0x005C, 0x005E, 0x005E, 0x0060, 0x007A, 0x007C, 0x007C, 0x007E, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD>{};
    
		//allchars - ("'")
        struct not_3apostrophe : pegtl8::ranges<0x0009, 0x0009, 0x000A, 0x000A, 0x000D, 0x000D, 0x0020, 0x0026, 0x0028, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};
		//allchars - ('"')
		struct not_3quote : pegtl8::ranges<0x0009, 0x0009, 0x000A, 0x000A, 0x000D, 0x000D, 0x0020, 0x0021, 0x0023, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};

		//char_ - ("'")
		struct not_1apostrophe : pegtl8::ranges<0x0009, 0x0009, 0x0020, 0x0026, 0x0028, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};
		//char_ - ('"')
		struct not_1quote : pegtl8::ranges<0x0009, 0x0009, 0x0020, 0x0021, 0x0023, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};




        //Whitespace, line termination, and comments
        struct line_term : pegtl::sor<pegtl::seq<pegtl::one<'\r'>,pegtl::opt<pegtl::one<'\n'>>>, pegtl::one<'\n'>> {};
        struct inline_wspace : pegtl::one<' ', '\t'> {};
        struct comment : pegtl::seq<pegtl::one<'#'>, 
			                        pegtl::star<char_>
		                           > {};
		//struct comment : pegtl::if_must<pegtl::one<'#'>,
		//	                            pegtl::star<char_>
		//> {};
        struct wspace_to_eol : pegtl::seq<pegtl::star<inline_wspace>, 
			                              pegtl::opt<comment>, 
			                              line_term
		                                 > {};
        struct wspace_any : pegtl::seq<pegtl::star<wspace_to_eol>, 
			                           pegtl::star<inline_wspace>
		                              > {};
        struct wspace_lines : pegtl::seq<pegtl::opt<pegtl::plus<inline_wspace>, 
			                                        pegtl::opt<comment>
		                                           >, 
			                             line_term, 
			                             pegtl::star<wspace_to_eol>
		                                > {};
        struct wspace : pegtl::seq<pegtl::sor<inline_wspace, line_term>, 
			                       wspace_any
		                          > {};

        //text block
        struct text_delim : pegtl::seq<line_term, pegtl::one<';'>> {};
		struct text_content : allchars {};
		struct text_field : pegtl::seq<text_delim, pegtl::until<text_delim, text_content>> {};

        //triple-quote block
        struct apostrophe3_delim : TAO_PEGTL_STRING("'''") {};
		struct apostrophe3_content : pegtl::star<pegtl::opt<pegtl::seq<pegtl::one<'\''>, pegtl::opt<pegtl::one<'\''>>>>, pegtl::plus<not_3apostrophe>> {};
		//struct apostrophe3 : pegtl::seq<apostrophe3_delim, pegtl::until<apostrophe3_delim, apostrophe3_content>> {};
		struct apostrophe3 : pegtl::seq<apostrophe3_delim, apostrophe3_content, apostrophe3_delim> {};

		struct quote3_delim : TAO_PEGTL_STRING("\"\"\"") {};
		struct quote3_content : pegtl::star<pegtl::opt<pegtl::seq<pegtl::one<'"'>, pegtl::opt<pegtl::one<'"'>>>>, pegtl::plus<not_3quote>> {};
		struct quote3 : pegtl::seq<quote3_delim, quote3_content, quote3_delim> {};

        struct triple_quoted_string : pegtl::sor<quote3, apostrophe3> {};

        //single-quote block
		struct apostrophe1_delim : TAO_PEGTL_STRING("'") {};
		struct apostrophe1_content : pegtl::star<not_1apostrophe> {};
		struct apostrophe1 : pegtl::seq<apostrophe1_delim, apostrophe1_content, apostrophe1_delim> {};

		struct quote1_delim : TAO_PEGTL_STRING("\"") {};
		struct quote1_content : pegtl::star<not_1quote> {};
		struct quote1 : pegtl::seq<quote1_delim, quote1_content, quote1_delim> {};

		struct single_quoted_string : pegtl::sor<quote1, apostrophe1> {};

        //strings and values
		struct restrict_char_removal : pegtl::sor<pegtl::one<'['>, pegtl::one<']'>, pegtl::one<'{'>, pegtl::one<'}'>> {};;
		struct lead_char_removal : pegtl::sor<pegtl::one<'"'>, pegtl::one<'#'>, pegtl::one<'$'>, pegtl::one<'\''>, pegtl::one<'_'>> {};;
		struct restrict_char : pegtl::seq<pegtl::not_at<restrict_char_removal>, non_blank_char> {};
		struct lead_char : pegtl::seq<pegtl::not_at<lead_char_removal>, restrict_char> {};

        struct wsdelim_string : pegtl::seq<
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
		//struct wsdelim_string_sol : pegtl::seq<pegtl::not_at<pegtl::seq<pegtl::one<';'>, pegtl::star<non_blank_char>>>, wsdelim_string> {};
		struct wsdelim_string_sol : pegtl::minus<wsdelim_string, pegtl::seq<pegtl::one<';'>, pegtl::star<non_blank_char>>>{};

        struct lst;
        struct table;

        struct nospace_value : pegtl::sor<triple_quoted_string, single_quoted_string, lst, table> {};
		struct wspace_data_value : pegtl::sor<pegtl::seq<wspace, nospace_value>,
                                              pegtl::seq<pegtl::opt<wspace_lines>, pegtl::plus<inline_wspace>, wsdelim_string>,
			                                  pegtl::seq<wspace_lines, wsdelim_string_sol>,
			                                  pegtl::seq<pegtl::opt<wspace>, pegtl::opt<comment>, text_field>
                                             > {};

		struct rowles_data_value : pegtl::sor<pegtl::seq<wspace, nospace_value>,
			pegtl::seq<pegtl::opt<wspace_lines>, pegtl::plus<inline_wspace>, wsdelim_string>,
			pegtl::seq<wspace_lines, wsdelim_string_sol>,
			pegtl::seq<pegtl::opt<wspace>, pegtl::opt<comment>, text_field>
		>
		{};



		//struct wspace_data_value : pegtl::seq<pegtl::opt<wspace>, pegtl::opt<comment>, text_field>{};

        //table
		struct table_key : pegtl::sor<triple_quoted_string, single_quoted_string> {};
		struct table_value : pegtl::sor<nospace_value, wsdelim_string, wspace_data_value> {};
        struct table_entry : pegtl::if_must<table_key, pegtl::one<':'>, table_value> {};
		struct table_begin : pegtl::one<'{'> {};
		struct table_end : pegtl::one<'}'> {};
		struct table : pegtl::if_must <
			                      table_begin,
			                      pegtl::opt<
			                                 wspace_any, 
			                                 table_entry, 
			                                 pegtl::star<wspace, table_entry>
								            >, 
			                      pegtl::opt<wspace>, 
			                      table_end
		                         >{};

        //list
        struct list_values_start : pegtl::sor<
			                                  pegtl::seq<wspace_any, nospace_value>,
			                                  pegtl::seq<wspace_any, pegtl::opt<comment>, text_field>,
			                                  pegtl::seq<pegtl::opt<pegtl::star<wspace_to_eol>, pegtl::plus<inline_wspace>>, wsdelim_string>,
			                                  pegtl::seq<pegtl::plus<wspace_to_eol>, wsdelim_string_sol>
                                             >{};
		struct lst_begin : pegtl::one<'['> {};
		struct lst_end : pegtl::one<']'> {};
		struct lst_value : pegtl::opt<list_values_start, pegtl::star<wspace_data_value>> {};
		struct lst : pegtl::if_must<
			                    lst_begin,
			                    lst_value, 
			                    pegtl::opt<wspace>, 
			                    lst_end
		                       > {};


        //Tag    
		struct data_name : pegtl::seq<pegtl::one<'_'>, pegtl::plus<non_blank_char>> {}; //pegtl::sor<pegtl::plus<non_blank_char>, TAO_PEGTL_RAISE_MESSAGE("Malformed tag name.")>

        //Loop
		struct loop_data_name : data_name {};
		struct loop_data_value : wspace_data_value {};
		struct loop_begin : LOOP {};
		struct data_loop : pegtl::if_must<
			                          loop_begin,
			                          pegtl::plus<wspace, loop_data_name>,
			                          pegtl::plus<loop_data_value>
		                             > {};

        //Data
        struct data : pegtl::sor<
			                     pegtl::if_must<data_name, wspace_data_value>, 
			                     data_loop
		                        > {};

        //Container code
        struct container_code : pegtl::plus<non_blank_char> {};

        //saveframe
        struct frame_content : pegtl::seq<wspace, data> {};
        struct save_heading : pegtl::if_must<SAVE, container_code> {};
        struct save_frame : pegtl::if_must<save_heading, pegtl::star<frame_content>, wspace, SAVE> {};

        //data block
		struct block_content : pegtl::seq<wspace, pegtl::sor<data, save_frame>> {};
		struct data_heading : pegtl::if_must<DATA, container_code> {};
		struct data_block : pegtl::if_must<data_heading, pegtl::star<block_content>> {};

        //CIF2 file
        struct magic_code : TAO_PEGTL_STRING("#\\#CIF_2.0") {};
        struct file_heading : pegtl::seq<pegtl::opt<pegtl8::bom>, magic_code, pegtl::star<inline_wspace>> {};
        struct CIF2_file : pegtl::seq<file_heading, pegtl::opt<line_term, 
			                                                   pegtl::opt<wspace_any, 
			                                                              data_block, 
			                                                              pegtl::star<wspace, data_block>
															             >, 
			                                                   pegtl::opt<wspace>, 
			                                                   pegtl::opt<comment>
		                                                      >,
		                              pegtl::eolf
		                             > {};
		//, TAO_PEGTL_RAISE_MESSAGE("Got to here.")

        ////Tags and values
        //struct tag : pegtl::seq<pegtl::one<'_'>, pegtl::sor<pegtl::plus<nonblankchar>, TAO_PEGTL_RAISE_MESSAGE("Malformed tag name.")>> {};
        //struct value : pegtl::sor<numeric, textfield, charstring> {}; //need to check for textfield first, as a charstring can start with a semicolon
        //struct itemtag : tag {};
        //struct itemvalue : value {};
        //struct looptag : tag {};
        //struct loopvalue : value {};

        ////CIF Structure
        ////loop
        //struct loopend : pegtl::opt<STOP, ws_or_eof> {};
        //struct looptags : pegtl::plus<pegtl::seq<looptag, whitespace, pegtl::discard>> {};
        //struct loopvalues : pegtl::sor<pegtl::plus<pegtl::seq<loopvalue, ws_or_eof, pegtl::discard>>, /* handle incorrect CIF with empty loop -->*/ pegtl::at<pegtl::sor<reserved, pegtl::eof>>> {};
        //struct loopstart : pegtl::seq<LOOP, whitespace> {};
        //struct loop : pegtl::if_must<loopstart, looptags, loopvalues, loopend> {};

    } //end namespace rules


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

	template<> struct Action<rules::file_heading>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "file_heading: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::magic_code>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "magic_code: |" << in.string() << "|\n";
		}
	};

	//template<> struct Action<rules::data_block>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "data_block: |" << in.string() << "|\n";
	//	}
	//};

	template<> struct Action<rules::data_heading>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "data_heading: |" << in.string() << "|\n";
		}
	};

	//template<> struct Action<rules::block_content>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "block_content: |" << in.string() << "|\n";
	//	}
	//};

	//template<> struct Action<rules::save_frame>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "save_frame: |" << in.string() << "|\n";
	//	}
	//};

	template<> struct Action<rules::save_heading>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "save_heading: |" << in.string() << "|\n";
		}
	};

	//template<> struct Action<rules::frame_content>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "frame_content: |" << in.string() << "|\n";
	//	}
	//};

	//template<> struct Action<rules::container_code>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "container_code: |" << in.string() << "|\n";
	//	}
	//};

	//template<> struct Action<rules::data>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "data: |" << in.string() << "|\n";
	//	}
	//};

	template<> struct Action<rules::loop_begin>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "A LOOP HAS BEGUN!\n";
		}
	};

	template<> struct Action<rules::loop_data_name>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "loop_data_name: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::loop_data_value>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "loop_data_value: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::data_loop>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "data_loop: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::data_name>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "data_name: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::lst_begin>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "A LIST HAS BEGUN!\n";
		}
	};

	template<> struct Action<rules::lst>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "lst: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::lst_end>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "A LIST HAS ENDED!\n";
		}
	};

	template<> struct Action<rules::lst_value>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "lst_value: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::table_begin>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "A TABLE HAS BEGUN!\n";
		}
	};

	template<> struct Action<rules::table>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::table_end>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "A TABLE HAS ENDED!\n";
		}
	};

	template<> struct Action<rules::table_entry>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_entry: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::table_key>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_key: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::table_value>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "table_value: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::wspace_data_value>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "wspace_data_value: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::nospace_value>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "nospace_value: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::wsdelim_string_sol>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "wsdelim_string_sol: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::wsdelim_string>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "wsdelim_string: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::single_quoted_string>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "single_quoted_string: |" << in.string() << "|\n";
		}
	};

	//template<> struct Action<rules::quote1>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "quote1: |" << in.string() << "|\n";
	//	}
	//};

	template<> struct Action<rules::quote1_content>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "quote1_content: |" << in.string() << "|\n";
		}
	};

	//template<> struct Action<rules::quote1_delim>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "quote1_delim: |" << in.string() << "|\n";
	//	}
	//};

	template<> struct Action<rules::apostrophe1>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe1: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::apostrophe1_content>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe1_content: |" << in.string() << "|\n";
		}
	};

	//template<> struct Action<rules::apostrophe1_delim>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "apostrophe1_delim: |" << in.string() << "|\n";
	//	}
	//};

	template<> struct Action<rules::triple_quoted_string>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "triple_quoted_string: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::quote3>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "quote3: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::quote3_content>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "quote3_content: |" << in.string() << "|\n";
		}
	};

	//template<> struct Action<rules::quote3_delim>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "quote3_delim: |" << in.string() << "|\n";
	//	}
	//};

	template<> struct Action<rules::apostrophe3>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe3: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::apostrophe3_content>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "apostrophe3_content: |" << in.string() << "|\n";
		}
	};

	//template<> struct Action<rules::apostrophe3_delim>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "apostrophe3_delim: |" << in.string() << "|\n";
	//	}
	//};

	template<> struct Action<rules::text_field>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "text_field: |" << in.string() << "|\n";
		}
	};

	template<> struct Action<rules::text_content>
	{
		template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
		{
			std::cout << "text_content: |" << in.string() << "|\n";
		}
	};

	//template<> struct Action<rules::text_delim>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "text_delim: |" << in.string() << "|\n";
	//	}
	//};






	//template<> struct Action<rules::wspace>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "wspace: |" << in.string() << "|\n";
	//	}
	//};

	//template<> struct Action<rules::wspace_lines>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "wspace_lines: |" << in.string() << "|\n";
	//	}
	//};

	//template<> struct Action<rules::wspace_any>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "wspace_any: |" << in.string() << "|\n";
	//	}
	//};

	//template<> struct Action<rules::wspace_to_eol>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "wspace_to_eol: |" << in.string() << "|\n";
	//	}
	//};

	//template<> struct Action<rules::comment>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "comment: |" << in.string() << "|\n";
	//	}
	//};

	//template<> struct Action<rules::inline_wspace>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "inline_wspace: |" << in.string() << "|\n";
	//	}
	//};

	//template<> struct Action<rules::line_term>
	//{
	//	template<typename Input> static void apply(const Input& in) //, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer)
	//	{
	//		std::cout << "line_term: |" << in.string() << "|\n";
	//	}
	//};


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

    ////read in a file into a Cif. Will throw std::runtime_error if it encounters problems
    //inline /*Cif*/ void read_file(const std::string& filename, bool overwrite = false, bool printErr = true) noexcept(false)
    //{
    //    pegtl::file_input in(filename);
    //    return read_input(in, overwrite, printErr);
    //}

    //read a string into a Cif. Will throw std::runtime_error if it encounters problems
    inline /*Cif*/ void read_string(const std::string& cifstring, bool printErr = true, const std::string& source = "string") noexcept(false)
    {
        pegtl::string_input in(cifstring, source);
        read_input(in, printErr);
    }

}
#endif // !ROW_CIFPARSE_HPP
