# A C++ Library to Execute Leetcode Problems Locally

This repository contains a C++ library that helps to execute [Leetcode](https://leetcode.com/) problems in **one line**. 

header-only version will be available in the future.

## Quick Start

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

By default, the program will load the input in *Inputs/{source_name.txt}* in current working directory. And the structure of the working directory should be 

```
.
├── 01_solution.cpp
├── 02_design.cpp
├── 03_overload.cpp
├── 04_void_return.cpp
└── Inputs
    ├── 01_solution.txt
    ├── 02_design.txt
    ├── 03_overload.txt
    └── 04_void_return.txt
```

if none of the settings are changed (just as *examples*).

<sup>* the input directory and extension can be customized by defining specific MACROs. `_LL_INPUT_DIR` and `_LL_INPUT_EXT` are used for the directory and extension of input files respectively</sup>

Each line of the input file contains all the arguments to execute the {method} or {class} **once**. The seperator of arguments are `,`, and spaces are skipped also.

Following is the example of `int, vector<int>, TreeNode*` as input
```txt
1, [1, 2], [1, null, 2]
2, [3, 2], [5, null, 2, null, 3]
```

For design type, the format of each line is `["{method_names}", ...], [[{arguments}], ...]`.

- Build the library

check section [build](#build)

- Compilation flags

```bash
clang++ -std=c++17 {source_name.cpp} -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -O2 -Ipath/to/local_leetcode/include -Lpath/to/local_leetcode/lib -o {output_executable} -llocal_leetcode
```

<sup>* ~~To use `libc++` on non-Mac OS,`-stdlib=libc++` is added~~ add flag `-stdlib=c++` and link `-lc++abi` if `-DUSE_LIBC++=on` was enabled for building the library</sup>

<sup>* to suppress memory leak error, try setting environment variable `ASAN_OPTIONS=detect_leaks=0`</sup>


Check the directory [*examples*](https://github.com/yuhenghuang/local_leetcode/tree/main/examples) for more granular cases.

## Build

* Requirements
  * cmake (3.16.3 or higher)
  * clang++ (11 or higher)
  * libcxx or libc++ (no longer enforce the use of libc++)
  * ~~libc++-dev libc++abi-dev (to use std provided by llvm)~~
  * build tool. `make` on linux or `ninja` (or alike) on windows
* Other details
  * to build examples, please add `-DBUILD_EXAMPLES=on` in cmake arguments
  * to use libc++ on non-macOS machines, please add `-DUSE_LIBC++=on` in cmake arguments
    * if libc++ is enabled, don't forget to add flag `-stdlib=c++` and link `-lc++abi` when compiling executables against the built library
  * it's not recommended to install the library in the system paths because of its limited usage
  * please use soft link (`ln -s` on unix or `mklink` on windows) to achieve similar goals.


### Unix-like OS

The requirements should be almost (already) be met on most unix-like OS. Thus following 

```bash
mkdir build
cd build

# add
# -DBUILD_EXAMPLES=on -DUSE_LIBC++=on
# to enable building examples and use libc++ on non-macos machine
cmake .. -DCMAKE_CXX_COMPILER=clang++  -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

The built library should be *lib/liblocal_leetcode.so* (or *lib/liblocal_leetcode.dylib* on macOS).

Use `ln -s` or update `ld` configurations to make the library to be loaded at runtime.

### Windows (msys2)

To avoid the standard library of `msvc`, which is not completely open source, it's better to install gnu/llvm on windows.

After testing several approaches on windows, *msys2* is recommended because it's less likely to encounter unsolvable bugs.

As stated in the requirements, from *msys2*, the following libraries/tools are needed

- clang-toolchain (comes with clang targeted to gnu)
  - required libraries should come with the toolchain by default
- ninja (or other preferred build tool)

After other steps like setting `PATH`, one can now start building

```bat
mkdir build
cd build

:: add
:: -DBUILD_EXAMPLES=on -DUSE_LIBC++=on
:: to enable building examples and use libc++ on non-macos machine

:: please specify correct make target (e.g. Ninja)
cmake .. -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

The built library should be in *bin/liblocal_leetcode.dll* and *lib/liblocal_leetcode.dll.a*.

Use `mklink` to put *.dll* file in the directory of executables or set `PATH` to make the library to be loaded at runtime.

## Special Cases

#### Node types

To enable correct `Node` type for specific problem from various types, one needs to define macro before the header file.

For example, to use a tree node with four children as typename `Node`, 

```cpp
#define QUAD_NODE
#include <local_leetcode>
```

For more definition of marcos, see the content of the header file, [*include/local_leetcode.hpp*](https://github.com/yuhenghuang/local_leetcode/blob/main/include/local_leetcode.hpp)


#### Inconsistent input type

The macro `EXECS` supports overload.

The syntax is

```cpp
EXECS(Solution::method, return_type, (arg_types, ...));
```

#### non-trivial constructor of class `Solution`

The library internally distinguishes *solution* and *design* types of problems by checking whether the class is trivially constructable.

This logic fails if `Solution` has explicit constructors or non-primitive members. 

To enforce the behavior of the library under the condition where explicit constructor in `Solution` in unavoidable, simply define

```cpp
#define _LL_NONTRIVIAL_SOLUTION_CTOR
#include <local_leetcode.hpp>
```

before including the library header file.

#### memory leaking of return pointer (optional)

This is an optional feature which releases the memory of return pointer, thus avoid memory leaking. The module is implemented by heavy meta-programming techniques and should cover most common data types (e.g. tree). Pointers in form of vector, e.g. `std::vector<Node*>` are also supported.

To enable it, 

```cpp
#define _LL_DELETE_RETURN_POINTER
#include <local_leetcode.hpp>
```

For more details of the supported data types of the feature, see the following section.

#### destroy (delete) pointers (optional)


This function, `ll::destroy(void *)` from [*include/io/destroyer.hpp*](https://github.com/yuhenghuang/local_leetcode/blob/main/include/io/destroyer.hpp),  is used to release memories of the interim objects in the heap.

The original purpose of the function is to manage the memory perfectly by the user him/herself, while it came to light that the task was impossible during the development and test. The function remains specifically for someone who wants to manage the memory by oneself without setting `ASAN_OPTIONS=detect_leaks=0`.

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


## FAQ

1. What are the purposes of the library when those websites provide online compilation and execution?

- manage code and test cases in a local directory
  - further manage alternative solutions in a *cpp* file
- debug locally (this is the biggest plus)
- use one's favorite IDE


2. Why is built version of the library preferred to header-only one?

- reduce compilation time of solutions drastically
  - the exposure of STL symbols contributes (e.g. `std::vector<int>`)
- reduce the size of compiled binary of the solution


3. Why is the compiler specified as `clang` and not other compilers?

- in line with the instructions from the official site
- same goes for c++ standards and the compilation flags, including *asan* flags
- same goes for the choice of STL library `libc++`


4. Can test cases be skipped to avoid some annoying inconveniences, like long outputs blocking the terminal when debugging?

- Test cases can be commented out by `# ` at the beginning of a line to skip it.

- The maximum printed output of `vector<>` type can also be adjusted macros, e.g. use `#define _LL_PRINT_ARRAY_MAX_LENGTH 100` to limit the output elements of an array to be non-greater than 100.


For other more specific questions, the wiki will be available in the future.


## License

The library is released under the GNU GPL-3.0 ![license logo](https://www.gnu.org/graphics/gplv3-with-text-84x42.png) license.
