/**
 * @file data_structure.hpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * data_structure.hpp is part of library local leetcode, 
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


#ifndef _LL_DATA_STRUCTURE_HPP
#define _LL_DATA_STRUCTURE_HPP


#include <vector>

namespace ll {

// binary tree node
struct TreeNode;

// binary tree node with an extra edge to its neighbor
struct TernaryTreeNode;

// n-ary tree node (single root)
struct NaryTreeNode;

// binary tree node with an extra edge to random node in the ree
struct RandomTreeNode;

// n-ary graph (no root, undirected)
struct NaryGraphNode;

// quad tree node (one node with at most 4 children)
struct QuadNode;


// linked list node
struct ListNode;

// linked list node with an extra edge to random node
struct RandomListNode;

// circular linked list node whoes tail connected to its head
struct CircularListNode;

// doubly linked list with an extra edge to the head of next level
struct TernaryListNode;

// nested integer, be either an integer or a list of integer
class NestedInteger;

// java-like iterator over vector<int> with methods of next() and hasNext()
class Iterator;

// sea where ships are located at integer points represented by a cartesian plane, and each integer point may contain at most 1 ship.
class Sea;


// --------------------
// implementation below


struct TreeNode {
  int val;

  TreeNode *left;
  TreeNode *right;

  TreeNode() : val(0), left(nullptr), right(nullptr) {}

  TreeNode(int x): val(x), left(nullptr), right(nullptr) {}

  TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};


struct TernaryTreeNode {
  int val;

  TernaryTreeNode* left;
  TernaryTreeNode* right;
  TernaryTreeNode* next;

  TernaryTreeNode() : val(0), left(nullptr), right(nullptr), next(nullptr) {}

  TernaryTreeNode(int _val) : val(_val), left(nullptr), right(nullptr), next(nullptr) {}

  TernaryTreeNode(int _val, TernaryTreeNode* _left, TernaryTreeNode* _right, TernaryTreeNode* _next)
    : val(_val), left(_left), right(_right), next(_next) {}
};


struct RandomTreeNode {
  int val;

  RandomTreeNode* left;
  RandomTreeNode* right;
  RandomTreeNode* random;

  RandomTreeNode() : val(0), left(nullptr), right(nullptr), random(nullptr) {}

  RandomTreeNode(int x) : val(x), left(nullptr), right(nullptr), random(nullptr) {}

  RandomTreeNode(int x, RandomTreeNode* left, RandomTreeNode* right, RandomTreeNode* random) : val(x), left(left), right(right), random(random) {}
};


struct NaryTreeNode {
  int val;

  std::vector<NaryTreeNode*> children;

  NaryTreeNode() {}

  NaryTreeNode(int _val): val(_val) { }

  NaryTreeNode(int _val, std::vector<NaryTreeNode*>& nodes): val(_val), children(nodes) { }
};


struct NaryGraphNode {
  int val;

  std::vector<NaryGraphNode*> neighbors;

  NaryGraphNode() {}

  NaryGraphNode(int _val): val(_val) { }

  NaryGraphNode(int _val, std::vector<NaryGraphNode*>& nodes): val(_val), neighbors(nodes) { }
};


struct QuadNode {
  bool val;
  bool isLeaf;

  QuadNode* topLeft;
  QuadNode* topRight;
  QuadNode* bottomLeft;
  QuadNode* bottomRight;
  
  QuadNode() {
    val = false;
    isLeaf = false;
    topLeft = NULL;
    topRight = NULL;
    bottomLeft = NULL;
    bottomRight = NULL;
  }
  
  QuadNode(bool _val, bool _isLeaf) {
    val = _val;
    isLeaf = _isLeaf;
    topLeft = NULL;
    topRight = NULL;
    bottomLeft = NULL;
    bottomRight = NULL;
  }
  
  QuadNode(bool _val, bool _isLeaf, QuadNode* _topLeft, QuadNode* _topRight, QuadNode* _bottomLeft, QuadNode* _bottomRight) {
    val = _val;
    isLeaf = _isLeaf;
    topLeft = _topLeft;
    topRight = _topRight;
    bottomLeft = _bottomLeft;
    bottomRight = _bottomRight;
  }
};


struct ListNode {
  int val;

  ListNode *next;

  ListNode() : val(0), next(nullptr) {}

  ListNode(int x) : val(x), next(nullptr) {}

  ListNode(int x, ListNode *next) : val(x), next(next) {}
};


struct RandomListNode {
  int val;

  RandomListNode* next;
  RandomListNode* random;
  
  RandomListNode(int _val = 0): val(_val), next(nullptr), random(nullptr) { }
};


struct CircularListNode {
  int val;
  CircularListNode* next;
  
  CircularListNode() {}

  CircularListNode(int _val): val(_val), next(nullptr) {}

  CircularListNode(int _val, CircularListNode* _next): val(_val), next(_next) { }
};


struct TernaryListNode {
  int val;

  TernaryListNode* prev;
  TernaryListNode* next;
  TernaryListNode* child;

  TernaryListNode():
    val(0), prev(nullptr), next(nullptr), child(nullptr)
  { }
};


class NestedInteger {
  private:
    bool is_integer;
    int val;
    std::vector<NestedInteger> data;

  public:
    // Constructor initializes an empty nested list.
    NestedInteger();

    // Constructor initializes a single integer.
    NestedInteger(int value);

    // user-defined constructors and assignments, unofficial
    NestedInteger(const NestedInteger&) = default;
    NestedInteger& operator=(const NestedInteger&) = default;

    NestedInteger(NestedInteger&&) noexcept;
    NestedInteger& operator=(NestedInteger&&) noexcept;

    // Return true if this NestedInteger holds a single integer, rather than a nested list.
    bool isInteger() const;

    // Return the single integer that this NestedInteger holds, if it holds a single integer
    // The result is undefined if this NestedInteger holds a nested list
    int getInteger() const;

    // Set this NestedInteger to hold a single integer.
    void setInteger(int value);

    // Set this NestedInteger to hold a nested list and adds a nested integer to it.
    void add(const NestedInteger &ni);

    // Return the nested list that this NestedInteger holds, if it holds a nested list
    // The result is undefined if this NestedInteger holds a single integer
    const std::vector<NestedInteger> &getList() const;
};


class Iterator {
    struct Data;
    Data* data;
  public:
    Iterator(const std::vector<int>& nums);
    Iterator(const Iterator& iter);
    ~Iterator();

    // Returns the next element in the iteration.
    int next();

    // Returns true if the iteration has more elements.
    bool hasNext() const;
};


class Sea {
  private:
    std::vector<std::vector<int>> prefix;

  public:
    Sea();
    Sea(const std::vector<std::vector<int>>& ships);

    // move construction and assignment
    
    Sea(Sea&&) noexcept;
    Sea& operator==(Sea&&) noexcept;

    // has ships in the rectangle represented by two points, including on the boundary.
    bool hasShips(std::vector<int> topRight, std::vector<int> bottomLeft);
};


// _LL_IMPLEMENTATION


} // end of ll
#endif // end of _LL_DATA_STRUCTURE_HPP
