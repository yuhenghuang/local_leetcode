/**
 * @file utility.hpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * utility.hpp is part of library local leetcode, 
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


#ifndef _LL_UTILITY_HPP
#define _LL_UTILITY_HPP

#include "property.hpp"
#include "traits.hpp"
#include <assert.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-inline"

namespace ll {

/**
 * @brief contains start (i, inclusive) and end (j, exclusive) index of a parameter
 * 
 */
class param_range {
  private:
    _LL_FRIEND_PROPERTY

    typedef param_range self;
    
    size_t length() const { return j - i; }


    typedef property<size_t, self, &param_range::length> length_property;
    
  public:
    size_t i; // start index, inclusive
    size_t j; // end index, exclusive

    // size_t, length property
    length_property l;

    param_range(size_t _i = 0, size_t _j = 0): 
      i(_i), j(_j), l(this) 
    { }

    // don't copy properties from other objects
    param_range(const param_range& rhs):
      i(rhs.i), j(rhs.j), l(this) 
    { }
};


template <
  typename Tp, 
  bool IsScalar = (rank<Tp>::value == 0), 
  bool IsChar = is_character<Tp>::value
>
struct find_param_range {
  /**
   * @brief find the range of current parameter [i, j) on string s
   * 
   * @param s string to be parsed
   * @param i index, the end of previous parameter. 0 if no such parameter
   * @return param_range 
   */
  param_range operator()(const std::string& s, size_t i); 
};


// scalar
template <typename Tp>
struct find_param_range<Tp, true, false> { param_range operator()(const std::string& s, size_t i); };

// character
template <typename Tp>
struct find_param_range<Tp, true, true> { param_range operator()(const std::string& s, size_t i); };

// array
template <typename Tp>
struct find_param_range<Tp, false, false> { param_range operator()(const std::string& s, size_t i); };

// nested integer
template <>
struct find_param_range<NestedInteger> { param_range operator()(const std::string& s, size_t i); };


/**
 * @brief iterate over an array (string representation) of a certain type, "[...]"
 * 
 * @tparam Tp type of the parameter
 */
template <typename Tp>
class param_iterator {
  private:
    const std::string& s;
    param_range range;
    find_param_range<Tp> finder;

  public:
    param_iterator(const std::string& _s, size_t i = 1UL);

    /**
     * @brief verify if there exists next value
     * 
     * @return true if there is next string
     */
    bool has_next();

    /**
     * @brief get the next string representation of value
     * 
     * @param val string, output
     * 
     * @return bool, whether any value left
     */
    bool get_next(std::string& val);
};


/**
 * @brief generate path to input file given input file's directory, extension and cpp file's path
 * 
 * @param dir directory of input file
 * @param input_ext extention of input file
 * @param cpp_path paht of cpp file
 * @return std::string path to input file
 */
std::string 
path_to_input_file(const char* dir, 
                   const std::string& input_ext, 
                   const std::string& cpp_path);


/**
 * @brief find the node given value in a tree
 * 
 * @param root 
 * @param val value
 * @param is_bst if it's BST
 * @return TreeNode* node or nullptr if not found
 */
TreeNode* find_node(TreeNode* root, int val, bool is_bst = false);

/**
 * @brief find the node given in a linked list
 * 
 * @param head head of the linked list
 * @param val value
 * @return ListNode* node or nullptr if not found
 */
ListNode* find_node(ListNode* head, int val);


/**
 * @brief is tree 2 the same as tree 1 in terms of value?
 * 
 * @param root1 tree 1
 * @param root2 tree 2
 * @return bool
 */
bool is_same(TreeNode* root1, TreeNode* root2);

/**
 * @brief is linked list 2 the same as linked list 1 in terms of value?
 * 
 * @param root1 list 1
 * @param root2 list 2
 * @return bool
 */
bool is_same(ListNode* head1, ListNode* head2);

/**
 * @brief find the index of node in a linked list
 * 
 * @param head head of the linked list
 * @param node target node
 * @return int index (0-base). -1 if not found
 */
int find_index(ListNode* head, ListNode* node);

/**
 * @brief link the tail to the node at index pos (0-base)
 * 
 * @param head head of the linked list
 * @param pos index of the node to be linked by tail
 * @return head after cycle created
 */
ListNode* create_cycle(ListNode* head, int pos);

/**
 * @brief Intersect linked lists after skip first several.
 *   The implementation keeps nodes in linked list A as intersection if both skips are positive.
 *   Two skips shall not both be 0 (i.e. two lists are identical).
 * 
 * @param headA head of linked list A
 * @param headB head of linked list B
 * @param skipA num. of nodes skipped in A before intersection
 * @param skipB num. of nodes skipped in B before intersection
 */
void intersect_linked_lists(ListNode* headA, ListNode* headB, int skipA, int skipB);

namespace internal {

/**
 * @brief skip ' ' and ',' in string s
 * 
 * @param s string
 * @param i index
 */
inline
void 
skip_delimiters(const std::string& s, size_t& i);

} // end of internal


// scalar
template <typename Tp>
param_range
find_param_range<Tp, true, false>::operator()(const std::string& s, size_t i) {
  internal::skip_delimiters(s, i);

  size_t j = i;
  while (j < s.size() && s[j] != ' ' && s[j] != ',' && s[j] != ']')
    ++j;

  return {i, j};
};


// character
template <typename Tp>
param_range
find_param_range<Tp, true, true>::operator()(const std::string& s, size_t i) {
  internal::skip_delimiters(s, i);

  // move from left quote
  size_t j = i + 1;

  while (j < s.size() && s[j] != '\'' && s[j] != '"')
    ++j;
    
  return {i, j + 1};
}


// array
template <typename Tp>
param_range
find_param_range<Tp, false, false>::operator()(const std::string& s, size_t i) {
  internal::skip_delimiters(s, i);

  size_t j = i + 1;

  int count = 1;
  for (; j < s.size() && count > 0; ++j) {
    if (s[j] == '[')
      ++count;
    else if (s[j] == ']')
      --count;
  }
    
  return {i, j};
};


template <typename Tp>
param_iterator<Tp>::param_iterator(const std::string& _s, size_t i): s(_s) {
  range = finder(s, i);
}


template <typename Tp>
inline 
bool
param_iterator<Tp>::has_next() { 
  return s[range.i] != ']'; 
}


template <typename Tp>
bool
param_iterator<Tp>::get_next(std::string& val) {
  if (has_next()) {
    val = s.substr(range.i, range.l);

    range = finder(s, range.j);

    return true;
  }

  return false;
}


// _LL_IMPLEMENTATION


} // end of ll

#pragma GCC diagnostic pop // pop ignored "-Wundefined-inline"

#endif // end of _LL_UTILITY_HPP