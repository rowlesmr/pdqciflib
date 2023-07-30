


#include <iostream>
#include <string>
#include  <filesystem>


#define MINE

#ifdef MINE
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/trace.hpp>
#include "pdqciflib.hpp"








int main() {

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

	const std::size_t issues = tao::pegtl::analyze< row::cif::rules::CIF2_file >(1);
	std::cout << issues << '\n';

	//row::cif::Block blk("blkname");

	//std::vector<tao::json::value> tmp;

	//blk.add_dataitem("_tag1", "v1");
	//blk.add_dataitem("_tag2", "v2");
	//blk.add_dataitem("_tag3", "v3");
	//blk.add_dataitem("_tag4", "v4");
	//blk.add_dataitem("_tag5", "v5");
	//blk.add_dataitem("_tag6", "v6");
	//blk.add_dataitem("_tag7", "v7");
	//std::cout << std::setw(2) << blk.blk << std::endl;
	//blk.create_loop({ "_tag3", "_tag5", "_tag7" });

	//std::cout << std::setw(2) << blk.blk << std::endl;


	std::string str{ R"(#\#CIF_2.0   
data_block1
_tag4 12.34(5)
_tag5  1 2 3 4
_tag6 hello
)" };



	row::cif::read_string(str);
	tao::pegtl::string_input in(str, "string");








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


