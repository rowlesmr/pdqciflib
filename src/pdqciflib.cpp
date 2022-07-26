// pdqciflib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>

import util;



int main()
{

    std::vector<std::string> vals{ "13", "12.3", "-23", "+12.3", 
        "13(4)", "23.4(5)", 
        "123.4e2", "123.4e+2", "123.4e-2", 
        "123.4e2(5)", "123.4e+2(6)", "123.4e-2(7)", 
        "ThisIsString", ".", "?", "e", "(12)",
        "-.3(2)", ".1"};

    for (const auto& s : vals) {
        auto [v, e] = row::util::stode(s);
        std::cout << s<<'\t'<<v << " " << e << '\n';
    }


    //auto [v, e] = row::util::stode("123.4e+2(6)", 11);
    //std::cout << v << " " << e << '\n';

    //auto [vv, ee] = row::util::stode("ThisIsString",12);
    //std::cout << vv << " " << ee << '\n';





    std::cout << "Hello World!\n";
    std::cout << sizeof(char) << "\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
