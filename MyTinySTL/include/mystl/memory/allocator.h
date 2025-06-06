#ifndef MYSTL_MEMORY_ALLOCATOR_H_
#define MYSTL_MEMORY_ALLOCATOR_H_

// MyTinySTL 内存分配器
// Memory allocator for MyTinySTL

#include "../core/config.h"
#include "../core/exceptions.h"
#include "../core/type_traits.h"
#include <new>
#include <cstdlib>

MYSTL_NAMESPACE_BEGIN

// 前向声明 / Forward Declarations
template <typename T>
class allocator;

// 分配器特征 / Allocator Traits
template <typename Alloc>
struct allocator_traits
{
    using allocator_type = Alloc;
    using value_type = typename Alloc::value_type;
    using pointer = typename Alloc::pointer;
    using const_pointer = typename Alloc::const_pointer;
    using reference = typename Alloc::reference;
    using const_reference = typename Alloc::const_reference;
    using size_type = typename Alloc::size_type;
    using difference_type = typename Alloc::difference_type;

    // 分配内存 / Allocate Memory
    static pointer allocate(allocator_type& alloc, size_type n)
    {
        return alloc.allocate(n);
    }

    static pointer allocate(allocator_type& alloc, size_type n, const void* hint)
    {
        return alloc.allocate(n, hint);
    }

    // 释放内存 / Deallocate Memory
    static void deallocate(allocator_type& alloc, pointer p, size_type n)
    {
        alloc.deallocate(p, n);
    }

    // 构造对象 / Construct Object
    template <typename T, typename... Args>
    static void construct(allocator_type& alloc, T* p, Args&&... args)
    {
        alloc.construct(p, mystl::forward<Args>(args)...);
    }

    // 析构对象 / Destroy Object
    template <typename T>
    static void destroy(allocator_type& alloc, T* p)
    {
        alloc.destroy(p);
    }

    // 最大分配大小 / Maximum Allocation Size
    static size_type max_size(const allocator_type& alloc) noexcept
    {
        return alloc.max_size();
    }

    // 复制构造时选择分配器 / Select allocator on container copy construction
    static allocator_type select_on_container_copy_construction(const allocator_type& alloc)
    {
        return alloc;
    }
};

// 默认分配器 / Default Allocator
template <typename T>
class allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    // 重绑定 / Rebind
    template <typename U>
    struct rebind
    {
        using other = allocator<U>;
    };

    // 构造函数 / Constructors
    allocator() noexcept = default;
    allocator(const allocator&) noexcept = default;
    
    template <typename U>
    allocator(const allocator<U>&) noexcept {}

    // 析构函数 / Destructor
    ~allocator() = default;

    // 赋值操作符 / Assignment Operators
    allocator& operator=(const allocator&) = default;

    // 分配内存 / Allocate Memory
    pointer allocate(size_type n)
    {
        MYSTL_THROW_LENGTH_ERROR_IF(n > max_size(), 
            "allocator<T>::allocate(size_t n): n exceeds maximum supported size");
        
        if (n == 0) return nullptr;
        
        pointer result = static_cast<pointer>(std::malloc(n * sizeof(T)));
        MYSTL_THROW_RUNTIME_ERROR_IF(result == nullptr, 
            "allocator<T>::allocate(size_t n): memory allocation failed");
        
        return result;
    }

    pointer allocate(size_type n, const void* /*hint*/)
    {
        return allocate(n);
    }

    // 释放内存 / Deallocate Memory
    void deallocate(pointer p, size_type /*n*/) noexcept
    {
        if (p != nullptr)
        {
            std::free(p);
        }
    }

    // 构造对象 / Construct Object
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args)
    {
        ::new (static_cast<void*>(p)) U(mystl::forward<Args>(args)...);
    }

    // 析构对象 / Destroy Object
    template <typename U>
    void destroy(U* p)
    {
        if (p != nullptr)
        {
            p->~U();
        }
    }

    // 最大分配大小 / Maximum Allocation Size
    size_type max_size() const noexcept
    {
        return static_cast<size_type>(-1) / sizeof(T);
    }

    // 获取对象地址 / Get Object Address
    pointer address(reference x) const noexcept
    {
        return &x;
    }

    const_pointer address(const_reference x) const noexcept
    {
        return &x;
    }
};

// 分配器比较 / Allocator Comparison
template <typename T1, typename T2>
bool operator==(const allocator<T1>&, const allocator<T2>&) noexcept
{
    return true;
}

template <typename T1, typename T2>
bool operator!=(const allocator<T1>&, const allocator<T2>&) noexcept
{
    return false;
}

// 内存操作辅助函数 / Memory Operation Helpers

// 获取对象地址 / Get Object Address
template <typename T>
constexpr T* addressof(T& obj) noexcept
{
    return &obj;
}

// 未初始化内存操作 / Uninitialized Memory Operations
template <typename ForwardIt, typename T>
void uninitialized_fill(ForwardIt first, ForwardIt last, const T& value)
{
    using value_type = typename std::iterator_traits<ForwardIt>::value_type;
    ForwardIt current = first;
    
    try
    {
        for (; current != last; ++current)
        {
            ::new (static_cast<void*>(&*current)) value_type(value);
        }
    }
    catch (...)
    {
        for (; first != current; ++first)
        {
            first->~value_type();
        }
        throw;
    }
}

template <typename ForwardIt, typename Size, typename T>
ForwardIt uninitialized_fill_n(ForwardIt first, Size count, const T& value)
{
    using value_type = typename std::iterator_traits<ForwardIt>::value_type;
    ForwardIt current = first;
    
    try
    {
        for (Size i = 0; i < count; ++i, ++current)
        {
            ::new (static_cast<void*>(&*current)) value_type(value);
        }
        return current;
    }
    catch (...)
    {
        for (; first != current; ++first)
        {
            first->~value_type();
        }
        throw;
    }
}

MYSTL_NAMESPACE_END

#endif // MYSTL_MEMORY_ALLOCATOR_H_
