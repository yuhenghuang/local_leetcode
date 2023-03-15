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
#include "../io/destroyer.hpp"

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
// n-dim vector supported now
template <typename Tp>
class input_parameter<Tp, true> {
  public:
    typedef typename std::remove_cv<
      typename std::remove_reference<Tp>::type
    >::type type;

  private:
    typedef input_parameter<Tp, true> self;
    typedef typename is_vector_of_pointers<Tp>::value_type value_type;

    type par;

    void clear() {
      // passing type n-dim vector of Tp[] and argument of Tp* to destroyer
      // indicates that Tp* was created by array new, thus should be destroyed by array delete.
      // why rank<type>::value needs to subtract 1?
      // because rank of pointer is 1 (not 0 because it's actually created by array new) by definition
      // see include/misc/traits.hpp:47
      universal_destroyer<typename n_rank_vector<value_type[], rank<type>::value - 1>::type>()(par);
    }

  public:
    input_parameter() = default;

    operator Tp() { return static_cast<Tp>(par); }

    template <
      typename Up,
      typename = typename std::enable_if<std::is_assignable<type&, Up&&>::value>::type
    >
    self& operator=(Up&& _par) 
    noexcept(std::is_nothrow_assignable<type&, Up&&>::value) {
      clear();

      par = std::forward<Up>(_par);

      return *this;
    }

    ~input_parameter() { clear(); }
};


} // end of ll
#endif // end of _LL_INPUT_PARAMETER_HPP
