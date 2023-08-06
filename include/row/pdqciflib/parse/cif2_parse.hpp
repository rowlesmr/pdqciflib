#pragma once



#include <iostream>
#include <stdexcept>
#include "tao/pegtl.hpp"

#include "cif2_control.hpp"
#include "..\structure\structure.hpp"

#include "cif2_rules.hpp"
#include "cif2_actions.hpp"

namespace row::cif
{
    namespace pegtl = tao::pegtl;
    namespace pegtl8 = tao::pegtl::utf8;

    template<typename Input>
    void parse_input(Input&& in, bool printErr = true)
    {
        try
        {
            //Status status{};
            Cif cif{};
            states::Buffer buffer{in.source()};
			pegtl::parse < rules::CIF2_file, actions::action> (in, cif, buffer);

            std::cout << tao::json::to_string(cif.file, 3) << '\n';
            
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

    //read in a file into a Cif. Will throw std::runtime_error if it encounters problems
    inline /*Cif*/ void read_file(const std::string& filename, bool overwrite = false, bool printErr = true) noexcept(false)
    {
        pegtl::file_input<pegtl::tracking_mode::eager, pegtl::eol::lf_crlf> in(filename);
        return read_input(in, printErr);
    }

    //read a string into a Cif. Will throw std::runtime_error if it encounters problems
    inline /*Cif*/ void read_string(const std::string& cifstring, bool printErr = true, const std::string& source = "string") noexcept(false)
    {
        pegtl::string_input<pegtl::tracking_mode::eager, pegtl::eol::lf_crlf> in(cifstring, source);
        read_input(in, printErr);
    }

}

