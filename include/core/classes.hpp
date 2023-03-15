/**
 * @file classes.hpp
 * 
 * @copyright Copyright (c) 2021 - 2023, Yuheng Huang <kongqiota@gmail.com>
 * 
 * classes.hpp is part of library local leetcode, 
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


#ifndef _LL_CLASSES_HPP
#define _LL_CLASSES_HPP

#include "input_parameter.hpp"
#include "../io/printer.hpp"
#include "../io/destroyer.hpp"

#include <memory>


namespace ll {


template <typename Cp>
class method_class_base {
  private:
    typedef method_class_base<Cp> self;
    typedef Cp class_type;

  public:
    virtual std::string get_name() const = 0;

    virtual double operator()(class_type*, const std::string&) const = 0;

    virtual self* clone() const = 0;

    virtual ~method_class_base() { }
};


/**
 * @brief allow to call a class method given string representations of method name and arguments
 * 
 * @tparam MemFn member function pointer
 */
template <class MemFn>
class method_class : public method_class_base<typename fn_ptr_traits<MemFn>::class_type> {
  public:
    typedef MemFn mem_fn_ptr;
    typedef typename fn_ptr_traits<MemFn>::class_type class_type;
    typedef typename fn_ptr_traits<MemFn>::return_type return_type;
    typedef typename fn_ptr_traits<MemFn>::args_tuple_param_type args_tuple_param_type;

  private:
    typedef method_class_base<class_type> base;
    typedef method_class<mem_fn_ptr> self;

    constexpr static size_t args_size = fn_ptr_traits<MemFn>::value;

    // member function pointer
    mem_fn_ptr fn;

    // method name
    std::string name;

    /**
     * @brief execute the member function (NO return) given pointer to class and arguments
     * print first argument (as it shall be modified in-place)
     * 
     * @tparam Tp return type of member function
     * @tparam Is index sequence of the size of arguments
     * 
     * @param ptr pointer to class 
     * @param params arguments wrapped in tuple and input_parameter, e.g. tuple<input_parameter<...>>
     * @return execution time
     */
    template <typename Tp = return_type, size_t... Is>
    typename std::enable_if<std::is_void<Tp>::value, double>::type 
    exec_call(class_type* ptr, 
              args_tuple_param_type& params, 
              std::index_sequence<Is...>) const;

    /**
     * @brief execute the member function (HAS return) given pointer to class and arguments
     * print returned result
     * 
     * @tparam Tp return type of member function
     * @tparam Is index sequence of the size of arguments
     * 
     * @param ptr pointer to class 
     * @param params arguments wrapped in tuple and input_parameter, e.g. tuple<input_parameter<...>>
     * @return execution time
     */
    template <typename Tp = return_type, size_t... Is>
    typename std::enable_if<!std::is_void<Tp>::value, double>::type 
    exec_call(class_type* ptr, 
              args_tuple_param_type& params, 
              std::index_sequence<Is...>) const;

  public:
    method_class(mem_fn_ptr _fn, const std::string& _name)
      : fn(_fn), name(_name) { }

    virtual std::string get_name() const override { return name; }

    /**
     * @brief execute the member function given pointer to class and arguments in string format
     * 
     * @param ptr pointer to class
     * @param s arguments in string format
     * @return double, execution time
     */
    virtual double operator()(class_type* ptr, const std::string& s) const override;

    virtual self* clone() const override { return new self(*this); }

    virtual ~method_class() { }
};


/**
 * @brief wraps everything (factory and method) up
 * allows user to call method function by string representation of method name and arguments
 * 
 * @tparam Cp class type
 * @tparam Args argument types of a specific ctor
 * 
 */
template <class Cp, class... Args>
class universal_class {
  private:
    typedef universal_class<Cp, Args...> self;
    typedef Cp class_type;

    typedef std::tuple<input_parameter<
      typename remove_reference_to_pointer<Args>::type
    > ...> args_tuple_param_type;

    constexpr static size_t args_size = sizeof...(Args);

    // in many cases the arguments of ctor have to live
    // at least as long as the object they created
    args_tuple_param_type inputs;

    // pointer to the instance used by all methods
    std::unique_ptr<class_type> ptr;

    // class_factory_ptr factory;
    std::vector< std::unique_ptr<method_class_base<class_type>> > methods;

    template <size_t... Is>
    void factory_helper(std::index_sequence<Is...>) {
      ptr = std::make_unique<class_type>(std::get<Is>(inputs) ...);
    }

    /*
    // delete ptr
    void release_object() { delete ptr; }

    // delete ptr and methods
    void release_memory() {
      release_object();

      // be careful of memory leak...
      for (auto& m : methods)
        delete m;

      methods.clear();
    }
    */

  public:
    explicit universal_class() { }

    // the reason that copy/move construction/assignment are deleted is
    // UNNECESSARY

    universal_class(const self&) = delete;
    self& operator=(const self& x) = delete;

    universal_class(self&& x) = delete;
    self& operator=(self&& x) = delete;

    /**
     * @brief initialize by creating new class_type object or replace current one
     * 
     * @param s string representation of input of class factory
     * @return self& 
     */
    self& initialize_or_replace(const std::string& s = "");

