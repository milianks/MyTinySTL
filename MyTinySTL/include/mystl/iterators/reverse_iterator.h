#ifndef MYSTL_ITERATORS_REVERSE_ITERATOR_H_
#define MYSTL_ITERATORS_REVERSE_ITERATOR_H_

// MyTinySTL 反向迭代器
// Reverse iterator for MyTinySTL

#include "iterator_base.h"

MYSTL_NAMESPACE_BEGIN

// 反向迭代器 / Reverse Iterator
template <typename Iterator>
class reverse_iterator
{
public:
    using iterator_type = Iterator;
    using iterator_category = iterator_category_t<Iterator>;
    using value_type = iterator_value_t<Iterator>;
    using difference_type = iterator_difference_t<Iterator>;
    using pointer = iterator_pointer_t<Iterator>;
    using reference = iterator_reference_t<Iterator>;

protected:
    Iterator current_;  // 对应的正向迭代器 / Corresponding forward iterator

public:
    // 构造函数 / Constructors
    reverse_iterator() : current_() {}
    
    explicit reverse_iterator(iterator_type it) : current_(it) {}
    
    template <typename U>
    reverse_iterator(const reverse_iterator<U>& other) : current_(other.base()) {}

    // 赋值操作符 / Assignment Operators
    template <typename U>
    reverse_iterator& operator=(const reverse_iterator<U>& other)
    {
        current_ = other.base();
        return *this;
    }

    // 获取基础迭代器 / Get Base Iterator
    iterator_type base() const
    {
        return current_;
    }

    // 解引用操作符 / Dereference Operators
    reference operator*() const
    {
        Iterator temp = current_;
        return *--temp;
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    // 下标操作符 / Subscript Operator
    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }

    // 前置递增 / Pre-increment
    reverse_iterator& operator++()
    {
        --current_;
        return *this;
    }

    // 后置递增 / Post-increment
    reverse_iterator operator++(int)
    {
        reverse_iterator temp = *this;
        --current_;
        return temp;
    }

    // 前置递减 / Pre-decrement
    reverse_iterator& operator--()
    {
        ++current_;
        return *this;
    }

    // 后置递减 / Post-decrement
    reverse_iterator operator--(int)
    {
        reverse_iterator temp = *this;
        ++current_;
        return temp;
    }

    // 算术操作符 / Arithmetic Operators
    reverse_iterator operator+(difference_type n) const
    {
        return reverse_iterator(current_ - n);
    }

    reverse_iterator& operator+=(difference_type n)
    {
        current_ -= n;
        return *this;
    }

    reverse_iterator operator-(difference_type n) const
    {
        return reverse_iterator(current_ + n);
    }

    reverse_iterator& operator-=(difference_type n)
    {
        current_ += n;
        return *this;
    }
};

// 非成员函数 / Non-member Functions

// 比较操作符 / Comparison Operators
template <typename Iterator1, typename Iterator2>
bool operator==(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() == rhs.base();
}

template <typename Iterator1, typename Iterator2>
bool operator!=(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() != rhs.base();
}

template <typename Iterator1, typename Iterator2>
bool operator<(const reverse_iterator<Iterator1>& lhs,
               const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() > rhs.base();
}

template <typename Iterator1, typename Iterator2>
bool operator<=(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() >= rhs.base();
}

template <typename Iterator1, typename Iterator2>
bool operator>(const reverse_iterator<Iterator1>& lhs,
               const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() < rhs.base();
}

template <typename Iterator1, typename Iterator2>
bool operator>=(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs)
{
    return lhs.base() <= rhs.base();
}

// 算术操作符 / Arithmetic Operators
template <typename Iterator>
reverse_iterator<Iterator> operator+(
    typename reverse_iterator<Iterator>::difference_type n,
    const reverse_iterator<Iterator>& it)
{
    return it + n;
}

template <typename Iterator1, typename Iterator2>
auto operator-(const reverse_iterator<Iterator1>& lhs,
               const reverse_iterator<Iterator2>& rhs)
    -> decltype(rhs.base() - lhs.base())
{
    return rhs.base() - lhs.base();
}

// 创建反向迭代器的辅助函数 / Helper Function to Create Reverse Iterator
template <typename Iterator>
reverse_iterator<Iterator> make_reverse_iterator(Iterator it)
{
    return reverse_iterator<Iterator>(it);
}

MYSTL_NAMESPACE_END

#endif // MYSTL_ITERATORS_REVERSE_ITERATOR_H_
