/**
 * @file destroyer.hpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * destroyer.hpp is part of library local leetcode, 
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

// non-pointer
template<typename Tp>
struct universal_destroyer { void operator()(Tp&) { } };

// pointer
template <typename Tp>
struct universal_destroyer<Tp*> { void operator()(Tp* ptr) { destroy(ptr); } };

// vector of pointers
template <typename Tp>
struct universal_destroyer<std::vector<Tp>> {
  void operator()(std::vector<Tp>& vec) {
    universal_destroyer<Tp> destroyer;

    for (Tp& e : vec)
      destroyer(e);
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
  
  /* failed to debug the implementation...

  // for the purpose of coping with undirected cyclic graph
  // cut edges from neighbors to root
  // act as if the graph is directed
  for (Tp*& p : root->neighbors) {
    if (p != nullptr) {
      // use auto and std::begin (end) to handle three types of arrays
      auto iter = std::find(std::begin(p->neighbors), std::end(p->neighbors), root);
      if (iter != std::end(p->neighbors))
        *iter = nullptr;
      else {
        std::cerr << "input undirected (a)cyclic graph is not well-defined" << std::endl;
        abort();
      }
    }
  }

  for (Tp*& p : root->neighbors)
    destroy_impl(p, tag);

  delete root;
  */
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
