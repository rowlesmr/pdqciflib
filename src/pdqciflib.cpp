// pdqciflib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

import util;
import ciffile;
import cifparse;

using namespace row::cif;

int main()
{



    std::string file{ "C:\\Users\\184277j\\Documents\\GitHub\\pdqciflib\\pdqciflib\\data\\data.txt" };

    try {
        Cif cif = read_file(file);

        cif.print_cif();
        cif.print_block_order();
        cif.print_true_case();

        cif.getLastBlock().print_block();
        cif.getLastBlock().print_loops();
        cif.getLastBlock().print_item_order();
        cif.getLastBlock().print_true_case();
        std::cout  << "\n--\n\n--\n\n--\n\n--\n\n--\n\n--\n";
        cif.getLastBlock().print();

        //Datavalue dv{};
        //cif.getLastBlock().get("_numeric", dv);

        //std::vector<double> dbl = dv.get_dbls();

        //std::cout << dbl.size() << "\n--\n";
        //for (const auto& d : dbl) {
        //    std::cout << d << '\n';
        //}


    }
    catch (std::runtime_error& e) {
        std::cout << "caught\n";
    }



    std::cout << "Hello World!\n";
    //std::cout << sizeof(char) << "\n";
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
