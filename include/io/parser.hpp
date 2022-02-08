/**
 * @file parser.hpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * parser.hpp is part of library local leetcode, 
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


#ifndef _LL_PARSER_HPP
#define _LL_PARSER_HPP

#include "../misc/utility.hpp"
#include <algorithm>
#include <regex>

namespace ll {

/**
 * @brief Parse std::string to given type
 * 
 * @tparam Tp target type
 */
template <typename Tp> 
struct universal_parser {
  /**
   * @brief parser functor
   * 
   * @param s input string
   * @return Tp output type
   */
  Tp operator()(const std::string& s);
};

template <> struct universal_parser<char> { char operator()(const std::string&) const; };
template <> struct universal_parser<std::string> { std::string operator()(const std::string&) const; };

template <> struct universal_parser<int> { int operator()(const std::string&) const; };
template <> struct universal_parser<long> { long operator()(const std::string&) const; };
template <> struct universal_parser<long long> { long long operator()(const std::string&) const; };
template <> struct universal_parser<double> { double operator()(const std::string&) const; };
template <> struct universal_parser<uint32_t> { uint32_t operator()(const std::string&) const; };

template <> struct universal_parser<TreeNode*> { TreeNode* operator()(const std::string&) const; };
template <> struct universal_parser<TernaryTreeNode*> { TernaryTreeNode* operator()(const std::string&) const; };
template <> struct universal_parser<QuadNode*> { QuadNode* operator()(const std::string&) const; };
template <> struct universal_parser<NaryTreeNode*> { NaryTreeNode* operator()(const std::string&) const; };

template <> struct universal_parser<NaryGraphNode*> { NaryGraphNode* operator()(const std::string&) const; };

template <> struct universal_parser<ListNode*> { ListNode* operator()(const std::string&) const; };
template <> struct universal_parser<RandomListNode*> { RandomListNode* operator()(const std::string&) const; };
template <> struct universal_parser<CircularListNode*> { CircularListNode* operator()(const std::string&) const; };
template <> struct universal_parser<TernaryListNode*> { TernaryListNode* operator()(const std::string&) const; };

template <> struct universal_parser<NestedInteger> { NestedInteger operator()(const std::string&) const; };
template <> struct universal_parser<Sea> { Sea operator()(const std::string&) const; };

template <typename Tp> struct universal_parser<std::vector<Tp>> { std::vector<Tp> operator()(const std::string&) const; };


// vector
template <typename Tp>
std::vector<Tp>
universal_parser<std::vector<Tp>>::operator()(const std::string& s) const {
  std::vector<Tp> vec;

  universal_parser<Tp> parser;

  param_iterator<Tp> param(s);
  std::string val;
  while (param.get_next(val))
    vec.push_back(parser(val));

  return vec;
}


// _LL_IMPLEMENTATION


} // end of ll
#endif // end of _LL_PARSER_HPP
