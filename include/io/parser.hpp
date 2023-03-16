/**
 * @file parser.hpp
 * 
 * @copyright Copyright (c) 2021 - 2023, Yuheng Huang <kongqiota@gmail.com>
 * 
 * parser.hpp is part of library local leetcode, 
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


#ifndef _LL_PARSER_HPP
#define _LL_PARSER_HPP

#include "../misc/utility.hpp"
#include <algorithm>
#include <regex>
#include <charconv>

namespace ll {

/**
 * @brief Parse std::string to given type
 * 
 * @tparam Tp target type
 */
template <typename Tp> 
struct universal_parser {
  /**
   * @brief parser functor, works for integers
   * 
   * @param sv input string view
   * @return Tp output type
   */
  typename std::enable_if<std::is_integral<Tp>::value, Tp>::type 
  operator()(std::string_view sv);
};

template <> struct universal_parser<char> { char operator()(std::string_view) const; };
template <> struct universal_parser<std::string> { std::string operator()(std::string_view) const; };

template <> struct universal_parser<bool> { bool operator()(std::string_view) const; };

/* signed integers are now parsed by template (not specialization)
template <> struct universal_parser<int> { int operator()(std::string_view) const; };
template <> struct universal_parser<long> { long operator()(std::string_view) const; };
template <> struct universal_parser<long long> { long long operator()(std::string_view) const; };
template <> struct universal_parser<uint32_t> { uint32_t operator()(std::string_view) const; };
*/
template <> struct universal_parser<double> { double operator()(std::string_view) const; };

template <> struct universal_parser<TreeNode*> { TreeNode* operator()(std::string_view) const; };
template <> struct universal_parser<TernaryTreeNode*> { TernaryTreeNode* operator()(std::string_view) const; };
template <> struct universal_parser<RandomTreeNode*> { RandomTreeNode* operator()(std::string_view) const; };
template <> struct universal_parser<QuadNode*> { QuadNode* operator()(std::string_view) const; };
template <> struct universal_parser<NaryTreeNode*> { NaryTreeNode* operator()(std::string_view) const; };

template <> struct universal_parser<NaryGraphNode*> { NaryGraphNode* operator()(std::string_view) const; };

template <> struct universal_parser<ListNode*> { ListNode* operator()(std::string_view) const; };
template <> struct universal_parser<RandomListNode*> { RandomListNode* operator()(std::string_view) const; };
template <> struct universal_parser<CircularListNode*> { CircularListNode* operator()(std::string_view) const; };
template <> struct universal_parser<TernaryListNode*> { TernaryListNode* operator()(std::string_view) const; };

template <> struct universal_parser<NestedInteger> { NestedInteger operator()(std::string_view) const; };
template <> struct universal_parser<Sea> { Sea operator()(std::string_view) const; };

template <typename Tp> struct universal_parser<std::vector<Tp>> { std::vector<Tp> operator()(std::string_view) const; };


namespace internal {

// https://en.cppreference.com/w/cpp/utility/from_chars
template <class Tp>
void from_chars_integral(const char* first, const char* last, Tp& t) {
  // base of unsigned integer is 2
  auto [ptr, ec] = std::from_chars(first, last, t, std::is_signed<Tp>::value ? 10 : 2);

  if (ec == std::errc::invalid_argument)
    throw std::invalid_argument("invalid_argument");
  else if (ec == std::errc::result_out_of_range) 
    throw std::out_of_range("result_out_of_range");
}

} // namespace internal


// integers
template <typename Tp>
typename std::enable_if<std::is_integral<Tp>::value, Tp>::type 
universal_parser<Tp>::operator()(std::string_view sv) {
  Tp res;

  internal::from_chars_integral(sv.begin(), sv.end(), res);
  return res;
}


// vector
template <typename Tp>
std::vector<Tp>
universal_parser<std::vector<Tp>>::operator()(std::string_view sv) const {
  std::vector<Tp> vec;

  universal_parser<Tp> parser;

  param_iterator<Tp> param(sv);
  std::string_view val;
  while (param.get_next(val))
    vec.push_back(parser(val));

  return vec;
}


// _LL_IMPLEMENTATION


} // namespace ll
#endif // _LL_PARSER_HPP
