#ifndef MYSTL_ITERATORS_ITERATOR_BASE_H_
#define MYSTL_ITERATORS_ITERATOR_BASE_H_

// MyTinySTL 迭代器基础
// Iterator base for MyTinySTL

#include "../core/config.h"
#include "../core/type_traits.h"
#include <cstddef>

MYSTL_NAMESPACE_BEGIN

// 迭代器标签 / Iterator Tags
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// 迭代器基类 / Iterator Base Class
template <typename Category, typename T, typename Distance = std::ptrdiff_t,
          typename Pointer = T*, typename Reference = T&>
struct iterator
{
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};

// 迭代器特征萃取 / Iterator Traits
namespace detail
{
    // 检查是否有迭代器类别 / Check if has iterator category
    template <typename T>
    struct has_iterator_category
    {
    private:
        struct two { char a; char b; };
        template <typename U> static two test(...);
        template <typename U> static char test(typename U::iterator_category* = nullptr);
    public:
        static constexpr bool value = sizeof(test<T>(nullptr)) == sizeof(char);
    };

    // 迭代器特征实现 / Iterator Traits Implementation
    template <typename Iterator, bool>
    struct iterator_traits_impl {};

    template <typename Iterator>
    struct iterator_traits_impl<Iterator, true>
    {
        using iterator_category = typename Iterator::iterator_category;
        using value_type = typename Iterator::value_type;
        using difference_type = typename Iterator::difference_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
    };

    template <typename Iterator>
    struct iterator_traits_helper 
        : public iterator_traits_impl<Iterator, has_iterator_category<Iterator>::value> {};
}

// 迭代器特征 / Iterator Traits
template <typename Iterator>
struct iterator_traits : public detail::iterator_traits_helper<Iterator> {};

// 指针特化 / Pointer Specialization
template <typename T>
struct iterator_traits<T*>
{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
};

template <typename T>
struct iterator_traits<const T*>
{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;
};

// 迭代器类别检查 / Iterator Category Checking
template <typename Iterator, typename Tag>
struct has_iterator_category_of
{
private:
    template <typename U>
    static auto test_category(int) -> typename iterator_traits<U>::iterator_category;
    template <typename>
    static void test_category(...);

    using category = decltype(test_category<Iterator>(0));

public:
    static constexpr bool value = !is_same<category, void>::value &&
                                  (is_same<category, Tag>::value ||
                                   std::is_base_of<Tag, category>::value);
};

template <typename Iterator>
struct is_input_iterator : public has_iterator_category_of<Iterator, input_iterator_tag> {};

template <typename Iterator>
struct is_output_iterator : public has_iterator_category_of<Iterator, output_iterator_tag> {};

template <typename Iterator>
struct is_forward_iterator : public has_iterator_category_of<Iterator, forward_iterator_tag> {};

template <typename Iterator>
struct is_bidirectional_iterator : public has_iterator_category_of<Iterator, bidirectional_iterator_tag> {};

template <typename Iterator>
struct is_random_access_iterator : public has_iterator_category_of<Iterator, random_access_iterator_tag> {};

template <typename Iterator>
struct is_iterator
{
    static constexpr bool value = is_input_iterator<Iterator>::value || 
                                  is_output_iterator<Iterator>::value;
};

// 便利的类型别名 / Convenient Type Aliases
template <typename Iterator>
using iterator_category_t = typename iterator_traits<Iterator>::iterator_category;

template <typename Iterator>
using iterator_value_t = typename iterator_traits<Iterator>::value_type;

template <typename Iterator>
using iterator_difference_t = typename iterator_traits<Iterator>::difference_type;

template <typename Iterator>
using iterator_pointer_t = typename iterator_traits<Iterator>::pointer;

template <typename Iterator>
using iterator_reference_t = typename iterator_traits<Iterator>::reference;

// 迭代器操作函数 / Iterator Operation Functions

// 距离计算 / Distance Calculation
template <typename InputIterator>
constexpr iterator_difference_t<InputIterator>
distance_impl(InputIterator first, InputIterator last, input_iterator_tag)
{
    iterator_difference_t<InputIterator> result = 0;
    while (first != last)
    {
        ++first;
        ++result;
    }
    return result;
}

template <typename RandomAccessIterator>
constexpr iterator_difference_t<RandomAccessIterator>
distance_impl(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
    return last - first;
}

template <typename Iterator>
constexpr iterator_difference_t<Iterator>
distance(Iterator first, Iterator last)
{
    return distance_impl(first, last, iterator_category_t<Iterator>{});
}

// 迭代器前进 / Iterator Advance
template <typename InputIterator, typename Distance>
constexpr void advance_impl(InputIterator& it, Distance n, input_iterator_tag)
{
    while (n > 0)
    {
        ++it;
        --n;
    }
}

template <typename BidirectionalIterator, typename Distance>
constexpr void advance_impl(BidirectionalIterator& it, Distance n, bidirectional_iterator_tag)
{
    if (n >= 0)
    {
        while (n > 0)
        {
            ++it;
            --n;
        }
    }
    else
    {
        while (n < 0)
        {
            --it;
            ++n;
        }
    }
}

template <typename RandomAccessIterator, typename Distance>
constexpr void advance_impl(RandomAccessIterator& it, Distance n, random_access_iterator_tag)
{
    it += n;
}

template <typename Iterator, typename Distance>
constexpr void advance(Iterator& it, Distance n)
{
    advance_impl(it, n, iterator_category_t<Iterator>{});
}

// 下一个迭代器 / Next Iterator
template <typename Iterator>
constexpr Iterator next(Iterator it, 
                       iterator_difference_t<Iterator> n = 1)
{
    advance(it, n);
    return it;
}

// 前一个迭代器 / Previous Iterator
template <typename Iterator>
constexpr Iterator prev(Iterator it, 
                       iterator_difference_t<Iterator> n = 1)
{
    advance(it, -n);
    return it;
}

MYSTL_NAMESPACE_END

#endif // MYSTL_ITERATORS_ITERATOR_BASE_H_
