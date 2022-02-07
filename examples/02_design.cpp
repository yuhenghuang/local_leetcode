/**
 * @file 02_design.cpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * 02_design.cpp is part of library local leetcode, 
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

class Design {
  private:
    const vector<int>& vec;

  public:
    Design(vector<int>& vec): vec(vec) { }

    int at(int i) {
      return vec[i];
    }

    int random() {
      return vec[rand() % vec.size()];
    }
};


int main() {
  EXECX(
    CTOR(vector<int>&),
    &Design::at,
    &Design::random
  );

  return 0;
}
