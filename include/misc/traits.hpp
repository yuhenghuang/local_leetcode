/**
 * @file traits.hpp
 * 
 * @copyright Copyright (c) 2021 - 2022, Yuheng Huang <kongqiota@gmail.com>
 * 
 * traits.hpp is part of library local leetcode, 
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


#ifndef _LL_TRAITS_HPP
#define _LL_TRAITS_HPP

#include "../data_structure/data_structure.hpp"
#include <string>
#include <type_traits>
#include <tuple>
#include <iostream>
#include <map>
#include <unordered_map>


namespace ll {

// scalar
template <typename>
struct rank: public std::integral_constant<size_t, 0UL> { };

// pointer, e.g. tree
template <typename Tp>
struct rank<Tp*>: public std::integral_constant<size_t, 1UL> { };

// vector
template <typename Tp>
struct rank<std::vector<Tp>>: public std::integral_constant<size_t, 1UL + rank<Tp>::value> { };

// nested integer
template <>
struct rank<NestedInteger>: public std::integral_constant<size_t, 1UL> { };

// sea
template <>
struct rank<Sea>: public std::integral_constant<size_t, 2UL> { };


// char or string
template <typename> struct is_character: public std::false_type { };

template <> struct is_character<std::string>: public std::true_type { };
template <> struct is_character<char>: public std::true_type { };


/**
 * @brief if operator<< is defined for Tp and Tp is not a pointer
 * 
 * @tparam Tp 
 */
template <typename Tp> struct is_printable;

/**
 * @brief if Tp is std::vector<Tp*> after removing const and reference
 * 
 * @tparam Tp 
 */
template <typename Tp> struct is_vector_of_pointers;


// remove references to pointer types
// (solve unfavorable reference to pointers after perfect forwarding)
template <typename Tp> struct remove_reference_to_pointer { typedef Tp type; };
template <typename Tp> struct remove_reference_to_pointer<Tp* &> { typedef Tp* type; };
template <typename Tp> struct remove_reference_to_pointer<Tp* &&> { typedef Tp* type; };


template <typename... Args> struct args_pack { };


template <
  typename Tp, 
  bool IsVecofPtrs = is_vector_of_pointers<Tp>::value
>
class input_parameter;


// remove const volatile noexcept
template <class Fn>
struct remove_fn_cv_noexcept;

template <class Ret, class... Args>
struct remove_fn_cv_noexcept<Ret (Args...)> {
  typedef Ret (type)(Args...);
};

template <class Ret, class... Args>
struct remove_fn_cv_noexcept<Ret (Args...) const> {
  typedef Ret (type)(Args...);
};

template <class Ret, class... Args>
struct remove_fn_cv_noexcept<Ret (Args...) volatile> {
  typedef Ret (type)(Args...);
};

template <class Ret, class... Args>
struct remove_fn_cv_noexcept<Ret (Args...) const volatile> {
  typedef Ret (type)(Args...);
};

template <class Ret, class... Args>
struct remove_fn_cv_noexcept<Ret (Args...) noexcept> {
  typedef Ret (type)(Args...);
};

template <class Ret, class... Args>
struct remove_fn_cv_noexcept<Ret (Args...) const noexcept> {
  typedef Ret (type)(Args...);
};

template <class Ret, class... Args>
struct remove_fn_cv_noexcept<Ret (Args...) volatile noexcept> {
  typedef Ret (type)(Args...);
};

template <class Ret, class... Args>
struct remove_fn_cv_noexcept<Ret (Args...) const volatile noexcept> {
  typedef Ret (type)(Args...);
};


// fn_has_const
template <class Fn>
struct fn_has_const : public std::false_type { };

template <class Ret, class... Args>
struct fn_has_const<Ret (Args...) const> : public std::true_type { };

template <class Ret, class... Args>
struct fn_has_const<Ret (Args...) const noexcept> : public std::true_type { };


// function type traits
template <class Fn> struct fn_traits;

template <class Ret, class... Args>
struct fn_traits<Ret (Args...)> :
  public std::integral_constant<size_t, sizeof...(Args)> 
{
  typedef Ret return_type;

  typedef std::tuple<input_parameter<
    typename remove_reference_to_pointer<Args>::type
  > ...> args_tuple_param_type;

  // extract arguments type by index
  template <size_t I>
  struct args_type {
    typedef typename std::tuple_element<I, std::tuple<Args...>>::type type;
  };
};


// function / class member function traits
template <typename FnPtr> struct fn_ptr_traits;

// member function pointer
template <class Fn, class Cp>
struct fn_ptr_traits<Fn Cp::*>:
  public fn_traits<typename remove_fn_cv_noexcept<Fn>::type>
{
  typedef typename remove_fn_cv_noexcept<Fn>::type Cp::* fn_ptr_raw_type;

  typedef Cp class_type;
  typedef fn_has_const<Fn> has_const;
};

// function pointer
template <class Fn>
struct fn_ptr_traits<Fn*>:
  public fn_traits<typename remove_fn_cv_noexcept<Fn>::type>
{
  typedef typename remove_fn_cv_noexcept<Fn>::type* fn_ptr_raw_type;
  typedef fn_has_const<Fn> has_const;
};

// specialization for nullptr_t
template <>
struct fn_ptr_traits<nullptr_t>: 
  public std::integral_constant<size_t, 0UL> 
{
  typedef nullptr_t fn_ptr_raw_type;
  typedef std::false_type has_const;
};


template <bool... > struct bool_dummies { };

// if predicates are all true
// todo: migrate to std::conjunction in the future
template <bool... Preds>
struct all : public std::is_same<
                      bool_dummies<Preds ...>, 
                      bool_dummies<((void)Preds, true) ...>
                    > 
{ };


// negation of true_type or false_type
template <typename PredType> struct negate;

