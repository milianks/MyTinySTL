#ifndef MYSTL_CORE_CONFIG_H_
#define MYSTL_CORE_CONFIG_H_

// MyTinySTL 核心配置文件
// Core configuration file for MyTinySTL

#include <cstddef>
#include <climits>

// 版本信息 / Version Information
#define MYSTL_VERSION_MAJOR 3
#define MYSTL_VERSION_MINOR 0
#define MYSTL_VERSION_PATCH 0

// 编译器检测 / Compiler Detection
#if defined(_MSC_VER)
    #define MYSTL_MSVC
    #if _MSC_VER < 1900
        #error "MyTinySTL requires Visual Studio 2015 or later"
    #endif
#elif defined(__GNUC__)
    #define MYSTL_GCC
    #if __GNUC__ < 5
        #error "MyTinySTL requires GCC 5.0 or later"
    #endif
#elif defined(__clang__)
    #define MYSTL_CLANG
    #if __clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ < 5)
        #error "MyTinySTL requires Clang 3.5 or later"
    #endif
#endif

// C++标准检测 / C++ Standard Detection
#if __cplusplus < 201103L
    #error "MyTinySTL requires C++11 or later"
#endif

// 平台检测 / Platform Detection
#if defined(_WIN32) || defined(_WIN64)
    #define MYSTL_WINDOWS
#elif defined(__linux__)
    #define MYSTL_LINUX
#elif defined(__APPLE__)
    #define MYSTL_APPLE
#endif

// 调试模式配置 / Debug Mode Configuration
#ifndef NDEBUG
    #define MYSTL_DEBUG_MODE
#endif

// 异常处理配置 / Exception Handling Configuration
#ifndef MYSTL_NO_EXCEPTIONS
    #define MYSTL_USE_EXCEPTIONS
#endif

// 内联配置 / Inline Configuration
#if defined(MYSTL_MSVC)
    #define MYSTL_FORCE_INLINE __forceinline
#elif defined(MYSTL_GCC) || defined(MYSTL_CLANG)
    #define MYSTL_FORCE_INLINE __attribute__((always_inline)) inline
#else
    #define MYSTL_FORCE_INLINE inline
#endif

// 分支预测优化 / Branch Prediction Optimization
#if defined(MYSTL_GCC) || defined(MYSTL_CLANG)
    #define MYSTL_LIKELY(x)   __builtin_expect(!!(x), 1)
    #define MYSTL_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define MYSTL_LIKELY(x)   (x)
    #define MYSTL_UNLIKELY(x) (x)
#endif

// 内存对齐 / Memory Alignment
#if defined(MYSTL_MSVC)
    #define MYSTL_ALIGN(n) __declspec(align(n))
#elif defined(MYSTL_GCC) || defined(MYSTL_CLANG)
    #define MYSTL_ALIGN(n) __attribute__((aligned(n)))
#else
    #define MYSTL_ALIGN(n)
#endif

// 警告抑制 / Warning Suppression
#ifdef MYSTL_MSVC
    #pragma warning(push)
    #pragma warning(disable: 4244)  // conversion warnings
    #pragma warning(disable: 4996)  // deprecated function warnings
#endif

// 命名空间配置 / Namespace Configuration
#define MYSTL_NAMESPACE_BEGIN namespace mystl {
#define MYSTL_NAMESPACE_END   }

// 常用类型别名 / Common Type Aliases
MYSTL_NAMESPACE_BEGIN

using size_type = std::size_t;
using difference_type = std::ptrdiff_t;

// 默认容器大小配置 / Default Container Size Configuration
constexpr size_type default_buffer_size = 512;
constexpr size_type max_container_size = SIZE_MAX / 2;

MYSTL_NAMESPACE_END

#endif // MYSTL_CORE_CONFIG_H_
