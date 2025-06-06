#ifndef MYSTL_CONTAINERS_VECTOR_H_
#define MYSTL_CONTAINERS_VECTOR_H_

// MyTinySTL 向量容器
// Vector container for MyTinySTL

#include "../core/config.h"
#include "../core/exceptions.h"
#include "../core/type_traits.h"
#include "../memory/allocator.h"
#include "../iterators/iterator_base.h"
#include "../iterators/reverse_iterator.h"
#include <initializer_list>
#include <algorithm>

MYSTL_NAMESPACE_BEGIN

// 向量容器 / Vector Container
template <typename T, typename Allocator = allocator<T>>
class vector
{
public:
    // 类型定义 / Type Definitions
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = typename allocator_traits<Allocator>::size_type;
    using difference_type = typename allocator_traits<Allocator>::difference_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename allocator_traits<Allocator>::pointer;
    using const_pointer = typename allocator_traits<Allocator>::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = mystl::reverse_iterator<iterator>;
    using const_reverse_iterator = mystl::reverse_iterator<const_iterator>;

private:
    // 成员变量 / Member Variables
    pointer begin_;      // 起始位置 / Start position
    pointer end_;        // 结束位置 / End position
    pointer capacity_;   // 容量结束位置 / Capacity end position
    allocator_type alloc_;  // 分配器 / Allocator

    // 辅助函数 / Helper Functions
    void deallocate_all()
    {
        if (begin_)
        {
            destroy_range(begin_, end_);
            allocator_traits<Allocator>::deallocate(alloc_, begin_, capacity_ - begin_);
            begin_ = end_ = capacity_ = nullptr;
        }
    }

    void destroy_range(pointer first, pointer last)
    {
        for (; first != last; ++first)
        {
            allocator_traits<Allocator>::destroy(alloc_, first);
        }
    }

    size_type calculate_growth(size_type new_size) const
    {
        const size_type current_capacity = capacity_ - begin_;
        const size_type max_size_val = max_size();
        
        MYSTL_THROW_LENGTH_ERROR_IF(new_size > max_size_val, 
            "vector::calculate_growth: new_size exceeds max_size");
        
        if (current_capacity > max_size_val - current_capacity / 2)
        {
            return max_size_val;
        }
        
        const size_type geometric_growth = current_capacity + current_capacity / 2;
        return (std::max)(new_size, geometric_growth);
    }

    void reallocate(size_type new_capacity)
    {
        if (new_capacity == 0)
        {
            deallocate_all();
            return;
        }

        pointer new_begin = allocator_traits<Allocator>::allocate(alloc_, new_capacity);
        pointer new_end = new_begin;
        
        try
        {
            // 移动或复制现有元素 / Move or copy existing elements
            for (pointer it = begin_; it != end_; ++it, ++new_end)
            {
                if (std::is_nothrow_move_constructible<T>::value)
                {
                    allocator_traits<Allocator>::construct(alloc_, new_end, mystl::move(*it));
                }
                else
                {
                    allocator_traits<Allocator>::construct(alloc_, new_end, *it);
                }
            }
        }
        catch (...)
        {
            destroy_range(new_begin, new_end);
            allocator_traits<Allocator>::deallocate(alloc_, new_begin, new_capacity);
            throw;
        }

        deallocate_all();
        begin_ = new_begin;
        end_ = new_end;
        capacity_ = new_begin + new_capacity;
    }

public:
    // 构造函数 / Constructors
    vector() noexcept(noexcept(Allocator()))
        : begin_(nullptr), end_(nullptr), capacity_(nullptr), alloc_() {}

    explicit vector(const Allocator& alloc) noexcept
        : begin_(nullptr), end_(nullptr), capacity_(nullptr), alloc_(alloc) {}

    explicit vector(size_type count, const T& value = T(), const Allocator& alloc = Allocator())
        : begin_(nullptr), end_(nullptr), capacity_(nullptr), alloc_(alloc)
    {
        if (count > 0)
        {
            begin_ = allocator_traits<Allocator>::allocate(alloc_, count);
            end_ = begin_;
            capacity_ = begin_ + count;
            
            try
            {
                for (size_type i = 0; i < count; ++i, ++end_)
                {
                    allocator_traits<Allocator>::construct(alloc_, end_, value);
                }
            }
            catch (...)
            {
                destroy_range(begin_, end_);
                allocator_traits<Allocator>::deallocate(alloc_, begin_, count);
                throw;
            }
        }
    }

    template <typename InputIt>
    vector(InputIt first, InputIt last, const Allocator& alloc = Allocator(),
           typename enable_if<!is_integral<InputIt>::value>::type* = nullptr)
        : begin_(nullptr), end_(nullptr), capacity_(nullptr), alloc_(alloc)
    {
        assign(first, last);
    }

