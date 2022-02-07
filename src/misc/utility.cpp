/**
 * @file utility.cpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * utility.cpp is part of library local leetcode, 
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


#include "misc/utility.hpp"

namespace ll {
namespace internal {

/**
 * @brief skip ' ' and ',' in string s
 * 
 * @param s string
 * @param i index
 */
inline 
void 
skip_delimiters(const std::string& s, size_t& i) {
  while (i < s.size() && ((s[i] == ' ') || (s[i] == ',')))
    ++i;
}


// find node in tree
TreeNode* 
find_node_in_arbitrary_tree(TreeNode* root, int val) {
  if (!root)
    return nullptr;
  else if (root->val == val)
    return root;

  TreeNode* node = find_node_in_arbitrary_tree(root->left, val);

  return node ? node : find_node_in_arbitrary_tree(root->right, val);
}


TreeNode* 
find_node_in_bst(TreeNode* root, int val) {
  if (!root)
    return nullptr;

  if (root->val > val)
    return find_node_in_bst(root->left, val);
  else if (root->val < val)
    return find_node_in_bst(root->right, val);
  else
    return root;
}


} // end of internal


// nested integer
param_range
find_param_range<NestedInteger>::operator()(const std::string& s, size_t i) {
  internal::skip_delimiters(s, i);

  // whether it's a list or an integer
  return s[i] == '[' ? find_param_range<std::vector<int>>()(s, i) : find_param_range<int>()(s, i);
};


std::string 
path_to_input_file(const char* dir, 
                   const std::string& input_ext, 
                   const std::string& cpp_path) 
{

#ifdef _WIN32
  #define _LL_SEPERATOR '\\'
#else
  #define _LL_SEPERATOR '/'
#endif

  std::string path = dir;

  // as windows supports '/' in many compilers, the seperator is determined actually by user input...
  if (!path.empty() && (path.back() != '\\') && (path.back() != '/'))
    path.push_back(_LL_SEPERATOR);

  size_t i = std::min(cpp_path.find_last_of('/'), cpp_path.find_last_of('\\'));
  
  std::string cpp_file = i == std::string::npos ? cpp_path : cpp_path.substr(i + 1);

  path += cpp_file.substr(0, cpp_file.find_last_of('.') + 1);
  
  return path + input_ext;

#undef _LL_SEPERATOR
}


// inline
TreeNode* 
find_node(TreeNode* root, int val, bool is_bst) {
  return is_bst ? internal::find_node_in_bst(root, val) : internal::find_node_in_arbitrary_tree(root, val);
}


ListNode* 
find_node(ListNode* head, int val) {
  while (head && head->val != val)
    head = head->next;

  return head;
}

bool 
is_same(TreeNode* root1, TreeNode* root2) {
  if (!root1 && !root2)
    return true;
  else if (root1 && root2)
    return root1->val == root2->val && is_same(root1->left, root2->left) && is_same(root1->right, root2->right);
  else
    return false;
}


bool
is_same(ListNode* head1, ListNode* head2) {
  for(; head1 && head2; head1 = head1->next, head2 = head2->next)
    if (head1->val != head2->val)
      return false;

  // both nullptr
  return head1 == head2;
}


int
find_index(ListNode* head, ListNode* node) {
  int i = 0;
  while (head && head != node) {
    head = head->next;
    ++i;
  }

  return head ? i : -1;
}

ListNode* 
create_cycle(ListNode* head, int pos) {
  if (pos >= 0) {
    ListNode* p = head;
    for (int i = 0; i < pos; ++i)
      p = p->next;

    ListNode* q = p;
    while (q->next)
      q = q->next;

    q->next = p;
  }

  return head;
}

inline 
void 
intersect_linked_lists_impl(ListNode* headA, ListNode* headB, int skipA, int skipB) {
  for (int i = 0; i < skipA; ++i)
    headA = headA->next;

  for (int i = 1; i < skipB; ++i)
    headB = headB->next;

  headB->next = headA;
}

void 
intersect_linked_lists(ListNode* headA, ListNode* headB, int skipA, int skipB) {
  if (nullptr == headA || nullptr == headB)
    return;

  assert((skipA > 0 || skipB > 0));

  if (skipB == 0)
    intersect_linked_lists_impl(headB, headA, skipB, skipA);
  else
    intersect_linked_lists_impl(headA, headB, skipA, skipB);
}


} // end of ll
