/**
 * @file data_structure.cpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * data_structure.cpp is part of library local leetcode, 
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


#include "data_structure/data_structure.hpp"

namespace ll {

// note: nested integer is implemented given only official public interfaces
NestedInteger::NestedInteger(): is_integer(false) { }

NestedInteger::NestedInteger(int value): is_integer(true), val(value) { }

bool 
NestedInteger::isInteger() const { 
  return is_integer;
}

int 
NestedInteger::getInteger() const { 
  return val; 
}

void 
NestedInteger::setInteger(int value) {
  val = value;
  if (!is_integer) {
    is_integer = true;
    data.clear();
  }
}

void 
NestedInteger::add(const NestedInteger &ni) {
  is_integer = false;
  data.push_back(ni);
}

const std::vector<NestedInteger>&
NestedInteger::getList() const { 
  return data; 
}


// embedding pointers to save space??
struct Iterator::Data {
  const int* iter;
  const int* end;

  Data(const int* _iter, const int* _end): iter(_iter), end(_end) { }
};

Iterator::Iterator(const std::vector<int>& nums) {
  data = new Data(nums.data(), nums.data() + nums.size());
}

Iterator::Iterator(const Iterator& iter) {
  data = new Data(iter.data->iter, iter.data->end);
}

Iterator::~Iterator() { delete data; }

int
Iterator::next() {
  return *data->iter++;
}

bool
Iterator::hasNext() const {
  return data->iter != data->end;
}

} // end of ll
