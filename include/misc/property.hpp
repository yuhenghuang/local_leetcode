/**
 * @file property.hpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * property.hpp is part of library local leetcode, 
 * a c++ library that parses inputs and execute solutions of programming problems
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


#ifndef _LL_PROPERTY_HPP
#define _LL_PROPERTY_HPP

#include <type_traits>
#include "traits.hpp"


namespace ll {

#if __cplusplus < 201703L

/**
 * @brief property. read-only if 4th template parameter is omitted.
 *   The implementation is not perfect because one has to declare the 
 *   property as friend in Cp, and do other chores (e.g. in ctors or assignments)...
 * 
 * @tparam Tp type of the property, be either value_type or const valut_type&
 * @tparam Cp type of the class
 * @tparam Getter member function pointer to getter
 * @tparam Setter member function pointer to setter. null pointer by default
 */
template <
  typename Tp, // either value_type or const valut_type&
  typename Cp,
  Tp (Cp::*Getter)() const,
  // ...Setter is supposed to return value_type& regardless of Tp. const Tp& is always const value_type&
  std::remove_const_t<std::remove_reference_t<Tp>>& (Cp::*Setter)(const Tp&) = nullptr 
>
class property {
  public:
    typedef Cp class_type;
    typedef std::remove_const_t<std::remove_reference_t<Tp>> value_type;

    static_assert(
      std::disjunction<std::is_same<Tp, value_type>, std::is_same<Tp, const value_type&>>::value, 
      "Getter must return either raw type or const reference type"
    );

  private:
    typedef property<Tp, Cp, Getter, Setter> self;

    Cp* ptr;

  public:
    property(class_type* _ptr): ptr(_ptr) { }

    property(const self&) = delete;

    self& operator=(const self& rhs) {
      // not read-only (compile time)
      if constexpr (nullptr != Setter) {
        // not from the same object (run time)
        if (ptr != rhs.ptr)
          (ptr->*Setter)( (rhs.ptr->*Getter)() );
      }

      return *this; 
    };

    // get
    operator Tp() const { return (ptr->*Getter)(); }

    // set, undefined-behavior if the property is read-only (Setter is nullptr)
    value_type& operator=(const value_type& rhs) { 
      static_assert(Setter != nullptr, "trying to set the value of a read-only property");

      return (ptr->*Setter)(rhs); 
    }
};


#define _LL_FRIEND_PROPERTY \
template <typename Tp, typename Cp, Tp (Cp::*Getter)() const, std::remove_const_t<std::remove_reference_t<Tp>>& (Cp::*Setter)(const Tp&)> friend class property;

#else

/**
 * @brief property. read-only if 2nd template parameter is omitted.
 *   The implementation under c++17 simplifies the template parameters.
 *   The validation of template parameters (member function pointers) is done by static_assert inside class
 * 
 * @tparam Getter member function pointer to getter
 * @tparam Setter member function pointer to setter. null pointer by default
 */
template <
  auto Getter,
  auto Setter = nullptr
>
class property {
  public:
    typedef typename fn_ptr_traits<decltype(Getter)>::class_type class_type;
    typedef typename fn_ptr_traits<decltype(Getter)>::return_type return_type;
    typedef std::remove_const_t<std::remove_reference_t<return_type>> value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;

    // assert Getter's type
    static_assert(
      std::is_same<decltype(Getter), return_type (class_type::*)() const>::value,
      "Getter must be the type of return_type (class_type::*)() const"
    );

    // assert Getter's return type
    static_assert(
      std::disjunction<
        std::is_same<return_type, value_type>, 
        std::is_same<return_type, const value_type&>
      >::value, 
      "Getter must return either raw type or const reference type"
    );

    // assert Setter's type
    static_assert(
      std::disjunction<
        std::is_same<decltype(Setter), nullptr_t>, 
        std::is_same<decltype(Setter), reference (class_type::*)(const_reference)>
      >::value, 
      "Setter must be compatible with Getter (either null or reference (class_type::*)(const reference))"
    );

  private:
    typedef property<Getter, Setter> self;

    class_type* const ptr;

  public:
    property(class_type* _ptr): ptr(_ptr) { }

    property(const self&) = delete;

    // need more test cases to perfect the logic in assignment
    self& operator=(const self& rhs) {
      // not read-only (compile time)
      if constexpr (nullptr != Setter) {
        // not from the same object (run time)
        if (ptr != rhs.ptr)
          (ptr->*Setter)( (rhs.ptr->*Getter)() );
      }

      return *this; 
    };

    // get
    operator return_type() const { return (ptr->*Getter)(); }

    // set, undefined-behavior if the property is read-only (Setter is nullptr)
    reference operator=(const_reference rhs) { 
      static_assert(Setter != nullptr, "trying to set the value of a read-only property");

      return (ptr->*Setter)(rhs); 
    }
};

#define _LL_FRIEND_PROPERTY template <auto Getter, auto Setter> friend class property;

#endif // end of property


} // end of ll
#endif // end of _LL_PROPERTY_HPP
