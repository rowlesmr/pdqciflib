// Copyright (c) 2019-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DISCARD_INPUT_ON_SUCCESS_HPP
#define TAO_PEGTL_DISCARD_INPUT_ON_SUCCESS_HPP

#include "apply_mode.hpp"
#include "match.hpp"
#include "nothing.hpp"
#include "rewind_mode.hpp"

namespace tao::pegtl
{
   struct discard_input_on_success
      : maybe_nothing
   {
      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         const bool result = tao::pegtl::match< Rule, A, M, Action, Control >( in, st... );
         if( result ) {
            in.discard();
         }
         return result;
      }
   };

}  // namespace tao::pegtl

#endif
