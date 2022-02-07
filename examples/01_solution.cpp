/**
 * @file 01_solution.cpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * 01_solution.cpp is part of library local leetcode, 
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
    vector<int> solve(int n, string s, ListNode* head) {
      vector<int> res;

      while (head) {
        res.push_back(head->val);
        head = head->next;
      }

      res.push_back(n);
      res.push_back(stoi(s));

      return res;
    }
};

int main() {
  EXECS(Solution::solve);
}
