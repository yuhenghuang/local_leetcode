/**
 * @file 06_ndim_vector_of_pointers.cpp
 * 
 * @copyright Copyright (c) 2021 - 2023, Yuheng Huang <kongqiota@gmail.com>
 * 
 * 04_void_return.cpp is part of library local leetcode, 
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

#define _LL_DELETE_RETURN_POINTER
#include <local_leetcode.hpp>

class Solution {
  public:
    // create a new linked list that connects all lists in the 2D vector
    ListNode* connectLists(vector<vector<ListNode*>>& lists) {
      ListNode dummy;
      ListNode* p = &dummy;

      for (auto& row : lists)
        for (auto q : row) {
          while (q != nullptr) {
            p->next = new ListNode(q->val);

            p = p->next;
            q = q->next;
          }
        }

      return dummy.next;
    }
};


int main() {
  EXECS(Solution::connectLists);
  return 0;
}
