


#include <iostream>
#include <string>
#include <filesystem>


#define MINE

#ifdef MINE
#include "tao/pegtl/contrib/analyze.hpp"
#include "tao/pegtl/contrib/trace.hpp"
#include "tao/pegtl.hpp"

#include "pdqciflib.hpp"

namespace pegtl = tao::pegtl;
namespace pegtl8 = tao::pegtl::utf8;
//
////character sets
//struct allchars : pegtl8::ranges<0x0009, 0x0009, 0x000A, 0x000A, 0x000D, 0x000D, 0x0020, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};
//struct char_ : pegtl8::ranges<0x0009, 0x0009, 0x0020, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};
//struct ws : pegtl::blank {}; // pegtl::one<' ', '\t'>
//
//
//
////struct continuation : pegtl::seq<pegtl::one<'\\'>, pegtl::star<ws>, pegtl::eol> {};
////
////struct first_prefix_text : pegtl::star<pegtl::seq<pegtl::not_at <continuation>, char_>> {};
////struct first_prefix : pegtl::seq<first_prefix_text, continuation> {};
////
////struct text : pegtl::star<pegtl::seq<pegtl::not_at<pegtl::sor<continuation, pegtl::eolf>>, char_>> {};
////struct line_with_continuation : continuation {};
////struct line_without_continuation : pegtl::seq<pegtl::eol, pegtl::not_at<pegtl::one<';'>>> {};
////struct final_line : pegtl::not_at<sc_text_delim_close> {}; // pegtl::eof {};
////struct prefix_line : pegtl::seq<pegtl::bol, prefix_text, text, pegtl::sor<line_with_continuation, line_without_continuation, final_line>> {};
////
////struct prefix_text_content : pegtl::seq<first_prefix, pegtl::star<prefix_line>> {};
//
//
//
//
//
//
//
//struct fsc_prefix_text
//{
//	using rule_t = fsc_prefix_text;
//
//	template< pegtl::apply_mode,
//		pegtl::rewind_mode,
//		template< typename... >
//	class Action,
//		template< typename... >
//	class Control,
//		typename ParseInput,
//		typename... States >
//	static bool match(ParseInput& in, std::string& /*unused*/, const std::string& prefix)
//	{
//		if (in.size(prefix.size()) >= prefix.size())
//		{
//			if (std::memcmp(in.current(), prefix.data(), prefix.size()) == 0)
//			{
//				in.bump(prefix.size());
//				return true;
//			}
//		}
//		return false;
//	}
//};
//
////text block
//struct sc_text_delim_open : pegtl::seq<pegtl::bol, pegtl::one<';'>> {};
//struct sc_text_delim_close : pegtl::seq<pegtl::eol, pegtl::one<';'>> {};
//struct sc_text_content : pegtl::star<pegtl::not_at<sc_text_delim_close>, allchars> {};
//
//
//
//struct continuation : pegtl::seq<pegtl::one<'\\'>, pegtl::star<ws>, pegtl::eol> {};
//
//struct first_prefix_text : pegtl::star<pegtl::seq<pegtl::not_at <continuation>, char_>> {}; //TAO_PEGTL_STRING("prefix:") {};  
//struct first_prefix : pegtl::seq<first_prefix_text, continuation> {};
//
//struct text : pegtl::star<pegtl::seq<pegtl::not_at<pegtl::sor<sc_text_delim_close, continuation, pegtl::eol>>, char_>> {};
//struct line_with_continuation : continuation {};
//struct line_without_continuation : pegtl::eol {};
//struct final_line : pegtl::at<sc_text_delim_close> {};
//struct prefix_line : pegtl::seq<pegtl::bol, fsc_prefix_text, text, pegtl::sor<final_line, line_with_continuation, line_without_continuation>> {};
//
//struct prefix_text_content : pegtl::seq<first_prefix, pegtl::star<prefix_line>> {};
//
//
//
//
//
//struct text_field : pegtl::if_must<sc_text_delim_open, pegtl::sor<prefix_text_content, sc_text_content>, sc_text_delim_close> {};
//
//
//
//
//
//
//
//template<typename Rule>
//struct action : pegtl::nothing<Rule> {};
//
//template<> struct action<first_prefix_text>
//{
//	template<typename Input> static void apply(const Input& in, std::string& /*unused*/, std::string& prefix)
//	{
//		prefix = in.string();
//		//std::cout << "first_prefix_text: |" << in.string() << "|\n";
//	}
//};
//
//template<> struct action<first_prefix>
//{
//	template<typename Input> static void apply(const Input& in, std::string& /*unused*/, std::string& prefix)
//	{
//		//std::cout << "first_prefix: |" << in.string() << "|\n";
//	}
//};
//
//template<> struct action<text>
//{
//	template<typename Input> static void apply(const Input& in, std::string& out, const std::string& /*unused*/)
//	{
//		out += in.string();
//		//std::cout << "text: |" << in.string() << "|\n";
//	}
//};
//
//template<> struct action<line_with_continuation>
//{
//	template<typename Input> static void apply(const Input& in, std::string& out, const std::string& /*unused*/)
//	{
//		//std::cout << "line_with_continuation: |" << in.string() << "|\n";
//	}
//};
//
//template<> struct action<line_without_continuation>
//{
//	template<typename Input> static void apply(const Input& in, std::string& out, const std::string& /*unused*/)
//	{
//		out += '\n';
//		//std::cout << "line_without_continuation: |" << in.string() << "|\n";
//	}
//};
//
//template<> struct action<final_line>
//{
//	template<typename Input> static void apply(const Input& in, std::string& out, const std::string& /*unused*/)
//	{
//		//std::cout << "final_line: |" << in.string() << "|\n";
//	}
//};
//
//
//template<> struct action<prefix_text_content>
//{
//	template<typename Input> static void apply(const Input& in, const std::string& out, const std::string& /*unused*/)
//	{
//		//std::cout << "prefix_text_content: |" << in.string() << "|\n";
//	}
//};
//
//template<> struct action<sc_text_content>
//{
//	template<typename Input> static void apply(const Input& in, std::string& out, const std::string& /*unused*/)
//	{
//		out = in.string();
//		//std::cout << "sc_text_content: |" << in.string() << "|\n";
//	}
//};
//
//template<> struct action<text_field>
//{
//	template<typename Input> static void apply(const Input& in, const std::string& out, const std::string& /*unused*/)
//	{
//		//std::cout << "text_field: |" << in.string() << "|\n";
//
//		std::cout << "The result is: \n|" << out << "|\n";
//	}
//};

