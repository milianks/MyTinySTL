// MyTinySTL Vector 使用示例
// Example usage of MyTinySTL Vector

#include <iostream>
#include <string>
#include <algorithm>
#include "../MyTinySTL/include/mystl/containers/vector.h"

void basic_usage_example()
{
    std::cout << "=== 基本使用示例 / Basic Usage Example ===" << std::endl;
    
    // 创建vector / Create vector
    mystl::vector<int> vec;
    
    // 添加元素 / Add elements
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    
    std::cout << "Vector size: " << vec.size() << std::endl;
    std::cout << "Elements: ";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

void constructor_examples()
{
    std::cout << "\n=== 构造函数示例 / Constructor Examples ===" << std::endl;
    
    // 默认构造 / Default constructor
    mystl::vector<int> v1;
    std::cout << "Default constructor - size: " << v1.size() << std::endl;
    
    // 填充构造 / Fill constructor
    mystl::vector<int> v2(5, 42);
    std::cout << "Fill constructor (5, 42) - elements: ";
    for (const auto& item : v2)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    
    // 初始化列表构造 / Initializer list constructor
    mystl::vector<std::string> v3{"Hello", "World", "MyTinySTL"};
    std::cout << "Initializer list constructor - elements: ";
    for (const auto& item : v3)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    
    // 复制构造 / Copy constructor
    mystl::vector<std::string> v4(v3);
    std::cout << "Copy constructor - size: " << v4.size() << std::endl;
    
    // 移动构造 / Move constructor
    mystl::vector<std::string> v5(std::move(v4));
    std::cout << "Move constructor - v5 size: " << v5.size() 
              << ", v4 size: " << v4.size() << std::endl;
}

void iterator_examples()
{
    std::cout << "\n=== 迭代器示例 / Iterator Examples ===" << std::endl;
    
    mystl::vector<int> vec{10, 20, 30, 40, 50};
    
    // 正向迭代器 / Forward iterator
    std::cout << "Forward iteration: ";
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 反向迭代器 / Reverse iterator
    std::cout << "Reverse iteration: ";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 范围for循环 / Range-based for loop
    std::cout << "Range-based for: ";
    for (const auto& item : vec)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

void capacity_examples()
{
    std::cout << "\n=== 容量管理示例 / Capacity Management Examples ===" << std::endl;
    
    mystl::vector<int> vec;
    
    std::cout << "Initial - size: " << vec.size() 
              << ", capacity: " << vec.capacity() << std::endl;
    
    // 预留容量 / Reserve capacity
    vec.reserve(100);
    std::cout << "After reserve(100) - size: " << vec.size() 
              << ", capacity: " << vec.capacity() << std::endl;
    
    // 添加元素 / Add elements
    for (int i = 0; i < 10; ++i)
    {
        vec.push_back(i);
    }
    std::cout << "After adding 10 elements - size: " << vec.size() 
              << ", capacity: " << vec.capacity() << std::endl;
    
    // 调整大小 / Resize
    vec.resize(5);
    std::cout << "After resize(5) - size: " << vec.size() 
              << ", capacity: " << vec.capacity() << std::endl;
    
    // 收缩容量 / Shrink to fit
    vec.shrink_to_fit();
    std::cout << "After shrink_to_fit() - size: " << vec.size() 
              << ", capacity: " << vec.capacity() << std::endl;
}

void modification_examples()
{
    std::cout << "\n=== 修改操作示例 / Modification Examples ===" << std::endl;
    
    mystl::vector<int> vec{1, 2, 3, 4, 5};
    
    auto print_vec = [&vec]() {
        std::cout << "Vector: ";
        for (const auto& item : vec)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    };
    
    std::cout << "Initial vector:" << std::endl;
    print_vec();
    
    // 插入元素 / Insert element
    vec.insert(vec.begin() + 2, 99);
    std::cout << "After insert(begin()+2, 99):" << std::endl;
    print_vec();
    
    // 删除元素 / Erase element
    vec.erase(vec.begin() + 2);
    std::cout << "After erase(begin()+2):" << std::endl;
    print_vec();
    
    // 就地构造 / Emplace
    vec.emplace(vec.begin() + 1, 88);
    std::cout << "After emplace(begin()+1, 88):" << std::endl;
    print_vec();
    
    // 就地构造到末尾 / Emplace back
    vec.emplace_back(77);
    std::cout << "After emplace_back(77):" << std::endl;
    print_vec();
    
    // 删除末尾元素 / Pop back
    vec.pop_back();
    std::cout << "After pop_back():" << std::endl;
    print_vec();
}

void algorithm_compatibility_example()
{
    std::cout << "\n=== 算法兼容性示例 / Algorithm Compatibility Example ===" << std::endl;
    
    mystl::vector<int> vec{5, 2, 8, 1, 9, 3};
    
    std::cout << "Original vector: ";
    for (const auto& item : vec)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    
    // 使用标准库算法 / Use standard library algorithms
    std::sort(vec.begin(), vec.end());
    std::cout << "After std::sort: ";
    for (const auto& item : vec)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    
    // 查找元素 / Find element
    auto it = std::find(vec.begin(), vec.end(), 5);
    if (it != vec.end())
    {
        std::cout << "Found 5 at position: " << (it - vec.begin()) << std::endl;
    }
    
    // 反转 / Reverse
    std::reverse(vec.begin(), vec.end());
    std::cout << "After std::reverse: ";
    for (const auto& item : vec)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

int main()
{
    std::cout << "MyTinySTL Vector 使用示例" << std::endl;
    std::cout << "MyTinySTL Vector Usage Examples" << std::endl;
    std::cout << "================================" << std::endl;
    
    try
    {
        basic_usage_example();
        constructor_examples();
        iterator_examples();
        capacity_examples();
        modification_examples();
        algorithm_compatibility_example();
        
        std::cout << "\n所有示例运行成功！" << std::endl;
        std::cout << "All examples completed successfully!" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "错误 / Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
