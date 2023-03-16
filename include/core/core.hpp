/**
 * @file core.hpp
 * 
 * @copyright Copyright (c) 2021 - 2023, Yuheng Huang <kongqiota@gmail.com>
 * 
 * core.hpp is part of library local leetcode, 
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


#ifndef _LL_CORE_HPP
#define _LL_CORE_HPP


#include "classes.hpp"
#include <fstream>

namespace ll {


/**
 * @brief construct class by specified ctor (Args) and call methods by
 * parsing the input file located at path as parameters (and method names)
 * 
 * @tparam Args argument types for a specific ctor, 
 *   must be explicitly specified (no implicit deduction available)
 * @tparam MemFns pointers to member functions
 * 
 * @param path path to input file
 * @param method_names member function names generated by MACRO #__VA_ARGS__
 * @param mem_fns pointers to member functions
 * 
 * @return nothing
 */
template <
  typename... Args, 
  typename... MemFns
>
void 
execx(const std::string& path,
      const std::string& method_names,
      MemFns... mem_fns);


/**
 * @brief construct class by default ctor and call method by
 * parsing the input file located at path as parameters
 * 
 * @tparam MemFns pointers to member function
 * 
 * @param path path to input file
 * @param method_name member function names generated by MACRO #METHOD
 * @param mem_fn pointer to member function
 * 
 * @return nothing
 */
template <typename MemFn>
void 
execs(const std::string& path,
      const std::string& method_name_raw,
      MemFn mem_fn);


#ifndef _LL_INPUT_DIR
#define _LL_INPUT_DIR "Inputs"
#endif

#ifndef _LL_INPUT_EXT
#define _LL_INPUT_EXT "txt"
#endif

#define _LL_INPUT_FILE ll::path_to_input_file(_LL_INPUT_DIR, _LL_INPUT_EXT, __FILE__)

#define CTOR(...) __VA_ARGS__

#define EXECX(CTOR_ARGS, ...) \
  ll::execx<CTOR_ARGS>( \
    _LL_INPUT_FILE, \
    #__VA_ARGS__, __VA_ARGS__ \
  )


#define EXECS_BASE(METHOD) \
  ll::execs( \
    _LL_INPUT_FILE, \
    #METHOD, &METHOD \
  )

/**
 * @brief explicitly specify the type of member function pointer
 *   to handle override correctly
 * 
 */
#define EXECR_BASE(METHOD, RETURN, ARGS) \
  ll::execs<RETURN (Solution::*) ARGS>( \
    _LL_INPUT_FILE, \
    #METHOD, &METHOD \
  )


#define EXECR_NOARGS(METHOD, RETURN) EXECR_BASE(METHOD, RETURN, ());

#define GETEXEC(DUMMY1, DUMMY2, DUMMY3, NAME, ...) NAME

#define EXECS(...) GETEXEC(__VA_ARGS__, EXECR_BASE, EXECR_NOARGS, EXECS_BASE) (__VA_ARGS__)


namespace internal {

/*
template <typename Cp, typename... Args>
Cp* 
class_factory(Args&&... args) 
noexcept(std::is_nothrow_constructible<Cp, Args&&...>::value)
{
  return new Cp(std::forward<Args>(args) ...);
}
*/

} // end of internal


template <
  typename... Args, 
  typename... MemFns
>
void 
execx(const std::string& path,
      const std::string& method_names,
      MemFns... mem_fns)
{
  typedef typename all_same<
    typename fn_ptr_traits<MemFns>::class_type ...
  >::type class_type;

  static_assert(!std::is_void<class_type>::value, "all methods must be from the same class!");
  static_assert(std::is_constructible<class_type, Args...>::value, "no specified ctor exists!");

  // parse method names by regex iterator
  std::regex re("&(\\w+)::(\\w+)");
  std::sregex_iterator iter(method_names.begin(), method_names.end(), re);

  std::string class_name = iter->str(1);

  universal_class<class_type, Args...> cls;

  // add methods and names to wrapper (feature since c++17)
  ( cls.add_method(mem_fns, iter++->str(2)), ... );

  std::ifstream f(path);
  std::string line;
  double exec_time = 0; // execution time
  while (getline(f, line)) {

    if (line.front() == '#')
      continue;

    // parse input file (methods and arguments)

    param_range range = find_param_range<std::vector<std::string>>()(line, 0);
    auto&& methods = universal_parser<std::vector<std::string>>()(line.substr(range.i, range.l));

    // find start of parameters
    size_t idx = range.j;
    internal::skip_delimiters(line, idx);

    ++idx; // skip first '['
    assert(idx < line.size() && "For the input of design type, the methods and parameters should be in ONE line and seperated by ','");

    // arbitrary std::vector<Tp> will do
    param_iterator<std::vector<int>> param(line, idx);

    std::string_view args;
    param.get_next(args);

    // need to remove '[' and ']' on both sides
    cls.initialize_or_replace(args.substr(1, args.size() - 2));

    // ctor prints nothing
    std::cout << "[null" << std::flush;

    for (size_t i = 1; i < methods.size(); ++i) {
      std::cout << ", ";

      param.get_next(args);

      exec_time += cls(methods[i], args.substr(1, args.size() - 2));
      std::cout << std::flush;
    }

    std::cout << ']' << std::endl;

  } // end of while loop

  std::cout << "****** Execution time of `" << class_name  \
  << "` is: " << exec_time << " milliseconds. ******" << std::endl;
}


template <typename MemFn>
void 
execs(const std::string& path,
      const std::string& method_name_raw,
      MemFn mem_fn) 
{
  typedef typename fn_ptr_traits<MemFn>::class_type class_type;
  // assert default ctor
  static_assert(std::is_default_constructible<class_type>::value, "no default ctor");

  universal_class<class_type> cls;

  std::string method_name = 
    method_name_raw.substr(method_name_raw.find_last_of(':') + 1);

  // create instance and add method and name to wrapper
  cls.initialize_or_replace().add_method(mem_fn, method_name);

  std::ifstream f(path);
  std::string line;
  double exec_time = 0; // execution time
  while (getline(f, line)) {

    if (line.front() == '#')
      continue;

    exec_time += cls(method_name, line);

    std::cout << std::endl;

  } // end of while loop

  std::cout << "****** Execution time of `" << method_name \
  << "` is: " << exec_time << " milliseconds. ******" << std::endl;
}


} // end of ll
#endif // end of _LL_CORE_HPP
