/**
 * @file printer.hpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * printer.hpp is part of library local leetcode, 
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


#ifndef _LL_PRINTER_HPP
#define _LL_PRINTER_HPP

#include "../misc/utility.hpp"
#include <queue>

namespace ll {

/**
 * @brief print Tp to std::cout in a desirable format
 * 
 * @tparam Tp type
 * @tparam Rank rank, detected by rank<Tp> by default
 */
template <typename Tp, size_t Rank = rank<Tp>::value> 
struct universal_printer {
  // flag to help only print info once
  inline static bool flag = true;

  /**
   * @brief neither operator<< nor specialization printer is implemented
   * 
   * @return void
   */
  template <typename Up = Tp>
  typename std::enable_if<!is_printable<Up>::value>::type
  operator()(const Tp&) {
    if (flag) {
      std::cout << "no available printer for the type" << std::endl;
      flag = false;
    }
  }

  /**
   * @brief print Tp via operator<<
   * 
   * @return void
   */
  template <typename Up = Tp>
  typename std::enable_if<is_printable<Up>::value>::type
  operator()(const Tp& res) {
    std::cout << res;
  }
};


// characters
template <> struct universal_printer<char> { void operator()(const char); };
template <> struct universal_printer<std::string> { void operator()(const std::string&); };

// boolean
template <> struct universal_printer<bool> { void operator()(const bool); };

// double
template <> struct universal_printer<double> { void operator()(const double); };

// tree
template <> struct universal_printer<TreeNode*> { void operator()(TreeNode*); };
template <> struct universal_printer<TernaryTreeNode*> { void operator()(TernaryTreeNode*); };
template <> struct universal_printer<RandomTreeNode*> { void operator()(RandomTreeNode*); };
template <> struct universal_printer<QuadNode*> { void operator()(QuadNode*); };
template <> struct universal_printer<NaryTreeNode*> { void operator()(NaryTreeNode*); };

// graph
template <> struct universal_printer<NaryGraphNode*> { void operator()(NaryGraphNode*); };

// list
template <> struct universal_printer<ListNode*> { void operator()(ListNode*); };
template <> struct universal_printer<RandomListNode*> { void operator()(RandomListNode*); };
template <> struct universal_printer<CircularListNode*> { void operator()(CircularListNode*); };
template <> struct universal_printer<TernaryListNode*> { void operator()(TernaryListNode*); };


// nested integer
template <> struct universal_printer<NestedInteger> { void operator()(const NestedInteger&); };

// vector
template <typename Tp> struct universal_printer<std::vector<Tp>, 1UL> { void operator()(const std::vector<Tp>&); };
template <typename Tp> struct universal_printer<std::vector<Tp>, 2UL> { void operator()(const std::vector<Tp>&); };


// 1D vector
template <typename Tp>
void
universal_printer<std::vector<Tp>, 1UL>::operator()(const std::vector<Tp>& res) {
  size_t n = res.size();
  universal_printer<Tp> print;

  std::cout << '[';
  for (size_t i = 0; i < n; ++i) {

#ifdef _LL_PRINT_ARRAY_MAX_LENGTH
    if (i == _LL_PRINT_ARRAY_MAX_LENGTH) {
      std::cout << "..." << n - i << " more elements";
      break;
    }
#endif

    print(res[i]);

    if (i < n - 1)
      std::cout << ", ";
  }
  std::cout << ']';
}


template <typename Tp>
void
universal_printer<std::vector<Tp>, 2UL>::operator()(const std::vector<Tp>& res) {
  size_t n = res.size();
  universal_printer<Tp> print;

  std::cout << '[';
  for (size_t i = 0; i < n; ++i) {
    
#ifndef _LL_FLATTEN_2D
    if (i > 0) std::cout << ' ';
#endif

#ifdef _LL_PRINT_ARRAY_MAX_LENGTH
    if (i == _LL_PRINT_ARRAY_MAX_LENGTH) {
      std::cout << "..." << n - i << " more elements";
      break;
    }
#endif

    print(res[i]);
    
    if (i < n - 1)
#ifndef _LL_FLATTEN_2D
      std::cout << ',' << std::endl;
#else
      std::cout << ", ";
#endif

  }
  std::cout << ']';
}


// _LL_IMPLEMENTATION


} // end of ll
#endif // end of _LL_PRINTER_HPP