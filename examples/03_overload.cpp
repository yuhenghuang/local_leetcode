/**
 * @file 03_overload.cpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * 03_overload.cpp is part of library local leetcode, 
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


#include <local_leetcode.hpp>

class Solution {
  public:
    int findNodeIndex(ListNode* head, ListNode* node) {
      int i = 0;
      while (head != node) {
        head = head->next;
        ++i;
      }

      return i;
    }

    // real input is linked list and the value of the node
    int findNodeIndex(ListNode* head, int val) {
      // use helper function to find the node
      // for more helper functions, please check misc/utility.hpp
      return findNodeIndex(
        head,
        ll::find_node(head, val)
      );
    }
};


int main() {
  EXECS(Solution::findNodeIndex, int, (ListNode*, int));
  return 0;
}