int main() {
	

//	std::string str{
//		R"(;prefix:\
//prefix:This is a short line.
//prefix:This is a long\
//prefix:Spaces.
//;)"
//	};
	std::string str{
		R"(;prefix:\
prefix:This is a short line.
prefix:This is a long\
prefix: line and should all \
prefix:be on one line.
prefix:There are some
prefix:lines without
prefix:continuation markers.
prefix:\
prefix:\
prefix:
prefix: This, and the next line, \
prefix:start with spaces.
prefix: Here are \ embedded \'s to confuse
prefix:And\
prefix:Others\
prefix:With\    
prefix:No\
prefix:Spaces.
;)"
	};

		std::string result {
R"(This is a short line.
This is a long line and should all be on one line.
There are some
lines without
continuation markers.

 This, and the next line, start with spaces.
 Here are \ embedded \'s to confuse
AndOthersWithNoSpaces.)"
		};



//#define TRACER
//#define ANALYZE


#ifdef ANALYZE
		const std::size_t issues = tao::pegtl::analyze<text_field>(1);
		std::cout << issues << '\n';
#endif

		pegtl::string_input<pegtl::tracking_mode::eager, pegtl::eol::lf_crlf> in(str, "string");
		try
		{
			row::cif::Cif cif{};
			row::cif::states::Buffer buffer{};

#ifdef TRACER
			tao::pegtl::tracer<tao::pegtl::tracer_traits<true, true, true>> tr(in);
			tr.parse< row::cif::rules::text_field, row::cif::actions::action, tao::pegtl::normal>(in, cif, buffer);
			std::cout << "\n###\n";
#endif

			tao::pegtl::parse< row::cif::rules::text_field, row::cif::actions::action >(in, cif, buffer);

			std::cout << buffer.content << '\n';
			std::cout << (buffer.content == result) << '\n';
			
		}
		catch (pegtl::parse_error& e)
		{
			const auto p = e.positions().front();
			//pretty-print the error msg and the line that caused it, with an indicator at the token that done it.
			std::cerr << e.what() << '\n'
				<< in.line_at(p) << '\n'
				<< std::setw(p.column) << '^' << std::endl;
			//throw std::runtime_error("Parsing error -> " + std::string(e.what()));
			return 1;
		}

	

	return 0;





	//row::cif::Cif cif{};
	//cif.set_version("2.0");
	//row::cif::Block blk = cif.add_block("block1");

	//blk.add_dataitem("_tag1", "hello");
	//blk.add_dataitem("_tag2", "there");
	//blk.add_dataitem("_tag3", "world");
	//blk.add_dataitem("_tag3", "three");



	//std::vector<std::string> looptags {"_tagl4", "_tagl5", "_tagl6"};
	//std::vector< tao::json::value> loopvals {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

	//blk.add_loop_dataitems(looptags, loopvals);



	//std::cout << tao::json::to_string(cif.file, 3) << '\n';


	//return 0;

