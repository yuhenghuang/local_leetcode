/**
 * @file local_leetcode.hpp
 * 
 * @copyright Copyright (c) 2021 - 2023, Yuheng Huang <kongqiota@gmail.com>
 * 
 * local_leetcode.hpp is part of library local leetcode, 
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


#ifndef _LL_LOCAL_LEETCODE_HPP
#define _LL_LOCAL_LEETCODE_HPP

// define input file's directory and extension
// #define _LL_INPUT_DIR "Inputs"
// #define _LL_INPUT_EXT "txt"

// #define _LL_PRINT_ARRAY_MAX_LENGTH 100

#include <array>
#include <set>
#include <climits>
#include <numeric>
#include <stack>
#include <random>

#include "data_structure/data_structure.hpp"
#include "misc/utility.hpp"
#include "misc/traits.hpp"
#include "io/io.hpp"
#include "core/core.hpp"


// undefine internal macros
#undef _LL_FRIEND_PROPERTY


// expose funtion / data structure
using ll::rand7;

using ll::TreeNode;
using ll::ListNode;

using ll::NestedInteger;
using ll::Sea;

using ll::Iterator;

// create alias of various nodes as Node
#ifdef TERNARY_TREE_NODE
  using Node = ll::TernaryTreeNode;
#endif
#ifdef RANDOM_TREE_NODE
  using Node = ll::RandomTreeNode;
  using NodeCopy = ll::RandomTreeNode;
#endif
#ifdef NARY_TREE_NODE
  using Node = ll::NaryTreeNode;
#endif
#ifdef NARY_GRAPH_NODE
  using Node = ll::NaryGraphNode;
#endif
#ifdef QUAD_NODE
  using Node = ll::QuadNode;
#endif
#ifdef CIRCULAR_LIST_NODE
  using Node = ll::CircularListNode;
#endif
#ifdef RANDOM_LIST_NODE
  using Node = ll::RandomListNode;
#endif
#ifdef TERNARY_LIST_NODE
  using Node = ll::TernaryListNode;
#endif


using namespace std;

#endif // _LL_LOCAL_LEETCODE_HPP
