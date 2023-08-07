/**
 * @file parser.cpp
 * 
 * @copyright Copyright (c) 2021 - 2023, Yuheng Huang <kongqiota@gmail.com>
 * 
 * parser.cpp is part of library local leetcode, 
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


#include "io/parser.hpp"
#include "core/classes.hpp"

namespace ll {
namespace internal {

// parse binary tree node and ternary tree node with one null edge
template <typename BinaryTreeNode>
BinaryTreeNode* parse_binary_tree_node(std::string_view sv) {
  param_iterator<int> param(sv);

  if (!param.has_next())
    return nullptr;

  // size of tree
  size_t n = std::count(sv.begin(), sv.end(), ',') + 1UL;

  // size of null node
  size_t u = std::count(sv.begin(), sv.end(), 'u');

  // only array new non-null nodes
  // prepare for further plan of memory management (smart pointer)
  BinaryTreeNode* root = new BinaryTreeNode[n - u];

  // iterator over non-null nodes
  BinaryTreeNode* node = root;

  // array of null and non-null nodes in the order of input
  BinaryTreeNode* nodes[n];

  // iterator over nodes
  BinaryTreeNode** iter;

  universal_parser<int> parser;

  std::string_view val;
  for (iter = &nodes[0]; param.get_next(val); ++iter) {
    if (val != "null") {
      node->val = parser(val);
      *iter = node++;
    }
    else
      *iter = nullptr;
  }

  iter = &nodes[0];
  bool left = true;
  for (size_t i = 1; i < n; ++i) {
    // this loop is always valid because it runs slower than `i`
    while (*iter == nullptr)
      ++iter;

    if (left)
      (*iter)->left = nodes[i];
    else 
      (*iter++)->right = nodes[i];

    left = !left;
  }

  return root;
}

} // end of internal


// ---------------------
// implementations below

// boolean
bool 
universal_parser<bool>::operator()(std::string_view sv) const {
  if (sv == "true")
    return true;
  else if (sv == "false")
    return false;
  else
    throw std::runtime_error("invalid bool input!");
}

// characters
// inline
char
universal_parser<char>::operator()(std::string_view sv) const {
  // skip '\'' at index 0
  return sv[1];
}

// inline
std::string
universal_parser<std::string>::operator()(std::string_view sv) const {
  // skip '"'s on the sides
  // use explicit ctor
  return std::string(sv.substr(1, sv.size() - 2));
}


// inline
double
universal_parser<double>::operator()(std::string_view sv) const {
  // not implemented in clang/gcc yet, only implemented in MSVC
  // double res;
  // internal::from_chars_throws(sv.data(), sv.data() + sv.size(), res);
  return std::stod(std::string(sv)); 
}


// tree / graph
// inline
TreeNode*
universal_parser<TreeNode*>::operator()(std::string_view sv) const {
  return internal::parse_binary_tree_node<TreeNode>(sv);
}


// inline
TernaryTreeNode*
universal_parser<TernaryTreeNode*>::operator()(std::string_view sv) const {
  return internal::parse_binary_tree_node<TernaryTreeNode>(sv);
}


RandomTreeNode*
universal_parser<RandomTreeNode*>::operator()(std::string_view sv) const {
  // null random pointer to -1
  std::string line;
  
  regex_replace(
    std::back_inserter(line),
    sv.begin(), sv.end(),
    std::regex("null"), "-1"
  );

  // use nested integer to save parsed vector
  NestedInteger ni = universal_parser<NestedInteger>()(line);

  // size of tree
  size_t n = ni.getList().size();

  if (n == 0)
    return nullptr;

  // size of null nodes
  size_t u = 0;
  const std::vector<NestedInteger>& ls = ni.getList();

  for (const NestedInteger& node : ls)
    if (node.isInteger())
      ++u;

  RandomTreeNode* root = new RandomTreeNode[n - u];

  RandomTreeNode* nodes[n];

  RandomTreeNode* node = root;

  for (size_t i = 0; i < n; ++i) {
    if (ls[i].isInteger())
      nodes[i] = nullptr;
    else {
      node->val = ls[i].getList().front().getInteger();
      nodes[i] = node++;
    }
  }

  // random index
  int ri = ls.front().getList().back().getInteger();

  if (ri >= 0)
    root->random = nodes[ri];

  RandomTreeNode** iter = &nodes[0];
  bool left = true;
  for (size_t i = 1; i < n; ++i) {
    // set random pointer
    if (nodes[i] != nullptr) {
      ri = ls[i].getList().back().getInteger();

      if (ri >= 0)
        nodes[i]->random = nodes[ri];
    }

    // this loop is always valid because iter runs slower than `i`
    while (*iter == nullptr)
      ++iter;

    if (left)
      (*iter)->left = nodes[i];
    else 
      (*iter++)->right = nodes[i];

    left = !left;
  }

  return root;
}


QuadNode*
universal_parser<QuadNode*>::operator()(std::string_view sv) const {
  // size of null node
  size_t u = std::count(sv.begin(), sv.end(), 'u');
  
  // such that it can be parsed to 2D vector
  std::string line;
  
  regex_replace(
    std::back_inserter(line),
    sv.begin(), sv.end(),
    std::regex("null"), "[-1,-1]"
  );

  // must be declared after the following specialization of 2D vector
  auto&& mat = universal_parser<std::vector<std::vector<int>>>()(line);

  size_t n = mat.size();

  if (n == 0)
    return nullptr;

  QuadNode* root = new QuadNode[n - u];
  QuadNode* nodes[n];

  QuadNode* node = root;
  QuadNode** iter = &nodes[0];
  for (const std::vector<int>& p : mat) {
    if (p[0] >= 0) {
      node->isLeaf = p[0];
      node->val = p[1];

      *iter = node++;
    }
    else
      *iter = nullptr;

    ++iter;
  }


  iter = &nodes[0];
  for (int i = 1; i < n; ++i) {
    while (*iter == nullptr)
      ++iter;

    switch ((i + 3) % 4) {
      case 0:
        (*iter)->topLeft = nodes[i]; break;
      case 1:
        (*iter)->topRight = nodes[i]; break;
      case 2:
        (*iter)->bottomLeft = nodes[i]; break;
      case 3:
        (*iter++)->bottomRight = nodes[i]; break;
      default:
        abort();
    }
  }
  
  return root;
}


NaryTreeNode*
universal_parser<NaryTreeNode*>::operator()(std::string_view sv) const {
  param_iterator<int> param(sv);

  if (!param.has_next())
    return nullptr;

  // size of tree
  size_t n = std::count(sv.begin(), sv.end(), ',') + 1UL;

  // size of null node
  size_t u = std::count(sv.begin(), sv.end(), 'u');

  // only array new non-null nodes
  // prepare for further plan of memory management (smart pointer)
  // works even after the tree is dismantled...
  NaryTreeNode* root = new NaryTreeNode[n - u];

  // iterator over non-null nodes
  NaryTreeNode* node = root;

  // array of null and non-null nodes in the order of input
  NaryTreeNode* nodes[n];

  // iterator over nodes
  NaryTreeNode** iter;

  universal_parser<int> parser;

  std::string_view val;
  for (iter = &nodes[0]; param.get_next(val); ++iter) {
    if (val != "null") {
      node->val = parser(val);
      *iter = node++;
    }
    else
      *iter = nullptr;
  }

  iter = &nodes[0];
  for (size_t i = 2; i < n; ++i) {
    if (nodes[i] == nullptr) 
      // move to next valid parent
      while (*(++iter) == nullptr);
    else 
      (*iter)->children.push_back(nodes[i]);
  }

  return root;
}


NaryGraphNode*
universal_parser<NaryGraphNode*>::operator()(std::string_view sv) const {
  auto&& mat = universal_parser<std::vector<std::vector<int>>>()(sv);

  size_t n = mat.size();
  
  if (n == 0)
    return nullptr;

  NaryGraphNode* node = new NaryGraphNode[n];
  for (int i = 0; i < n; ++i) {
    node[i].val = i + 1;
    for (int j : mat[i])
      node[i].neighbors.push_back(&node[j-1]);
  }

  return node;
}


// list
ListNode*
universal_parser<ListNode*>::operator()(std::string_view sv) const {
  param_iterator<int> param(sv);

  if (!param.has_next())
    return nullptr;

  // size of linked list
  size_t n = std::count(sv.begin(), sv.end(), ',') + 1UL;

  // array new to save memory
  // prepare for further plan of memory management (smart pointer)
  ListNode* head = new ListNode[n];

  ListNode* node = head;

  universal_parser<int> parser;

  std::string_view val;
  for (size_t i = 0; i < n; ++i, (void) ++node) {
    param.get_next(val);
    node->val = parser(val);

    if (i < n - 1) 
      node->next = node + 1;
  }

  return head;
}


RandomListNode*
universal_parser<RandomListNode*>::operator()(std::string_view sv) const {
  // null random pointer to -1
  std::string line;
  
  regex_replace(
    std::back_inserter(line),
    sv.begin(), sv.end(),
    std::regex("null"), "-1"
  );

  auto&& mat = universal_parser<std::vector<std::vector<int>>>()(line);

  size_t n = mat.size();

  if (n == 0)
    return nullptr;

  RandomListNode* head = new RandomListNode[n];
  for (size_t i = 0; i < n; ++i) {
    // value
    head[i].val = mat[i][0];

    // random edge
    if (mat[i][1] >= 0)
      head[i].random = &head[mat[i][1]];

    // next edge
    if (i + 1 < n)
      head[i].next = &head[i+1];
  }

  return head;
}


CircularListNode*
universal_parser<CircularListNode*>::operator()(std::string_view sv) const {
  param_iterator<int> param(sv);

  if (!param.has_next())
    return nullptr;

  // size of linked list
  size_t n = std::count(sv.begin(), sv.end(), ',') + 1UL;

  // array new to save memory
  // prepare for further plan of memory management (smart pointer)
  CircularListNode* head = new CircularListNode[n];

  CircularListNode* node = head;

  universal_parser<int> parser;

  std::string_view val;
  for (size_t i = 0; i < n; ++i, (void) ++node) {
    param.get_next(val);
    node->val = parser(val);

    if (i < n - 1) 
      node->next = node + 1;
    else
      // circulate
      node->next = head;
  }

  return head;
}


TernaryListNode*
universal_parser<TernaryListNode*>::operator()(std::string_view sv) const {
  param_iterator<int> param(sv);

  if (!param.has_next())
    return nullptr;

  // size of tree
  size_t n = std::count(sv.begin(), sv.end(), ',') + 1UL;

  // size of null node
  size_t u = std::count(sv.begin(), sv.end(), 'u');

  // only array new non-null nodes
  // prepare for further plan of memory management (smart pointer)
  // works even after the tree is dismantled...
  TernaryListNode* head = new TernaryListNode[n - u];

  TernaryListNode* nodes[n];
  
  std::string_view val;
  universal_parser<int> parser;

  // index in head;
  size_t j = 0;

  // index of previous level
  size_t k = 0;

  for (size_t i = 0; i < n; ++i) {
    param.get_next(val);
    if ("null" != val) {
      nodes[i] = &head[j++];

      nodes[i]->val = parser(val);

      // skip head node in first level
      if (i > 0) {
        // non-head nodes
        if (nullptr != nodes[i-1]) {
          nodes[i]->prev = nodes[i-1];
          nodes[i-1]->next = nodes[i];
        }
        // head nodes
        else {
          nodes[k]->child = nodes[i];
          k = i;
        }
      }
    }
    else {
      nodes[i] = nullptr;
      if (nullptr == nodes[i-1])
        ++k;
    }
  }

  return head;
}


// nested integer
NestedInteger
universal_parser<NestedInteger>::operator()(std::string_view sv) const {
  // integer
  if (sv.front() != '[')
    return NestedInteger(universal_parser<int>()(sv));

  NestedInteger ni;

  param_iterator<NestedInteger> param(sv);
  std::string_view val;
  while (param.get_next(val))
    ni.add((*this)(val));

  return ni;
}


// sea
Sea
universal_parser<Sea>::operator()(std::string_view sv) const {
  auto&& ships = universal_parser<std::vector<std::vector<int>>>()(sv);

  return Sea(ships);
}


// mountain array
MountainArray
universal_parser<MountainArray>::operator()(std::string_view sv) const {
  std::vector<int>&& arr = universal_parser<std::vector<int>>()(sv);

  return MountainArray(std::move(arr));
}


// guess functor
Guess
universal_parser<Guess>::operator()(std::string_view sv) const {
  int pick = universal_parser<int>()(sv);

  return Guess(pick);
}


// is bad version functor
IsBadVersion
universal_parser<IsBadVersion>::operator()(std::string_view sv) const {
  int bad = universal_parser<int>()(sv);

  return IsBadVersion(bad);
}


// master
Master
universal_parser<Master>::operator()(std::string_view sv) const {
  std::tuple<
    input_parameter<std::string&&>, 
    input_parameter<std::vector<std::string>&&>, 
    input_parameter<int>
  > inputs;

  internal::generate_params(inputs, sv.substr(1, sv.size() - 1), std::make_index_sequence<3UL>{});

  return Master(std::get<0>(inputs), std::get<1>(inputs), std::get<2>(inputs));
}


} // end of ll