    vector(const vector& other)
        : begin_(nullptr), end_(nullptr), capacity_(nullptr), 
          alloc_(allocator_traits<Allocator>::select_on_container_copy_construction(other.alloc_))
    {
        assign(other.begin(), other.end());
    }

    vector(vector&& other) noexcept
        : begin_(other.begin_), end_(other.end_), capacity_(other.capacity_), alloc_(mystl::move(other.alloc_))
    {
        other.begin_ = other.end_ = other.capacity_ = nullptr;
    }

    vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
        : vector(init.begin(), init.end(), alloc) {}

    // 析构函数 / Destructor
    ~vector()
    {
        deallocate_all();
    }

    // 赋值操作符 / Assignment Operators
    vector& operator=(const vector& other)
    {
        if (this != &other)
        {
            assign(other.begin(), other.end());
        }
        return *this;
    }

    vector& operator=(vector&& other) noexcept
    {
        if (this != &other)
        {
            deallocate_all();
            begin_ = other.begin_;
            end_ = other.end_;
            capacity_ = other.capacity_;
            alloc_ = mystl::move(other.alloc_);
            other.begin_ = other.end_ = other.capacity_ = nullptr;
        }
        return *this;
    }

    vector& operator=(std::initializer_list<T> init)
    {
        assign(init.begin(), init.end());
        return *this;
    }

    // 赋值函数 / Assign Functions
    void assign(size_type count, const T& value)
    {
        clear();
        if (count > 0)
        {
            reserve(count);
            for (size_type i = 0; i < count; ++i)
            {
                push_back(value);
            }
        }
    }

    template <typename InputIt>
    void assign(InputIt first, InputIt last,
                typename enable_if<!is_integral<InputIt>::value>::type* = nullptr)
    {
        clear();
        for (; first != last; ++first)
        {
            push_back(*first);
        }
    }

    void assign(std::initializer_list<T> init)
    {
        assign(init.begin(), init.end());
    }

    // 获取分配器 / Get Allocator
    allocator_type get_allocator() const noexcept
    {
        return alloc_;
    }

    // 元素访问 / Element Access
    reference at(size_type pos)
    {
        MYSTL_THROW_OUT_OF_RANGE_IF(pos >= size(), "vector::at: index out of range");
        return begin_[pos];
    }

    const_reference at(size_type pos) const
    {
        MYSTL_THROW_OUT_OF_RANGE_IF(pos >= size(), "vector::at: index out of range");
        return begin_[pos];
    }

    reference operator[](size_type pos) noexcept
    {
        MYSTL_DEBUG_ASSERT(pos < size(), "vector::operator[]: index out of range");
        return begin_[pos];
    }

    const_reference operator[](size_type pos) const noexcept
    {
        MYSTL_DEBUG_ASSERT(pos < size(), "vector::operator[]: index out of range");
        return begin_[pos];
    }

    reference front() noexcept
    {
        MYSTL_DEBUG_ASSERT(!empty(), "vector::front: container is empty");
        return *begin_;
    }

    const_reference front() const noexcept
    {
        MYSTL_DEBUG_ASSERT(!empty(), "vector::front: container is empty");
        return *begin_;
    }

    reference back() noexcept
    {
        MYSTL_DEBUG_ASSERT(!empty(), "vector::back: container is empty");
        return *(end_ - 1);
    }

    const_reference back() const noexcept
    {
        MYSTL_DEBUG_ASSERT(!empty(), "vector::back: container is empty");
        return *(end_ - 1);
    }

    pointer data() noexcept
    {
        return begin_;
    }

    const_pointer data() const noexcept
    {
        return begin_;
    }

    // 迭代器 / Iterators
    iterator begin() noexcept { return begin_; }
    const_iterator begin() const noexcept { return begin_; }
    const_iterator cbegin() const noexcept { return begin_; }

