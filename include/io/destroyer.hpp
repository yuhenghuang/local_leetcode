/**
 * @file destroyer.hpp
 * 
 * @copyright Copyright (c) 2021 - 2023, Yuheng Huang <kongqiota@gmail.com>
 * 
 * destroyer.hpp is part of library local leetcode, 
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


#ifndef _LL_DESTROYER_HPP
#define _LL_DESTROYER_HPP

#include <queue>
#include <unordered_set>
#include "../misc/traits.hpp"

namespace ll {

/**
 * @brief helper function for releasing memories
 * 
 * @tparam Tp type
 * 
 * @param root Tp*, e.g. TreeNode*
 * 
 */
template <typename Tp> void destroy(Tp* root);

// non-pointer, do nothing
template<typename Tp>
struct universal_destroyer { void operator()(const Tp&) { } };

// raw pointer
template <typename Tp>
struct universal_destroyer<Tp*> { void operator()(Tp* ptr) { destroy(ptr); } };

// vector of pointers
template <typename Tp>
struct universal_destroyer<std::vector<Tp>> {

  // the purpose of this functor is to reduce rank
  // thus it allows Tp and Up to be different
  template <typename Up>
  void operator()(const std::vector<Up>& vec) {
    universal_destroyer<Tp> destroyer;

    for (const Up& e : vec)
      destroyer(e);
  }
};


// array, or pointer obtained by array new
template <typename Tp>
struct universal_destroyer<Tp[]> {
  // fixed size
  template <size_t N>
  void operator()(Tp (&arr)[N]) {
    universal_destroyer<Tp> destroyer;

    for (Tp& e : arr)
      destroyer(e);
  }

  // variable size
  void operator()(Tp (&arr)[], size_t n) {
    universal_destroyer<Tp> destroyer;

    for (size_t i = 0; i < n; ++i)
      destroyer(arr[i]);
  }

  // delete memory allocated by array new
  void operator()(Tp* ptr) {
    delete[] ptr;
  }
};


namespace internal {

// cut prev -> head link
template <typename Tp>
typename std::enable_if<has_prev<Tp*>::value>::type
cut_prev(Tp* head) {
  if (head->prev)
    head->prev->next = nullptr;
}

// do nothing
template <typename Tp>
typename std::enable_if<!has_prev<Tp*>::value>::type
cut_prev(Tp* ) { }


template <typename Tp>
inline
void 
destroy_impl(Tp* ptr, single_node_tag) { delete ptr; }


template <typename Tp>
void 
destroy_impl(Tp* head, linked_list_tag) {
  if (!head)
    return;

  cut_prev(head);

  while (head) {
    Tp* p = head->next;
    delete head;
    head = p;
  }
}


template <typename Tp>
void 
destroy_impl(Tp* root, binary_tree_tag tag) {
  if (!root)
    return;

  destroy_impl(root->left, tag);
  destroy_impl(root->right, tag);

  delete root;
}


template <typename Tp>
void 
destroy_impl(Tp* root, quad_tree_tag tag) {
  if (!root)
    return;

  destroy_impl(root->topLeft, tag);
  destroy_impl(root->topRight, tag);
  destroy_impl(root->bottomLeft, tag);
  destroy_impl(root->bottomRight, tag);

  delete root;
}


// general nary tree
template <typename Tp>
typename std::enable_if<
  is_array_of_pointers<decltype(std::declval<Tp*&>()->children)>::value
>::type
destroy_impl(Tp* root, nary_tree_tag tag) {
  if (!root)
    return;
  
  for (Tp*& p : root->children)
    destroy_impl(p, tag);

  delete root;
}


// trie implementation by map<key, node*> or unordered_map<key, node*>
template <typename Tp>
typename std::enable_if<
  !is_array_of_pointers<
    decltype(std::declval<Tp*&>()->children)
  >::value
>::type
destroy_impl(Tp* root, nary_tree_tag tag) {
  if (!root)
    return;
  
  for (auto& p : root->children)
    destroy_impl(p.second, tag);

  delete root;
}


// undirected graph
template <typename Tp>
void 
destroy_impl(Tp* root, nary_graph_tag tag) {
  if (!root)
    return;

  // reason for using queue...
  // don't want to write helper function to traverse...

  std::queue<Tp*> q;
  q.push(root);

  std::unordered_set<Tp*> s;
  s.insert(root);
  while (!q.empty()) {
    root = q.front();
    q.pop();

    for (Tp*& neighbor : root->neighbors)
      if (s.count(neighbor) == 0) {
        s.insert(neighbor);
        q.push(neighbor);
      }
  }

  for (Tp* const& ptr : s)
    delete ptr;
  

}


} // end of internal


template <typename Tp> 
inline
void 
destroy(Tp* root) {
  internal::destroy_impl(root, graph_category<Tp*>{});
}


} // end of ll
#endif // end of _LL_DESTROYER_HPP
