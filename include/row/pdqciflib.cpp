

//#include <iostream>
//#include <string>

#include "pdqciflib.hpp"

int main() {

	std::string str{ 
R"(data_d
_tag
;\
C:\foldername\filename
;
)"
};

	row::cif::Cif cif1 = row::cif::read_string(str);

	std::cout << '|' << cif1.get("d").get("_tag")[0] << '|';


	std::cout << "\n-------\n" << cif1.to_string();

	row::cif::Cif cif2 = row::cif::read_string(cif1.to_string());

	std::cout << "\n-------\n" << cif2.to_string();

	return 0;
}