//	const std::size_t issues = tao::pegtl::analyze< row::cif::rules::CIF2_file >(1);
//	std::cout << issues << '\n';
//
//	//row::cif::Block blk("blkname");
//
//	//std::vector<tao::json::value> tmp;
//
//	//blk.add_dataitem("_tag1", "v1");
//	//blk.add_dataitem("_tag2", "v2");
//	//blk.add_dataitem("_tag3", "v3");
//	//blk.add_dataitem("_tag4", "v4");
//	//blk.add_dataitem("_tag5", "v5");
//	//blk.add_dataitem("_tag6", "v6");
//	//blk.add_dataitem("_tag7", "v7");
//	//std::cout << std::setw(2) << blk.blk << std::endl;
//	//blk.create_loop({ "_tag3", "_tag5", "_tag7" });
//
//	//std::cout << std::setw(2) << blk.blk << std::endl;
//
//
//	std::string str{ R"(
//;\
//This is a long\
// line and should all\
// be on one line.
//
//some lines
//don't have
//continuation characters
//
//There\
//Are\
//No\
//Spaces\
//Here.
//;)" };
//
//
//
//	row::cif::read_string(str);
//	tao::pegtl::string_input in(str, "string");
//







//#\#CIF_2.0   
//	data_name
//		_tag


	//const std::filesystem::path test_data_folder{ R"(C:\Users\184277j\Documents\GitHub\pdqciflib\pdqciflib\data\test_data)" };
	//for (auto const& filename : std::filesystem::directory_iterator{ test_data_folder })
	//{
	//	if (!std::filesystem::is_regular_file(filename) || filename.path().extension() != ".cif")
	//		continue;

	//	std::cout << filename << '\n';
	//	try
	//	{
	//		row::cif::read_file(filename.path().string());
	//	}
	//	catch(const std::runtime_error& e)
	//	{
	//		std::cerr << e.what() << '\n';
	//	}

	//	std::cout << "Start trace\n";
	//	tao::pegtl::file_input<tao::pegtl::tracking_mode::eager, tao::pegtl::eol::lf_crlf> in(filename);
	//try
	//{
	//	row::cif::states::Buffer buffer{in.source()};
	//	tao::pegtl::tracer< tao::pegtl::tracer_traits<true, true, true> > tr(in);
	//	tr.parse< row::cif::rules::CIF2_file, row::cif::actions::action, tao::pegtl::normal>(in, buffer);
	//}
	//catch (const tao::pegtl::parse_error& e)
	//{
	//	const auto p = e.positions().front();
	//	std::cerr << e.what() << '\n'
	//		<< in.line_at(p) << '\n'
	//		<< std::setw(p.column) << '^' << std::endl;
	//	return 1;
	//}
	//std::cout << "End trace\n";
	//}
	//





	//row::cif::read_file(R"(C:\Users\184277j\Documents\GitHub\pdqciflib\pdqciflib\data\test_data)");


	//row::cif::Cif cif1 = row::cif::read_string(str);

	//std::cout << '|' << cif1.get("d").get("_tag")[0] << '|';


	//std::cout << "\n-------\n" << cif1.to_string();

	//row::cif::Cif cif2 = row::cif::read_string(cif1.to_string());

	//std::cout << "\n-------\n" << cif2.to_string();

	return 0;
}


#else 



int main()
{





}

#endif


