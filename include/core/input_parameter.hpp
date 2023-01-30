/**
 * @file input_parameter.hpp
 * 
 * @copyright Copyright (c) 2021 - 2023, Yuheng Huang <kongqiota@gmail.com>
 * 
 * input_parameter.hpp is part of library local leetcode, 
 * a c++ library that parses inputs and executes solutions of programming problems
 * 
 * Local leetcode is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Local leetcode is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */


#ifndef _LL_INPUT_PARAMETER_HPP
#define _LL_INPUT_PARAMETER_HPP


#include "../misc/traits.hpp"
#include "../io/parser.hpp"

namespace ll {

/**
 * @brief wrap up input parameters, works as smart pointer
 * 
 * @tparam Tp input type (possibly with const, reference, pointer)
 * @tparam IsVecofPtrs is input type a vector of pointers (default is declarad in misc/traits.hpp)
 */
template <
  typename Tp, 
  bool IsVecofPtrs// = is_vector_of_pointers<Tp>::value
>
class input_parameter;

// scalar or vector of scalars
template <typename Tp>
class input_parameter<Tp, false> {
  public:
    typedef typename std::remove_const<
      typename std::remove_reference<Tp>::type
    >::type type;

  private:
    typedef input_parameter<Tp, false> self;

    type par;

  public:
    input_parameter() = default;

    operator Tp() { return static_cast<Tp>(par); }

    template <
      typename Up,
      typename = typename std::enable_if<std::is_assignable<type&, Up&&>::value>::type
    >
    self& operator=(Up&& _par)
    noexcept(std::is_nothrow_assignable<type&, Up&&>::value) {
      par = std::forward<Up>(_par);

      return *this;
    }
};


// pointer
template <typename Tp>
class input_parameter<Tp*, false> {
  public:
    typedef Tp* type;

  private:
    typedef input_parameter<Tp*, false> self;

    std::unique_ptr<Tp[]> ptr;

  public:
    input_parameter() = default;

    operator Tp*() { return ptr.get(); }

    template <
      typename Up,
      typename = typename std::enable_if<std::is_assignable<type&, Up&&>::value>::type
    >
    self& operator=(Up&& _par)
    noexcept(std::is_nothrow_assignable<type&, Up&&>::value) {
      // make_unique doesn't work for array type
      // use explicit ctor
      ptr = std::unique_ptr<Tp[]>(std::forward<Up>(_par));

      return *this;
    }
};


// vector of pointers
template <typename Tp>
class input_parameter<Tp, true> {
  public:
    typedef typename std::remove_reference<Tp>::type::value_type pointer;
    typedef std::vector<pointer> type;

  private:
    typedef input_parameter<Tp, true> self;
    typedef typename std::remove_pointer<pointer>::type value_type;

    type par;

    std::vector<std::unique_ptr<value_type[]>> ptrs;

  public:
    input_parameter() = default;

    operator Tp() { return static_cast<Tp>(par); }

    template <
      typename Up,
      typename = typename std::enable_if<std::is_assignable<type&, Up&&>::value>::type
    >
    self& operator=(Up&& _par) 
    noexcept(std::is_nothrow_assignable<type&, Up&&>::value) {
      par = std::forward<Up>(_par);

      // resize performs better than clear
      ptrs.resize(par.size());

      transform(
        par.begin(), par.end(),
        ptrs.begin(),
        // make_unique doesn't work for array type
        // use explicit ctor
        [] (const pointer& ptr) { return std::unique_ptr<value_type[]>(ptr); }
      );

      return *this;
    }
};


} // end of ll
#endif // end of _LL_INPUT_PARAMETER_HPP