template <bool Pred>
struct negate<std::bool_constant<Pred>> : public std::bool_constant<!Pred> { };


// check if all Types are the same Tp
// true -> type = Tp, value = true
// false -> type = void, value = false
template <typename... Types> struct all_same;
// end condition
template <typename Tp> struct all_same<Tp> : public std::true_type { typedef Tp type; };

template <typename Tp, typename Up, typename... Types>
struct all_same<Tp, Up, Types...> : public std::false_type { typedef void type; };

template <typename Tp, typename... Types>
struct all_same<Tp, Tp, Types...> : public all_same<Tp, Types...> { };


// extend std::is_array to std::array and std::vector
template <typename Tp>
struct is_array: public std::is_array<Tp> { };

template <typename Tp, size_t N>
struct is_array<std::array<Tp, N>> : public std::true_type { };

template <typename Tp>
struct is_array<std::vector<Tp>> : public std::true_type { };


// extend std::remove_extent to std::array, std::vector, std::map and std::unordered_map
template <typename Tp>
struct remove_extent: public std::remove_extent<Tp> { };

template <typename Tp, size_t N>
struct remove_extent<std::array<Tp, N>> { typedef Tp type; };

template <typename Tp>
struct remove_extent<std::vector<Tp>> { typedef Tp type; };

template <typename Key, typename Tp>
struct remove_extent<std::map<Key, Tp>> { typedef Tp type; };

template <typename Key, typename Tp>
struct remove_extent<std::unordered_map<Key, Tp>> { typedef Tp type; };


struct linked_list_tag { };
struct binary_tree_tag { };
struct quad_tree_tag { };
struct nary_tree_tag { };
struct nary_graph_tag { };
struct single_node_tag { };

// graph_category<Tp>::graph_category_tag 
template <typename Tp> struct graph_category;

// whether the linked list node is doubly linked
template <typename Tp> struct has_prev;

// is array (c or c++11) or std::vector of pointers
template <typename Tp>
struct is_array_of_pointers: 
  public all<
    is_array<Tp>::value,
    std::is_pointer<typename remove_extent<Tp>::type>::value
  >
{ };


namespace internal {

// is operator<< defined?
struct do_is_printable_impl {
  template <typename Tp, typename = decltype(std::cout << std::declval<Tp&>())>
  static std::true_type test(int);

  template <typename>
  static std::false_type test(...);
};

template <typename Tp>
struct is_printable_impl: public do_is_printable_impl {
  typedef decltype(test<Tp>(0)) type;
};


// is vector of pointers
template <typename Tp>
struct is_vector_of_pointers_impl: public std::false_type { };

template <typename Tp>
struct is_vector_of_pointers_impl<std::vector<Tp*>>: public std::true_type { };


// detect graph category
struct do_graph_category_impl {
  template <
    typename Tp,
    typename = typename std::enable_if<
      std::is_same<Tp, decltype(std::declval<Tp&>()->next)>::value
    >::type
  >
  static linked_list_tag test(int16_t);

  template <
    typename Tp, 
    typename = typename std::enable_if<
      std::conjunction<
        std::is_same<Tp, decltype(std::declval<Tp&>()->left)>,
        std::is_same<Tp, decltype(std::declval<Tp&>()->right)>
      >::value
    >::type
  >
  static binary_tree_tag test(int32_t);

  template <
    typename Tp, 
    typename = typename std::enable_if<
      std::conjunction<
        std::is_same<Tp, decltype(std::declval<Tp&>()->topLeft)>,
        std::is_same<Tp, decltype(std::declval<Tp&>()->topRight)>,
        std::is_same<Tp, decltype(std::declval<Tp&>()->bottomLeft)>,
        std::is_same<Tp, decltype(std::declval<Tp&>()->bottomRight)>
      >::value
    >::type
  >
  static quad_tree_tag test(int64_t);

  template <
    typename Tp, 
    typename = typename std::enable_if<
      std::is_same<
        Tp,
        typename remove_extent<decltype(std::declval<Tp&>()->children)>::type
      >::value
    >::type
  >
  static nary_tree_tag test(uint32_t);

  template <
    typename Tp,
    typename Up = decltype(std::declval<Tp&>()->neighbors),
    typename = typename std::enable_if<
      std::conjunction<
        is_array<Up>,
        std::is_same<Tp, typename remove_extent<Up>::type>
      >::value
    >::type
  >
  static nary_graph_tag test(uint64_t);

  template <typename>
  static single_node_tag test(...);
};


template <typename Tp>
struct graph_category_impl: public do_graph_category_impl {
  typedef decltype(test<Tp>(0)) type;
};


// has prev link?
// handle cyclic linked list
struct do_has_prev_impl {
  template <typename Tp, typename = decltype(std::declval<Tp&>()->prev)>
  static std::true_type test(int);

  template <typename>
  static std::false_type test(...);
};

template <typename Tp>
struct has_prev_impl : public do_has_prev_impl {
  typedef decltype(test<Tp>(0)) type;
};


} // end of internal


template <typename Tp>
struct is_printable : 
  public all<
    internal::is_printable_impl<Tp>::type::value,
    !std::is_pointer<Tp>::value
  >
{ };


template <typename Tp>
struct is_vector_of_pointers: 
  public internal::is_vector_of_pointers_impl<
    typename std::remove_cv<
      typename std::remove_reference<Tp>::type
    >::type
  > 
{ };


template <typename Tp>
struct graph_category: public internal::graph_category_impl<Tp>::type {
  typedef typename internal::graph_category_impl<Tp>::type graph_category_tag;
};


template <typename Tp>
struct has_prev : public internal::has_prev_impl<Tp>::type { };


} // end of ll
#endif // end of _LL_TRAITS_HPP