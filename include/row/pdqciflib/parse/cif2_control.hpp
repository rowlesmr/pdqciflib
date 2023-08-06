
#pragma once


#include <iostream>
#include <stdexcept>
#include "tao/pegtl.hpp"

#include "cif2_rules.hpp"


namespace row::cif::control
{

    namespace pegtl = tao::pegtl;
    namespace pegtl8 = tao::pegtl::utf8;
	namespace rules = row::cif::rules;
 
	//template< typename > inline constexpr const char* error_message = nullptr;
	//template<> inline constexpr auto error_message< rules::lst_end > = "Hello";


	//struct error
	//{
	//	template< typename Rule > static constexpr bool raise_on_failure = false;
	//	template< typename Rule > static constexpr auto message = error_message< Rule >;
	//};

	//template< typename Rule >
	//using control = pegtl::must_if< error >::control< Rule >;


}
