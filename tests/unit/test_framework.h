#ifndef MYSTL_TEST_FRAMEWORK_H_
#define MYSTL_TEST_FRAMEWORK_H_

// MyTinySTL 测试框架
// Test framework for MyTinySTL

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace mystl
{
namespace test
{

// 测试结果 / Test Result
struct TestResult
{
    std::string test_name;
    bool passed;
    std::string error_message;
    std::chrono::microseconds duration;
    
    TestResult(const std::string& name, bool pass, const std::string& msg = "", 
               std::chrono::microseconds dur = std::chrono::microseconds(0))
        : test_name(name), passed(pass), error_message(msg), duration(dur) {}
};

// 测试套件 / Test Suite
class TestSuite
{
private:
    std::string suite_name_;
    std::vector<TestResult> results_;
    size_t total_tests_;
    size_t passed_tests_;
    size_t failed_tests_;

public:
    explicit TestSuite(const std::string& name) 
        : suite_name_(name), total_tests_(0), passed_tests_(0), failed_tests_(0) {}

    // 运行单个测试 / Run Single Test
    template <typename TestFunc>
    void run_test(const std::string& test_name, TestFunc&& test_func)
    {
        std::cout << "Running test: " << test_name << " ... ";
        
        auto start_time = std::chrono::high_resolution_clock::now();
        bool test_passed = false;
        std::string error_msg;
        
        try
        {
            test_func();
            test_passed = true;
            std::cout << "\033[32mPASSED\033[0m" << std::endl;
        }
        catch (const std::exception& e)
        {
            error_msg = e.what();
            std::cout << "\033[31mFAILED\033[0m - " << error_msg << std::endl;
        }
        catch (...)
        {
            error_msg = "Unknown exception";
            std::cout << "\033[31mFAILED\033[0m - " << error_msg << std::endl;
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        
        results_.emplace_back(test_name, test_passed, error_msg, duration);
        ++total_tests_;
        
        if (test_passed)
            ++passed_tests_;
        else
            ++failed_tests_;
    }

    // 打印测试报告 / Print Test Report
    void print_report() const
    {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "Test Suite: " << suite_name_ << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        std::cout << "Total Tests: " << total_tests_ << std::endl;
        std::cout << "\033[32mPassed: " << passed_tests_ << "\033[0m" << std::endl;
        std::cout << "\033[31mFailed: " << failed_tests_ << "\033[0m" << std::endl;
        
        if (failed_tests_ > 0)
        {
            std::cout << "\nFailed Tests:" << std::endl;
            for (const auto& result : results_)
            {
                if (!result.passed)
                {
                    std::cout << "  - " << result.test_name << ": " << result.error_message << std::endl;
                }
            }
        }
        
        // 性能统计 / Performance Statistics
        if (!results_.empty())
        {
            auto total_duration = std::chrono::microseconds(0);
            for (const auto& result : results_)
            {
                total_duration += result.duration;
            }
            
            std::cout << "\nPerformance:" << std::endl;
            std::cout << "  Total Time: " << total_duration.count() << " μs" << std::endl;
            std::cout << "  Average Time: " << (total_duration.count() / total_tests_) << " μs per test" << std::endl;
        }
        
        std::cout << std::string(60, '=') << std::endl;
    }

    // 获取测试统计 / Get Test Statistics
    bool all_passed() const { return failed_tests_ == 0; }
    size_t total_count() const { return total_tests_; }
    size_t passed_count() const { return passed_tests_; }
    size_t failed_count() const { return failed_tests_; }
};

// 断言宏 / Assertion Macros
#define MYSTL_ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #condition << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define MYSTL_ASSERT_FALSE(condition) \
    MYSTL_ASSERT_TRUE(!(condition))

#define MYSTL_ASSERT_EQ(expected, actual) \
    do { \
        if (!((expected) == (actual))) { \
            std::ostringstream oss; \
            oss << "Assertion failed: expected " << (expected) << ", got " << (actual) \
                << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define MYSTL_ASSERT_NE(expected, actual) \
    do { \
        if ((expected) == (actual)) { \
            std::ostringstream oss; \
            oss << "Assertion failed: expected " << (expected) << " != " << (actual) \
                << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define MYSTL_ASSERT_LT(left, right) \
    do { \
        if (!((left) < (right))) { \
            std::ostringstream oss; \
            oss << "Assertion failed: expected " << (left) << " < " << (right) \
                << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define MYSTL_ASSERT_LE(left, right) \
    do { \
        if (!((left) <= (right))) { \
            std::ostringstream oss; \
            oss << "Assertion failed: expected " << (left) << " <= " << (right) \
                << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define MYSTL_ASSERT_GT(left, right) \
    do { \
        if (!((left) > (right))) { \
            std::ostringstream oss; \
            oss << "Assertion failed: expected " << (left) << " > " << (right) \
                << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define MYSTL_ASSERT_GE(left, right) \
    do { \
        if (!((left) >= (right))) { \
            std::ostringstream oss; \
            oss << "Assertion failed: expected " << (left) << " >= " << (right) \
                << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define MYSTL_ASSERT_THROW(statement, exception_type) \
    do { \
        bool caught_expected = false; \
        try { \
            statement; \
        } catch (const exception_type&) { \
            caught_expected = true; \
        } catch (...) { \
            std::ostringstream oss; \
            oss << "Assertion failed: expected " << #exception_type << " but caught different exception" \
                << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
        if (!caught_expected) { \
            std::ostringstream oss; \
            oss << "Assertion failed: expected " << #exception_type << " but no exception was thrown" \
                << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define MYSTL_ASSERT_NO_THROW(statement) \
    do { \
        try { \
            statement; \
        } catch (...) { \
            std::ostringstream oss; \
            oss << "Assertion failed: expected no exception but one was thrown" \
                << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

// 测试运行器 / Test Runner
class TestRunner
{
private:
    std::vector<TestSuite> suites_;

public:
    void add_suite(const TestSuite& suite)
    {
        suites_.push_back(suite);
    }

    void run_all()
    {
        size_t total_tests = 0;
        size_t total_passed = 0;
        size_t total_failed = 0;

        for (auto& suite : suites_)
        {
            suite.print_report();
            total_tests += suite.total_count();
            total_passed += suite.passed_count();
            total_failed += suite.failed_count();
        }

        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "OVERALL RESULTS" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "Total Test Suites: " << suites_.size() << std::endl;
        std::cout << "Total Tests: " << total_tests << std::endl;
        std::cout << "\033[32mTotal Passed: " << total_passed << "\033[0m" << std::endl;
        std::cout << "\033[31mTotal Failed: " << total_failed << "\033[0m" << std::endl;
        
        if (total_failed == 0)
        {
            std::cout << "\n\033[32mALL TESTS PASSED!\033[0m" << std::endl;
        }
        else
        {
            std::cout << "\n\033[31mSOME TESTS FAILED!\033[0m" << std::endl;
        }
        std::cout << std::string(60, '=') << std::endl;
    }

    bool all_passed() const
    {
        for (const auto& suite : suites_)
        {
            if (!suite.all_passed())
                return false;
        }
        return true;
    }
};

} // namespace test
} // namespace mystl

#endif // MYSTL_TEST_FRAMEWORK_H_
