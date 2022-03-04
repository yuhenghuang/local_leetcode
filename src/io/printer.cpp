/**
 * @file printer.cpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * printer.cpp is part of library local leetcode, 
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


#include "io/printer.hpp"

namespace ll {
namespace internal {

// find left most nodes of a tree in each level
void 
find_left_most_nodes(TernaryTreeNode* root, 
                     size_t depth, 
                     std::vector<TernaryTreeNode*>& nodes) 
{
  if (!root)
    return;
  else if (depth == nodes.size())
    nodes.push_back(root);

  find_left_most_nodes(root->left, depth + 1, nodes);
  find_left_most_nodes(root->right, depth + 1, nodes);
}


// traverse the graph to obtain node <-> index mapping
void 
nary_graph_traverse(NaryGraphNode* v, 
                    NaryGraphNode* (&nodes)[101],
                    std::unordered_map<NaryGraphNode*, int>& m) 
{
  if (v != nullptr && nodes[v->val - 1] == nullptr) {
    nodes[v->val - 1] = v;
    m[v] = v->val;

    for (auto& w : v->neighbors)
      nary_graph_traverse(w, nodes, m);
  }
}

// print singly linked list nodes
template <typename SinglyListNode>
void
print_linked_list(SinglyListNode* head) {
  std::cout << '[';

  if (head) {
    while (head->next) {
      std::cout << head->val << ", ";
      head = head->next;
    }
    std::cout << head->val;
  }
  
  std::cout << ']';
}

} // end of internal


// ---------------------
// implementations below


void
universal_printer<char>::operator()(const char res) {
  std::cout << '\'' << res << '\'';
}

void
universal_printer<std::string>::operator()(const std::string& res) {
  std::cout << '\"' << res << '\"';
}


void 
universal_printer<bool>::operator()(const bool res) {
  std::cout << std::boolalpha << res;
}


void
universal_printer<TreeNode*>::operator()(TreeNode* root) {
  std::cout << '[';

  // count of non-null nodes in current and next level
  int m, n = 0;

  std::queue<TreeNode*> q;
  if (root) {
    std::cout << root->val;

    if (root->left) ++n;
    if (root->right) ++n;

    q.push(root->left);
    q.push(root->right);
  }


  while (!q.empty()) {
    m = n;
    n = 0;

    int N = q.size();
    while (N--) {
      root = q.front();
      q.pop();

      if (root) {
        --m;

        std::cout << ", " << root->val;

        if (root->left) ++n;
        if (root->right) ++n;

        q.push(root->left);
        q.push(root->right);
      }
      else if (m > 0 || n > 0)
        std::cout << ", null";
    }

  }

  std::cout << ']';
}


void
universal_printer<TernaryTreeNode*>::operator()(TernaryTreeNode* root) {
  // left most nodes in each level
  std::vector<TernaryTreeNode*> nodes;

  internal::find_left_most_nodes(root, 0, nodes);

  std::cout << '[';

  for (size_t i = 0; i < nodes.size(); ++i) {
    TernaryTreeNode* node = nodes[i];
    while (node) {
      std::cout << node->val << ", ";
      node = node->next;
    }

    std::cout << ((i + 1 < nodes.size()) ? "#, " : "#");
  }

  std::cout << ']';
}


void
universal_printer<QuadNode*>::operator()(QuadNode* root) {
  std::cout << '[';

  // count of non-null nodes in current and next level
  int m, n = 0;

  std::queue<QuadNode*> q;
  if (root) {
    std::cout << "[" << root->isLeaf \
      << ", " << root->val << ']';

    if (root->topLeft) ++n;
    if (root->topRight) ++n;
    if (root->bottomLeft) ++n;
    if (root->bottomRight) ++n;

    q.push(root->topLeft);
    q.push(root->topRight);
    q.push(root->bottomLeft);
    q.push(root->bottomRight);
  }

  while (!q.empty()) {
    m = n;
    n = 0;

    int N = q.size();
    while (N--) {
      root = q.front();
      q.pop();

      if (root) {
        --m;

        std::cout << ", [" << root->isLeaf \
          << ", " << root->val << ']';

        if (root->topLeft) ++n;
        if (root->topRight) ++n;
        if (root->bottomLeft) ++n;
        if (root->bottomRight) ++n;

        q.push(root->topLeft);
        q.push(root->topRight);
        q.push(root->bottomLeft);
        q.push(root->bottomRight);
      }
      else if (m > 0 || n > 0)
        std::cout << ", null";
    }
  }

  std::cout << ']';
}


void
universal_printer<NaryTreeNode*>::operator()(NaryTreeNode* root) {
  std::cout << '[';

  // count of non-null nodes in current and next level
  int m, n = 0;

  std::queue<NaryTreeNode*> q;
  if (root) {
    std::cout << root->val;

    if (!root->children.empty())
      std::cout << ", null";

    for (NaryTreeNode*& child : root->children) {
      std::cout << ", " << child->val;

      q.push(child);
      n += child->children.size();
    }

    if (n > 0)
      std::cout << ", null";
  }

  while (!q.empty()) {
    m = n;
    n = 0;

    int N = q.size();
    while (N--) {
      root = q.front();
      q.pop();

      --m;

      for (NaryTreeNode*& child : root->children) {
        std::cout << ", " << child->val;

        q.push(child);
        n += child->children.size();
      }

      if (m > 0 || n > 0)
        std::cout << ", null";
    }
  }

  std::cout << ']';
}


void
universal_printer<NaryGraphNode*>::operator()(NaryGraphNode* node) {
  NaryGraphNode* nodes[101] = {0};
  std::unordered_map<NaryGraphNode*, int> m;

  internal::nary_graph_traverse(node, nodes, m);

  std::cout << '[';

  for (int i = 0; i < 100; ++i) {
    if (nodes[i] != nullptr) {
      std::cout << '[';

      std::vector<NaryGraphNode*>& neighbors = nodes[i]->neighbors;
      for (int j = 0; j < neighbors.size(); ++j) {
        std::cout << m[neighbors[j]];

        if (j + 1 < neighbors.size())
          std::cout << ", ";
      }

      std::cout << ']';
      if (nodes[i+1] != nullptr)
        std::cout << ", ";
    }
    else
      break;
  }

  std::cout << ']';
}


void
universal_printer<ListNode*>::operator()(ListNode* head) {
  internal::print_linked_list(head);
}


void
universal_printer<RandomListNode*>::operator()(RandomListNode* head) {
  // node to index;
  std::unordered_map<RandomListNode*, int> m;
  m[nullptr] = -1;

  RandomListNode* p = head;
  for (int i = 0; p != nullptr; p = p->next, ++i) 
    m[p] = i;

  std::cout << '[';

  while (head) {
    std::cout << '[' << head->val << ", ";

    int i = m[head->random];
    if (i == -1)
      std::cout << "null";
    else
      std::cout << i;

    std::cout << ']';

    if (head->next)
      std::cout << ", ";

    head = head->next;
  }

  std::cout << ']';
}


void
universal_printer<CircularListNode*>::operator()(CircularListNode* head) {
  std::cout << '[';

  if (head) {
    std::cout << head->val;

    CircularListNode* p = head->next;
    while (p != head) {
      std::cout << ", " << p->val;
      p = p->next;
    }
  }

  std::cout << ']';
}


void
universal_printer<TernaryListNode*>::operator()(TernaryListNode* head) {
  internal::print_linked_list(head);
}


void
universal_printer<NestedInteger>::operator()(const NestedInteger& ni) {
  if (ni.isInteger()) {
    std::cout << ni.getInteger();
  }
  else {
    std::cout << '[';

    auto iter = ni.getList().begin();
    (*this)(*iter++);

    auto end = ni.getList().end();
    for (; iter != end; ++iter) {
      std::cout << ", ";
      (*this)(*iter);
    }

    std::cout << ']';
  }
}


} // end of ll
