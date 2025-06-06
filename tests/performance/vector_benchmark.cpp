// MyTinySTL Vector 性能测试
// Performance benchmark for MyTinySTL Vector

#include "../unit/test_framework.h"
#include "../../MyTinySTL/include/mystl/containers/vector.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>

using namespace mystl::test;

class BenchmarkTimer
{
private:
    std::chrono::high_resolution_clock::time_point start_time_;
    std::string operation_name_;

public:
    explicit BenchmarkTimer(const std::string& name) : operation_name_(name)
    {
        start_time_ = std::chrono::high_resolution_clock::now();
    }

    ~BenchmarkTimer()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time_);
        std::cout << std::setw(30) << std::left << operation_name_ 
                  << ": " << std::setw(10) << std::right << duration.count() << " μs" << std::endl;
    }
};

template <typename VectorType>
void benchmark_push_back(const std::string& type_name, size_t count)
{
    std::cout << "\n=== " << type_name << " push_back benchmark ===" << std::endl;
    
    {
        BenchmarkTimer timer(type_name + " push_back");
        VectorType vec;
        for (size_t i = 0; i < count; ++i)
        {
            vec.push_back(static_cast<int>(i));
        }
    }
}

template <typename VectorType>
void benchmark_insert(const std::string& type_name, size_t count)
{
    std::cout << "\n=== " << type_name << " insert benchmark ===" << std::endl;
    
    VectorType vec;
    vec.reserve(count);
    
    {
        BenchmarkTimer timer(type_name + " insert at end");
        for (size_t i = 0; i < count; ++i)
        {
            vec.insert(vec.end(), static_cast<int>(i));
        }
    }
}

template <typename VectorType>
void benchmark_random_access(const std::string& type_name, size_t count)
{
    std::cout << "\n=== " << type_name << " random access benchmark ===" << std::endl;
    
    VectorType vec;
    for (size_t i = 0; i < count; ++i)
    {
        vec.push_back(static_cast<int>(i));
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, count - 1);
    
    volatile int sum = 0;  // 防止编译器优化
    {
        BenchmarkTimer timer(type_name + " random access");
        for (size_t i = 0; i < count; ++i)
        {
            sum += vec[dis(gen)];
        }
    }
}

template <typename VectorType>
void benchmark_iteration(const std::string& type_name, size_t count)
{
    std::cout << "\n=== " << type_name << " iteration benchmark ===" << std::endl;
    
    VectorType vec;
    for (size_t i = 0; i < count; ++i)
    {
        vec.push_back(static_cast<int>(i));
    }
    
    volatile int sum = 0;  // 防止编译器优化
    
    {
        BenchmarkTimer timer(type_name + " iterator");
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            sum += *it;
        }
    }
    
    {
        BenchmarkTimer timer(type_name + " range-based for");
        for (const auto& item : vec)
        {
            sum += item;
        }
    }
}

template <typename VectorType>
void benchmark_copy_operations(const std::string& type_name, size_t count)
{
    std::cout << "\n=== " << type_name << " copy operations benchmark ===" << std::endl;
    
    VectorType vec1;
    for (size_t i = 0; i < count; ++i)
    {
        vec1.push_back(static_cast<int>(i));
    }
    
    {
        BenchmarkTimer timer(type_name + " copy constructor");
        VectorType vec2(vec1);
        (void)vec2;  // 防止未使用警告
    }
    
    {
        BenchmarkTimer timer(type_name + " copy assignment");
        VectorType vec3;
        vec3 = vec1;
        (void)vec3;  // 防止未使用警告
    }
    
    {
        BenchmarkTimer timer(type_name + " move constructor");
        VectorType vec_temp(vec1);  // 创建临时对象
        VectorType vec4(std::move(vec_temp));
        (void)vec4;  // 防止未使用警告
    }
}

template <typename VectorType>
void benchmark_resize_operations(const std::string& type_name, size_t count)
{
    std::cout << "\n=== " << type_name << " resize operations benchmark ===" << std::endl;
    
    {
        BenchmarkTimer timer(type_name + " resize grow");
        VectorType vec;
        vec.resize(count, 42);
    }
    
    {
        BenchmarkTimer timer(type_name + " reserve");
        VectorType vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            vec.push_back(static_cast<int>(i));
        }
    }
}

void run_comprehensive_benchmark()
{
    const size_t small_count = 1000;
    const size_t medium_count = 100000;
    const size_t large_count = 1000000;
    
    std::cout << "MyTinySTL Vector Performance Benchmark" << std::endl;
    std::cout << "=======================================" << std::endl;
    
    // 小数据集测试
    std::cout << "\n--- Small Dataset (" << small_count << " elements) ---" << std::endl;
    benchmark_push_back<mystl::vector<int>>("MyTinySTL", small_count);
    benchmark_push_back<std::vector<int>>("std::vector", small_count);
    
    benchmark_random_access<mystl::vector<int>>("MyTinySTL", small_count);
    benchmark_random_access<std::vector<int>>("std::vector", small_count);
    
    // 中等数据集测试
    std::cout << "\n--- Medium Dataset (" << medium_count << " elements) ---" << std::endl;
    benchmark_push_back<mystl::vector<int>>("MyTinySTL", medium_count);
    benchmark_push_back<std::vector<int>>("std::vector", medium_count);
    
    benchmark_iteration<mystl::vector<int>>("MyTinySTL", medium_count);
    benchmark_iteration<std::vector<int>>("std::vector", medium_count);
    
    benchmark_copy_operations<mystl::vector<int>>("MyTinySTL", medium_count);
    benchmark_copy_operations<std::vector<int>>("std::vector", medium_count);
    
    // 大数据集测试
    std::cout << "\n--- Large Dataset (" << large_count << " elements) ---" << std::endl;
    benchmark_resize_operations<mystl::vector<int>>("MyTinySTL", large_count);
    benchmark_resize_operations<std::vector<int>>("std::vector", large_count);
    
    std::cout << "\nBenchmark completed!" << std::endl;
}

int main()
{
    try
    {
        run_comprehensive_benchmark();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Benchmark failed: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
