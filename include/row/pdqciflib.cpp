

//#include <iostream>
//#include <string>
#include  <filesystem>



#define _SILENCE_CXX20_CISO646_REMOVED_WARNING

#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/trace.hpp>
#include "pdqciflib.hpp"


int main() {


	const std::size_t issues = tao::pegtl::analyze< row::cif::rules::CIF2_file >(1);
	std::cout << issues << '\n';



//	std::string str{ R"(
//;sctf
//;
//)" };

	std::string str{ R"(#\#CIF_2.0   
data_name # comment with space

save_framename
_tagsf value
save_

_tag1 
;value1
;
_tag2
'val[ue2'

_tag3
 ;value3

_tag3 'hello "there", how are you?'
_tag4 "'hello' there world!"
_tag5 """hello
there
world
"""
_tag6 ''' "hi" ''there''
what 
are you
doping?'''

#save_frame
#_tag7
#;this is 
#a text block
#;
#save_
_tag8 [ list item three ]
_tag9 { "table": entry "table2":entry2 }

loop_
_tag10

_tag11
1 2 3 {"key":value}

_tag12 [ list { "table": entry "table2":{ "table": entry "table2":entry2 } } [ list item three ] ]


#comment for)" };

//#\#CIF_2.0   
//	data_name
//		_tag

	//tao::pegtl::string_input in(str, "string");

	const std::filesystem::path test_data_folder{ R"(C:\Users\184277j\Documents\GitHub\pdqciflib\pdqciflib\data\test_data)" };
	for (auto const& filename : std::filesystem::directory_iterator{ test_data_folder })
	{
		if (!std::filesystem::is_regular_file(filename) || filename.path().extension() != ".cif")
			continue;

		std::cout << filename << '\n';
		try
		{
			row::cif::read_file(filename.path().string());
		}
		catch(const std::runtime_error& e)
		{
			std::cerr << e.what() << '\n';
		}

		//std::cout << "Start trace\n";
		//tao::pegtl::file_input<tao::pegtl::tracking_mode::eager, tao::pegtl::eol::lf_crlf> in(filename);
		//try
		//{
		//	tao::pegtl::tracer< tao::pegtl::tracer_traits<true, true, true> > tr(in);
		//	tr.parse< row::cif::rules::CIF2_file, tao::pegtl::nothing, tao::pegtl::normal>(in);
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
	}
	//row::cif::read_string(str);

	//row::cif::read_file(R"(C:\Users\184277j\Documents\GitHub\pdqciflib\pdqciflib\data\test_data)");


	//row::cif::Cif cif1 = row::cif::read_string(str);

	//std::cout << '|' << cif1.get("d").get("_tag")[0] << '|';


	//std::cout << "\n-------\n" << cif1.to_string();

	//row::cif::Cif cif2 = row::cif::read_string(cif1.to_string());

	//std::cout << "\n-------\n" << cif2.to_string();

	return 0;
}