    iterator end() noexcept { return end_; }
    const_iterator end() const noexcept { return end_; }
    const_iterator cend() const noexcept { return end_; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    // 容量 / Capacity
    bool empty() const noexcept
    {
        return begin_ == end_;
    }

    size_type size() const noexcept
    {
        return static_cast<size_type>(end_ - begin_);
    }

    size_type max_size() const noexcept
    {
        return allocator_traits<Allocator>::max_size(alloc_);
    }

    void reserve(size_type new_cap)
    {
        if (new_cap > capacity())
        {
            reallocate(new_cap);
        }
    }

    size_type capacity() const noexcept
    {
        return static_cast<size_type>(capacity_ - begin_);
    }

    void shrink_to_fit()
    {
        if (capacity() > size())
        {
            reallocate(size());
        }
    }

    // 修改器 / Modifiers
    void clear() noexcept
    {
        destroy_range(begin_, end_);
        end_ = begin_;
    }

    iterator insert(const_iterator pos, const T& value)
    {
        return emplace(pos, value);
    }

    iterator insert(const_iterator pos, T&& value)
    {
        return emplace(pos, mystl::move(value));
    }

    iterator insert(const_iterator pos, size_type count, const T& value)
    {
        auto offset = pos - begin_;
        if (count == 0) return begin_ + offset;

        if (size() + count > capacity())
        {
            reallocate(calculate_growth(size() + count));
        }

        iterator insert_pos = begin_ + offset;

        // 移动现有元素 / Move existing elements
        if (insert_pos < end_)
        {
            std::move_backward(insert_pos, end_, end_ + count);
        }

        // 插入新元素 / Insert new elements
        for (size_type i = 0; i < count; ++i)
        {
            allocator_traits<Allocator>::construct(alloc_, insert_pos + i, value);
        }

        end_ += count;
        return insert_pos;
    }

    template <typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last,
                    typename enable_if<!is_integral<InputIt>::value>::type* = nullptr)
    {
        auto offset = pos - begin_;
        for (; first != last; ++first)
        {
            pos = insert(pos, *first);
            ++pos;
        }
        return begin_ + offset;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> init)
    {
        return insert(pos, init.begin(), init.end());
    }

    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args)
    {
        auto offset = pos - begin_;

        if (end_ == capacity_)
        {
            reallocate(calculate_growth(size() + 1));
        }

        iterator insert_pos = begin_ + offset;

        if (insert_pos < end_)
        {
            // 移动最后一个元素 / Move last element
            allocator_traits<Allocator>::construct(alloc_, end_, mystl::move(*(end_ - 1)));

            // 移动其他元素 / Move other elements
            std::move_backward(insert_pos, end_ - 1, end_);

            // 销毁并重新构造 / Destroy and reconstruct
            allocator_traits<Allocator>::destroy(alloc_, insert_pos);
        }

        allocator_traits<Allocator>::construct(alloc_, insert_pos, mystl::forward<Args>(args)...);
        ++end_;

        return insert_pos;
    }

    iterator erase(const_iterator pos)
    {
        MYSTL_DEBUG_ASSERT(pos >= begin_ && pos < end_, "vector::erase: iterator out of range");

        iterator erase_pos = const_cast<iterator>(pos);
        std::move(erase_pos + 1, end_, erase_pos);
        allocator_traits<Allocator>::destroy(alloc_, --end_);

        return erase_pos;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        MYSTL_DEBUG_ASSERT(first >= begin_ && last <= end_ && first <= last,
                          "vector::erase: invalid iterator range");

        if (first == last) return const_cast<iterator>(first);

        iterator erase_first = const_cast<iterator>(first);
        iterator erase_last = const_cast<iterator>(last);

        iterator new_end = std::move(erase_last, end_, erase_first);
        destroy_range(new_end, end_);
        end_ = new_end;

        return erase_first;
    }

    void push_back(const T& value)
    {
        emplace_back(value);
    }

    void push_back(T&& value)
    {
        emplace_back(mystl::move(value));
    }

    template <typename... Args>
    reference emplace_back(Args&&... args)
    {
        if (end_ == capacity_)
        {
            reallocate(calculate_growth(size() + 1));
        }

        allocator_traits<Allocator>::construct(alloc_, end_, mystl::forward<Args>(args)...);
        return *end_++;
    }

    void pop_back()
    {
        MYSTL_DEBUG_ASSERT(!empty(), "vector::pop_back: container is empty");
        allocator_traits<Allocator>::destroy(alloc_, --end_);
    }

    void resize(size_type count)
    {
        resize(count, T{});
    }

    void resize(size_type count, const T& value)
    {
        if (count < size())
        {
            destroy_range(begin_ + count, end_);
            end_ = begin_ + count;
        }
        else if (count > size())
        {
            reserve(count);
            for (size_type i = size(); i < count; ++i)
            {
                push_back(value);
            }
        }
    }

    void swap(vector& other) noexcept
    {
        std::swap(begin_, other.begin_);
        std::swap(end_, other.end_);
        std::swap(capacity_, other.capacity_);
        std::swap(alloc_, other.alloc_);
    }
};

// 非成员函数 / Non-member Functions
template <typename T, typename Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
{
    return !(rhs < lhs);
}

template <typename T, typename Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
{
    return rhs < lhs;
}

template <typename T, typename Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <typename T, typename Alloc>
void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) noexcept
{
    lhs.swap(rhs);
}

MYSTL_NAMESPACE_END

#endif // MYSTL_CONTAINERS_VECTOR_H_
