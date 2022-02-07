# C++ Library to Execute Leetcode Problems Locally

This repository contains a C++ header only library that helps to execute [Leetcode](https://leetcode.com/) problems in **one line**. 

header-only version will be available in the future.

### Quick Start

- Source file

```cpp
#include <local_leetcode.hpp>

/*
  you solution... e.g.
  
  class Solution {...};
*/

int main() {
  // for solution type
  EXECS(Solution::method_name);
  // for design type
  EXECX(CTOR(constructor_types, ...), &Solution::method1, &Solution::method2, ...);
}
```


- Input format

By default, the program will load the input in *Inputs/{source_name.txt}* in current working directory.

<sup>* the input directory and extension can be customized by defining specific MACROs. `_LL_INPUT_DIR` and `_LL_INPUT_EXT` are used for the directory and extension of input files respectively</sup>

Each line of the input file contains all the arguments to execute the {method} or {class} **once**. The seperator of arguments are `','`, and spaces are skipped also.

Following is the example of `int, vector<int>, TreeNode*` as input
```txt
1, [1, 2], [1, null, 2]
2, [3, 2], [5, null, 2, null, 3]
```

For design type, the format of each line is `["{method_names}", ...], [[{arguments}], ...]`.


- Compilation flags

```bash
clang++ -std=c++17 -stdlib=libc++ {source_name.cpp} -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -O2 -Ipath/to/local_leetcode/include -Lpath/to/local_leetcode/lib -o {output_executable} -llocal_leetcode
```

<sup>* To use `libc++` on non-Mac OS,`-stdlib=libc++` is added </sup>

<sup>* To suppress memory leak error, try setting environment variable `ASAN_OPTIONS=detect_leaks=0`</sup>


Check the directory *examples* for more granular cases

### Build

* Requirements
  * clang++11 or higher
  * libc++-dev libc++abi-dev (to use std provided by llvm)
  * build tool. `make` on linux or `ninja` (or alike) on windows
* Other details
  * to build examples, please add `-DBUILD_EXAMPLES=on` in cmake arguments
  * it's not recommended to install the library in the system paths because of its limited usage
  * please use `ln -s` or `mklink` to achieve similar goals.


#### Unix-like OS

The requirements should be almost (already) be met on most unix-like OS. Thus following 

```bash
mkdir build
cd build

cmake .. -DCMAKE_CXX_COMPILER=clang++  -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

#### Windows (msys2)

To avoid the standard library of `msvc`, which is not completely open source, it's better to install gnu/llvm on windows.

After testing several approaches on windows, *msys2* is recommended because of it's less likely to encounter unsolvable bugs.

As stated in the requirements, from *msys2*, the following libraries/tools are needed

- clang-toolchain (comes with clang targeted to gnu)
  - libraries should come with the toolchain
- ninja (or other build tool)

After other steps like setting `PATH`, one can now start building

```bat
mkdir build
cd build

:: please specify correct make target (e.g. Ninja)
cmake .. -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Special Cases

##### Node types

To enable correct `Node` for specific problem from various types, one needs to define macro before the header file.

For example, to add a tree node with four children, 

```cpp
#define QUAD_NODE
#include <local_leetcode>
```

For more definition of marcos, see the content of the header file, *include/local_leetcode.hpp*


##### Inconsistent input type

The macro `EXECS` supports overload.

The syntax is

```cpp
EXECS(Solution::method, return_type, (arg_types, ...));
```

##### non-trivial constructor of class `Solution`

The library internally distinguish *solution* and *design* types of problems by checking whether the class is trivially constructable.

This logic fails if `Solution` has an explicit constructor. 

To enforce the behavior of the library under the condition where explicit constructor in `Solution` in unavoidable, simply define

```cpp
#define _LL_NONTRIVIAL_SOLUTION_CTOR
#include <local_leetcode.hpp>
```

before including the library header file.

##### memory leaking of return pointer (optional)

This is an optional feature which release memories. The module is implemented by heavy meta-programming techniques and should cover most common data types (e.g. tree).

To enable it, 

```cpp
#define _LL_DELETE_RETURN_POINTER
#include <local_leetcode.hpp>
```

For more details of the supported data types of the feature, see the following section.

### `ll::destroy(void *)` (optional)

This function is used to release memories of the interim objects in the heap.

The original purpose of the function is to manage the memory perfectly by the user him/herself, while it came to light that the goal was impossible during the development and tests. The function remains specifically for someone who wants to manage the memory by oneself without setting `ASAN_OPTIONS=detect_leaks=0`.

Supported objects (`Node`)

- Single Node
- Linked list
  - linked list with `Node* next`
  - circular doubly-linked list is also supported (not circular singly-linked list)
- Tree
  - binary tree with `Node* left` and `Node* right`
  - quad-tree with `Node* topLeft`, `Node* topRight`, `Node* bottomLeft` and `Node* bottomLeft`
  - trie with `Node* children[N]` (fixed-size array), `std::array<Node*, N> children`, `std::map<Key, Node*> children`, `std::unordered_map<Key, Node*> children`
- Graph
  - directed graph (nary tree) with `std::vector<Node*> children`
  - undirected graph with `std::vector<Node*> neighbors`


