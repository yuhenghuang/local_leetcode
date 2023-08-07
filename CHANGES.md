------------------------------------------------------------------------------
2023-08-07 (as of)

The document details the changes between this version, local leetcode 0.9.4.2 and previous version, local leetcode 0.9.4.1.

- add parser for `MountainArray` (problem no.1095)


------------------------------------------------------------------------------
2023-06-08

The document details the changes between this version, local leetcode 0.9.4.1 and previous version, local leetcode 0.9.4.0.

- the use of `libc++` is no longer enforced. to enable it please add `-DUSE_LIBC++` in cmake arguments

- further optimized/simplified internal `input_parameter` to support n-dim (n >= 2) vector of pointers
  - 0-dim (single raw pointer) case is absorbed by in the same specialization now
  - non-pointer case is absorbed by the same specialization now. technically speaking these is no partial specialization now, template class only. 
  - changed internal implementations (use `universal_destroyer<...>` to manage memories instead of `unique_ptr<>`) to enable this feature
  - several type traits added for the implementation of the feature
  - new example added as test for the feature

- substring operations are now replaced by `string_view` to achieve better performance, especially for long inputs

- parsers for integers are now implemented in template (not specializations in *src/parser.cpp*)

- fix the print precision of `double` type by `std::setprecision(6)`


------------------------------------------------------------------------------
2023-02-11

The document details the changes between this version, local leetcode 0.9.4.0 and previous version, local leetcode 0.9.3.4.

- add parser for type `bool` (unquoted true or false)

- add examples to test if they are built. please execute `make test` to invoke tests

- support raw array (e.g. `Node* []`) of fixed size or variable size (need to pass length as second parameter) in functor `universal_destroyer<Tp[]>`.

- update debugger configurations in *examples/*
  - migrate from outdated `lldb-mi` to `lldb-vscode`
  - fix the deprecated parameter `stopAtEntry` to `stopOnEntry`

- optimize internal classes/functions
  - use `std::make_unique` to replace redundant `internal::class_factory` for class ctor
  - use `std::unique_ptr<value_type[]>` to manage memory for input parameters
  <sup>* note: remaining explicit `new` and `delete` operators exist in *src/*. migrating them to smart pointers will only affect the performance negatively while provide no visible benefit. thus, there will be NO plan to optimize them (e.g. *src/io/parser.cpp*) unless fatal bugs are found.</sup>


------------------------------------------------------------------------------
2022-09-23

The document details the changes between this version, local leetcode 0.9.3.4 and previous version, local leetcode 0.9.3.3.

- add `int rand7()` function to generate random integer over $[1, 7]$, and expose it from namespace

- add `include <stack>` in header file for convenience

- add explicit print format for `double` to be in line with official output.

- fix the logic mistake in printing `NaryTreeNode` which may result in redundant `null`

- fix the wrong operator implementation in type `Sea`, i.e. `operator==` to `operator=`

- fix the logic mistake of not being able to print empty `NestedInteger`

- fix the logic of failing to parse `'\''` inside a doubly quoted string

- minor fixes/updates on *README.md* and other files


------------------------------------------------------------------------------
2022-04-28

The document details the changes between this version, local leetcode 0.9.3.3 and previous version, local leetcode 0.9.3.2.

- add *FAQ* section to *README.md*

- minor fixes on internal functions/classes and documents

- update (simplify) the implementation of internal `property` to meet c++17 standards

- update the internal function pointer traits


------------------------------------------------------------------------------
2022-03-23

The document details the changes between this version, local leetcode 0.9.3.2 and previous version, local leetcode 0.9.3.1.

- add move constructor and assignment for type `NestedInteger`

- migrate from raw pointers to `std::unique_ptr` in file *include/core/classes.hpp* (not *input_parameters.hpp*) to simplify allocation and release of memories.

- check explicitly whether all methods are from the same class for *Design* problems by `static_assert` with pertinent error message.

- minor fixes on *README.md*


------------------------------------------------------------------------------
2022-03-16

The document details the changes between this version, local leetcode 0.9.3.1 and previous version, local leetcode 0.9.2.1.

- add parser and printer for type `RandomTreeNode` (i.e. binary tree node with one random pointer to a node in the tree)

- fix the logic mistake of the type of `Setter` in class `property`.

- minor fixes on function documentation


------------------------------------------------------------------------------
2022-03-04

The document details the changes between this version, local leetcode 0.9.2.1 and previous version, local leetcode 0.9.1.1.

- add printer for type `NaryTreeNode`

- minor fixes on function documentation


------------------------------------------------------------------------------
2022-02-21

The document details the changes between this version, local leetcode 0.9.1.1 and previous version, local leetcode 0.9.1.0.

- allows `AppleClang` as compiler

- add sample *vscode* configurations in *examples/* directory

- enable commentting out test cases by adding `#` at the beginning of the line

- add feature to control the maximum number of elements of array to be printed

- add move ctor to `Sea` type

- fixed typos in *README.md*


------------------------------------------------------------------------------
2022-02-14

The document details the changes between this version, local leetcode 0.9.1.0 and previous version, local leetcode 0.9.0.0.

- fixed wrong information in *README.md*
  - header-only version is forthcoming, not available now.

- fixed level of section titles to increase readability in *README.md*

- fixed omitted part in *README.md*
  - macros of `Node` types
  - dynamic link related suggestions after build

- add hyperlink for files or directories mentioned in *README.md*

- add example of working directory structure in *README.md*

- deleted unused code block in comment (in *include/io/destroyer.hpp::193*)

- add support for `Sea` type (problem no.1274)


------------------------------------------------------------------------------
2022-02-07

Release of local leetcode 0.9.0.0
