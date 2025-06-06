#ifndef MYSTL_CORE_TYPE_TRAITS_H_
#define MYSTL_CORE_TYPE_TRAITS_H_

// MyTinySTL 类型特征
// Type traits for MyTinySTL

#include "config.h"
#include <type_traits>

MYSTL_NAMESPACE_BEGIN

// 基础类型特征 / Basic Type Traits
template <typename T, T v>
struct integral_constant
{
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;
    
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

template <bool B>
using bool_constant = integral_constant<bool, B>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

// 条件类型选择 / Conditional Type Selection
template <bool B, typename T, typename F>
struct conditional { using type = T; };

template <typename T, typename F>
struct conditional<false, T, F> { using type = F; };

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

// 类型相同性检查 / Type Equality Check
template <typename T, typename U>
struct is_same : false_type {};

template <typename T>
struct is_same<T, T> : true_type {};

// C++11 兼容性：不使用变量模板
// template <typename T, typename U>
// constexpr bool is_same_v = is_same<T, U>::value;

// 移除修饰符 / Remove Qualifiers
template <typename T>
struct remove_const { using type = T; };

template <typename T>
struct remove_const<const T> { using type = T; };

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
struct remove_volatile { using type = T; };

template <typename T>
struct remove_volatile<volatile T> { using type = T; };

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

template <typename T>
struct remove_cv
{
    using type = remove_volatile_t<remove_const_t<T>>;
};

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

// 移除引用 / Remove Reference
template <typename T>
struct remove_reference { using type = T; };

template <typename T>
struct remove_reference<T&> { using type = T; };

template <typename T>
struct remove_reference<T&&> { using type = T; };

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

// 添加引用 / Add Reference
template <typename T>
struct add_lvalue_reference { using type = T&; };

template <typename T>
struct add_lvalue_reference<T&> { using type = T&; };

template <>
struct add_lvalue_reference<void> { using type = void; };

template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template <typename T>
struct add_rvalue_reference { using type = T&&; };

template <typename T>
struct add_rvalue_reference<T&> { using type = T&; };

template <typename T>
struct add_rvalue_reference<T&&> { using type = T&&; };

template <>
struct add_rvalue_reference<void> { using type = void; };

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

// 移动语义支持 / Move Semantics Support
template <typename T>
constexpr remove_reference_t<T>&& move(T&& t) noexcept
{
    return static_cast<remove_reference_t<T>&&>(t);
}

template <typename T>
constexpr T&& forward(remove_reference_t<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& forward(remove_reference_t<T>&& t) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, 
                  "Cannot forward an rvalue as an lvalue");
    return static_cast<T&&>(t);
}

// 类型检查 / Type Checking
template <typename T>
struct is_integral : std::is_integral<T> {};

// C++11 兼容性：不使用变量模板
// template <typename T>
// constexpr bool is_integral_v = is_integral<T>::value;

template <typename T>
struct is_floating_point : std::is_floating_point<T> {};

// template <typename T>
// constexpr bool is_floating_point_v = is_floating_point<T>::value;

template <typename T>
struct is_arithmetic : bool_constant<is_integral<T>::value || is_floating_point<T>::value> {};

// template <typename T>
// constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template <typename T>
struct is_pointer : std::is_pointer<T> {};

// template <typename T>
// constexpr bool is_pointer_v = is_pointer<T>::value;

// 可平凡复制检查 / Trivially Copyable Check
template <typename T>
struct is_trivially_copyable : std::is_trivially_copyable<T> {};

// template <typename T>
// constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

// 可平凡构造检查 / Trivially Constructible Check
template <typename T>
struct is_trivially_default_constructible : std::is_trivially_default_constructible<T> {};

// template <typename T>
// constexpr bool is_trivially_default_constructible_v = is_trivially_default_constructible<T>::value;

// 可平凡析构检查 / Trivially Destructible Check
template <typename T>
struct is_trivially_destructible : std::is_trivially_destructible<T> {};

// template <typename T>
// constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;

// SFINAE 辅助 / SFINAE Helpers
template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> { using type = T; };

template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

MYSTL_NAMESPACE_END

#endif // MYSTL_CORE_TYPE_TRAITS_H_
