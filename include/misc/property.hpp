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

namespace ll {

/**
 * @brief property. read-only if 4th template parameter is omitted.
 *   The implementation is not perfect because one has to declare the 
 *   property as friend in Cp, and do other chores (e.g. in ctors or assignments)...
 * 
 * @tparam Tp type of the property
 * @tparam Cp type of the class
 * @tparam Getter member function pointer to getter
 * @tparam Setter member function pointer to setter. null pointer by default
 */
template <
  typename Tp, 
  typename Cp,
  Tp (Cp::*Getter)() const,
  Tp& (Cp::*Setter)(const Tp&) = nullptr
>
class property {
  private:
    typedef property<Tp, Cp, Getter, Setter> self;

    Cp* ptr;

  public:
    typedef Cp class_type;
    typedef Tp value_type;

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
    operator value_type() const { return (ptr->*Getter)(); }

    // set, undefined-behavior if the property is read-only (Setter is nullptr)
    value_type& operator=(const value_type& rhs) { 
      static_assert(Setter != nullptr, "trying to set the value of a read-only property");

      return (ptr->*Setter)(rhs); 
    }
};


#define _LL_FRIEND_PROPERTY \
template <typename Tp, typename Cp, Tp (Cp::*Getter)() const, Tp& (Cp::*Setter)(const Tp&)> friend class property;


} // end of ll
#endif // end of _LL_PROPERTY_HPP
