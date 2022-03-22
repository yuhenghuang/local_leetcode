------------------------------------------------------------------------------
2022-03-22

The document details the changes between this version, local leetcode 0.9.3.2 and previous version, local leetcode 0.9.3.1.

- add move constructor and assignment for type `NestedInteger`

- migrate from raw pointers to `std::unique_ptr` in file *include/core/classes.hpp* (not *input_parameters.hpp*) to simplify allocate and release of memories.

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
  - header-only version is forthcoming, not avaible now.

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
