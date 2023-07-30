


#include <iostream>
#include <string>
#include  <filesystem>


#define MINE

#ifdef MINE
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/trace.hpp>
//#include "pdqciflib.hpp"
#include "tao/pegtl.hpp"

namespace pegtl = tao::pegtl;
namespace pegtl8 = tao::pegtl::utf8;

template<typename Rule>
struct action_fsctf : pegtl::nothing<Rule> {};


//character sets
//almost all of unicode, as per CIF2.0 grammar
struct allchars : pegtl8::ranges<0x0009, 0x0009, 0x000A, 0x000A, 0x000D, 0x000D, 0x0020, 0x007E, 0x00A0, 0xD7FF, 0xE000, 0xFDCF, 0xFDF0, 0xFFFD, 0x10000, 0x1FFFD, 0x20000, 0x2FFFD, 0x30000, 0x3FFFD, 0x40000, 0x4FFFD, 0x50000, 0x5FFFD, 0x60000, 0x6FFFD, 0x70000, 0x7FFFD, 0x80000, 0x8FFFD, 0x90000, 0x9FFFD, 0xA0000, 0xAFFFD, 0xB0000, 0xBFFFD, 0xC0000, 0xCFFFD, 0xD0000, 0xDFFFD, 0xE0000, 0xEFFFD, 0xF0000, 0xFFFFD, 0x100000, 0x10FFFD> {};

struct ws : pegtl::blank {}; // pegtl::one<' ', '\t'>


//rules for folded semicolon textfields
struct f_text_continuation : pegtl::seq<pegtl::one<'\\'>, pegtl::star<ws>, pegtl::eol> {};


struct fsctf_text : pegtl::plus<pegtl::not_at<f_text_continuation>, allchars> {};
struct fsctf_line_without_continuation : fsctf_text {};
struct fsctf_line_with_continuation : pegtl::seq<fsctf_text, f_text_continuation> {};
struct fsctf_grammar : pegtl::seq<pegtl::star<pegtl::plus<fsctf_line_with_continuation>, pegtl::star<fsctf_line_without_continuation>>, pegtl::sor<fsctf_line_without_continuation, pegtl::eof>> {};


template<> struct action_fsctf<fsctf_text>
{
	template<typename Input> static void apply(const Input& in, [[maybe_unused]] std::string& out)
	{
		out += in.string();
		std::cout << "fsctf_text: |" << in.string() << "|\n";
	}
};

template<> struct action_fsctf<fsctf_line_without_continuation>
{
	template<typename Input> static void apply(const Input& in, [[maybe_unused]] std::string& out)
	{
		std::cout << "fsctf_line_without_continuation: |" << in.string() << "|\n";
	}
};

template<> struct action_fsctf<fsctf_line_with_continuation>
{
	template<typename Input> static void apply(const Input& in, [[maybe_unused]] std::string& out)
	{
		std::cout << "fsctf_line_with_continuation: |" << in.string() << "|\n";
	}
};

template<> struct action_fsctf<fsctf_grammar>
{
	template<typename Input> static void apply(const Input& in, [[maybe_unused]] std::string& out)
	{
		std::cout << "fsctf_grammar: |" << in.string() << "|\n";

		std::cout << "The result is: \n|" << out << "|\n";
	}
};


int main() {
	{

		std::string str{ R"(This is a short line.
This is a long\
 line and should all \                         
be on one line.)" };


		const std::size_t issues = tao::pegtl::analyze<fsctf_grammar>(1);
		std::cout << issues << '\n';

		pegtl::string_input<pegtl::tracking_mode::eager, pegtl::eol::lf_crlf> in(str, "string");
		try
		{
			std::string out{};
			std::cout << "Parsing...\n";
			//tao::pegtl::tracer<tao::pegtl::tracer_traits<true, true, true>> tr(in);
			//tr.parse< fsctf_grammar, action_fsctf, tao::pegtl::normal>(in, out);

			tao::pegtl::parse< fsctf_grammar, action_fsctf >(in, out);
			std::cout << "Parsed.\n";
			
		}
		catch (pegtl::parse_error& e)
		{
			const auto p = e.positions().front();
			//pretty-print the error msg and the line that caused it, with an indicator at the token that done it.
			std::cerr << e.what() << '\n'
				<< in.line_at(p) << '\n'
				<< std::setw(p.column) << '^' << std::endl;
			throw std::runtime_error("Parsing error -> " + std::string(e.what()));
		}

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