    /**
     * @brief add method given its function pointer and name in string format
     * 
     * @tparam MemFn member function pointer type
     * 
     * @param fn pointer to member function, must be a member of the specific class
     * @param s member function name in string
     * 
     * @return self& 
     */
    template <
      typename MemFn,
      typename = typename std::enable_if<
        std::is_same<class_type, typename fn_ptr_traits<MemFn>::class_type>::value
      >::type
    >
    self& add_method(MemFn fn, const std::string& s);

    /**
     * @brief call a method given name and arguments both in string
     * 
     * @param name method name in string
     * @param args arguments in string
     * 
     * @return double, execution time
     */
    double operator()(const std::string& name, const std::string& args);

    // ~universal_class() { release_memory(); }
};


namespace internal {

// parse next Tp in s after j, then update j to the end of Tp
template <typename Tp>
inline 
Tp 
parse_next_arg(size_t& j , const std::string& s) {
  skip_delimiters(s, j);

  param_range range = find_param_range<Tp>()(s, j);

  // update j in-place before return
  j = range.j;

  return universal_parser<Tp>()(s.substr(range.i, range.l));
}


// generate parameters of various types
template <typename Tuple, std::size_t... Is>
void generate_params(Tuple& params,
                     const std::string& s, 
                     std::index_sequence<Is...>) 
{
  // ...
  // j is a global index of s, indicating end of previous argument
  size_t j = 0;
  
  (
    (std::get<Is>(params) = 
      parse_next_arg<
        typename std::tuple_element_t<Is, Tuple>::type // tuple_element_t<> = input_parameter
      >(j, s)
    ), 
    ...
  );
}

} // end of internal


// method class

template <class MemFn>
double 
method_class<MemFn>::operator()(class_type* ptr, const std::string& s) const {
  args_tuple_param_type params;

  // generate parameters in-place
  internal::generate_params(params, s, std::make_index_sequence<args_size>{});

  return exec_call(ptr, params, std::make_index_sequence<args_size>{});
}


// universal_class

template <class Cp, class... Args>
auto
universal_class<Cp, Args...>::initialize_or_replace(const std::string& s) -> self& {
  internal::generate_params(inputs, s, std::make_index_sequence<args_size>{});
  
  factory_helper(std::make_index_sequence<args_size>{});

  return *this;
}

template <class Cp, class... Args>
template <typename MemFn, typename>
inline
auto
universal_class<Cp, Args...>::add_method(MemFn fn, const std::string& s) -> self& {
  methods.push_back( 
    std::make_unique<method_class<MemFn>>(fn, s) 
  );
  return *this;
}

template <class Cp, class... Args>
double 
universal_class<Cp, Args...>::operator()(const std::string& name, const std::string& args) {
  method_class_base<class_type>* method = nullptr;
  for (auto& m : methods)
    if (m->get_name() == name) {
      method = m.get();
      break;
    }

  if (!method) {
    std::cerr << "\nclass method " << name << " is not found (added). \n";
    abort();
  }
  
  return method->operator()(ptr.get(), args);
}


// method class

template <class MemFn>
template <typename Tp, size_t... Is>
typename std::enable_if<std::is_void<Tp>::value, double>::type 
method_class<MemFn>::exec_call(class_type* ptr, 
                               args_tuple_param_type& params, 
                               std::index_sequence<Is...>) const 
{
  auto start( std::chrono::high_resolution_clock::now() );

  (ptr->*fn)(std::get<Is>(params) ...);

  auto end( std::chrono::high_resolution_clock::now() );
  std::chrono::duration<double, std::milli> elapsed( end - start );


// in case nontrivial default ctor of Solution is unavoidable 
#ifndef _LL_NONTRIVIAL_SOLUTION_CTOR
  // distinguish solution type(single function) and design type by verifying 
  // whether class_type is trivially default constructible
  if constexpr (!std::is_trivially_default_constructible<class_type>::value)
    std::cout << "null";
  else
#endif
    universal_printer<
      typename std::tuple_element_t<0, args_tuple_param_type>::type
    >()(std::get<0>(params));

  return elapsed.count();
}


template <class MemFn>
template <typename Tp, size_t... Is>
typename std::enable_if<!std::is_void<Tp>::value, double>::type 
method_class<MemFn>::exec_call(class_type* ptr, 
                               args_tuple_param_type& params, 
                               std::index_sequence<Is...>) const 
{
  auto start( std::chrono::high_resolution_clock::now() );

  return_type res = (ptr->*fn)(std::get<Is>(params) ...);

  auto end( std::chrono::high_resolution_clock::now() );
  std::chrono::duration<double, std::milli> elapsed( end - start );


// in case nontrivial default ctor of Solution is unavoidable 
#ifndef _LL_NONTRIVIAL_SOLUTION_CTOR
  // distinguish solution type(single function) and design type by verifying 
  // whether class_type is trivially default constructible
  if constexpr (!std::is_trivially_default_constructible<class_type>::value && (rank<return_type>::value == 2UL)) {
    std::cout << "\n ";
    // print 2D array flatly
    universal_printer<return_type, 1UL>()(res);
  }
  else
#endif
    universal_printer<return_type>()(res);

// in case returned pointer type should be deleted here
// to avoid memory leak error
#ifdef _LL_DELETE_RETURN_POINTER
  universal_destroyer<return_type>()(res);
#endif

  return elapsed.count();
}


} // end of ll
#endif // end of _LL_CLASSES_HPP
