#ifndef MYSTL_CORE_EXCEPTIONS_H_
#define MYSTL_CORE_EXCEPTIONS_H_

// MyTinySTL 异常处理
// Exception handling for MyTinySTL

#include "config.h"

#ifdef MYSTL_USE_EXCEPTIONS
    #include <stdexcept>
    #include <string>
#endif

MYSTL_NAMESPACE_BEGIN

#ifdef MYSTL_USE_EXCEPTIONS

// 自定义异常类 / Custom Exception Classes
class mystl_exception : public std::exception
{
public:
    explicit mystl_exception(const char* message) : message_(message) {}
    explicit mystl_exception(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override
    {
        return message_.c_str();
    }

private:
    std::string message_;
};

class length_error : public mystl_exception
{
public:
    explicit length_error(const char* message) : mystl_exception(message) {}
    explicit length_error(const std::string& message) : mystl_exception(message) {}
};

class out_of_range : public mystl_exception
{
public:
    explicit out_of_range(const char* message) : mystl_exception(message) {}
    explicit out_of_range(const std::string& message) : mystl_exception(message) {}
};

class invalid_argument : public mystl_exception
{
public:
    explicit invalid_argument(const char* message) : mystl_exception(message) {}
    explicit invalid_argument(const std::string& message) : mystl_exception(message) {}
};

class runtime_error : public mystl_exception
{
public:
    explicit runtime_error(const char* message) : mystl_exception(message) {}
    explicit runtime_error(const std::string& message) : mystl_exception(message) {}
};

// 异常抛出宏 / Exception Throwing Macros
#define MYSTL_THROW_LENGTH_ERROR_IF(condition, message) \
    do { if (MYSTL_UNLIKELY(condition)) throw mystl::length_error(message); } while(0)

#define MYSTL_THROW_OUT_OF_RANGE_IF(condition, message) \
    do { if (MYSTL_UNLIKELY(condition)) throw mystl::out_of_range(message); } while(0)

#define MYSTL_THROW_INVALID_ARGUMENT_IF(condition, message) \
    do { if (MYSTL_UNLIKELY(condition)) throw mystl::invalid_argument(message); } while(0)

#define MYSTL_THROW_RUNTIME_ERROR_IF(condition, message) \
    do { if (MYSTL_UNLIKELY(condition)) throw mystl::runtime_error(message); } while(0)

#else

// 无异常模式下的宏定义 / Macros for no-exception mode
#define MYSTL_THROW_LENGTH_ERROR_IF(condition, message) \
    do { (void)(condition); (void)(message); } while(0)

#define MYSTL_THROW_OUT_OF_RANGE_IF(condition, message) \
    do { (void)(condition); (void)(message); } while(0)

#define MYSTL_THROW_INVALID_ARGUMENT_IF(condition, message) \
    do { (void)(condition); (void)(message); } while(0)

#define MYSTL_THROW_RUNTIME_ERROR_IF(condition, message) \
    do { (void)(condition); (void)(message); } while(0)

#endif // MYSTL_USE_EXCEPTIONS

// 调试断言 / Debug Assertions
#ifdef MYSTL_DEBUG_MODE
    #include <cassert>
    #define MYSTL_DEBUG_ASSERT(condition, message) \
        do { if (MYSTL_UNLIKELY(!(condition))) { \
            assert((condition) && (message)); \
        } } while(0)
#else
    #define MYSTL_DEBUG_ASSERT(condition, message) \
        do { (void)(condition); (void)(message); } while(0)
#endif

// 通用断言宏 / General Assertion Macros
#define MYSTL_ASSERT(condition) MYSTL_DEBUG_ASSERT(condition, #condition)

MYSTL_NAMESPACE_END

#endif // MYSTL_CORE_EXCEPTIONS_H_
