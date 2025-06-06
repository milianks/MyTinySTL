// MyTinySTL Vector 单元测试
// Unit tests for MyTinySTL Vector

#include "test_framework.h"
#include "../../MyTinySTL/include/mystl/containers/vector.h"
#include <vector>
#include <string>

using namespace mystl::test;

void test_vector_constructors()
{
    // 默认构造函数 / Default constructor
    mystl::vector<int> v1;
    MYSTL_ASSERT_TRUE(v1.empty());
    MYSTL_ASSERT_EQ(0, v1.size());

    // 填充构造函数 / Fill constructor
    mystl::vector<int> v2(5, 42);
    MYSTL_ASSERT_EQ(5, v2.size());
    for (size_t i = 0; i < v2.size(); ++i)
    {
        MYSTL_ASSERT_EQ(42, v2[i]);
    }

    // 范围构造函数 / Range constructor
    std::vector<int> std_vec = {1, 2, 3, 4, 5};
    mystl::vector<int> v3(std_vec.begin(), std_vec.end());
    MYSTL_ASSERT_EQ(5, v3.size());
    for (size_t i = 0; i < v3.size(); ++i)
    {
        MYSTL_ASSERT_EQ(std_vec[i], v3[i]);
    }

    // 复制构造函数 / Copy constructor
    mystl::vector<int> v4(v3);
    MYSTL_ASSERT_EQ(v3.size(), v4.size());
    for (size_t i = 0; i < v4.size(); ++i)
    {
        MYSTL_ASSERT_EQ(v3[i], v4[i]);
    }

    // 移动构造函数 / Move constructor
    mystl::vector<int> v5(std::move(v4));
    MYSTL_ASSERT_EQ(5, v5.size());
    MYSTL_ASSERT_TRUE(v4.empty());

    // 初始化列表构造函数 / Initializer list constructor
    mystl::vector<int> v6{1, 2, 3, 4, 5};
    MYSTL_ASSERT_EQ(5, v6.size());
    for (size_t i = 0; i < v6.size(); ++i)
    {
        MYSTL_ASSERT_EQ(static_cast<int>(i + 1), v6[i]);
    }
}

void test_vector_assignment()
{
    mystl::vector<int> v1{1, 2, 3};
    mystl::vector<int> v2;

    // 复制赋值 / Copy assignment
    v2 = v1;
    MYSTL_ASSERT_EQ(v1.size(), v2.size());
    for (size_t i = 0; i < v2.size(); ++i)
    {
        MYSTL_ASSERT_EQ(v1[i], v2[i]);
    }

    // 移动赋值 / Move assignment
    mystl::vector<int> v3;
    v3 = std::move(v2);
    MYSTL_ASSERT_EQ(3, v3.size());
    MYSTL_ASSERT_TRUE(v2.empty());

    // 初始化列表赋值 / Initializer list assignment
    v3 = {10, 20, 30, 40};
    MYSTL_ASSERT_EQ(4, v3.size());
    MYSTL_ASSERT_EQ(10, v3[0]);
    MYSTL_ASSERT_EQ(40, v3[3]);
}

void test_vector_element_access()
{
    mystl::vector<int> v{1, 2, 3, 4, 5};

    // operator[] 测试 / operator[] test
    MYSTL_ASSERT_EQ(1, v[0]);
    MYSTL_ASSERT_EQ(5, v[4]);

    // at() 测试 / at() test
    MYSTL_ASSERT_EQ(1, v.at(0));
    MYSTL_ASSERT_EQ(5, v.at(4));
    MYSTL_ASSERT_THROW(v.at(10), mystl::out_of_range);

    // front() 和 back() 测试 / front() and back() test
    MYSTL_ASSERT_EQ(1, v.front());
    MYSTL_ASSERT_EQ(5, v.back());

    // data() 测试 / data() test
    int* ptr = v.data();
    MYSTL_ASSERT_EQ(1, *ptr);
    MYSTL_ASSERT_EQ(2, *(ptr + 1));
}

void test_vector_iterators()
{
    mystl::vector<int> v{1, 2, 3, 4, 5};

    // 正向迭代器测试 / Forward iterator test
    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        MYSTL_ASSERT_EQ(expected++, *it);
    }

    // 常量迭代器测试 / Const iterator test
    const auto& cv = v;
    expected = 1;
    for (auto it = cv.cbegin(); it != cv.cend(); ++it)
    {
        MYSTL_ASSERT_EQ(expected++, *it);
    }

    // 反向迭代器测试 / Reverse iterator test
    expected = 5;
    for (auto it = v.rbegin(); it != v.rend(); ++it)
    {
        MYSTL_ASSERT_EQ(expected--, *it);
    }
}

void test_vector_capacity()
{
    mystl::vector<int> v;

    // 空容器测试 / Empty container test
    MYSTL_ASSERT_TRUE(v.empty());
    MYSTL_ASSERT_EQ(0, v.size());

    // 添加元素后测试 / Test after adding elements
    v.push_back(1);
    MYSTL_ASSERT_FALSE(v.empty());
    MYSTL_ASSERT_EQ(1, v.size());

    // reserve 测试 / reserve test
    v.reserve(100);
    MYSTL_ASSERT_GE(v.capacity(), 100);
    MYSTL_ASSERT_EQ(1, v.size());

    // shrink_to_fit 测试 / shrink_to_fit test
    v.shrink_to_fit();
    MYSTL_ASSERT_EQ(v.capacity(), v.size());
}

void test_vector_modifiers()
{
    mystl::vector<int> v;

    // push_back 测试 / push_back test
    v.push_back(1);
    v.push_back(2);
    MYSTL_ASSERT_EQ(2, v.size());
    MYSTL_ASSERT_EQ(1, v[0]);
    MYSTL_ASSERT_EQ(2, v[1]);

    // emplace_back 测试 / emplace_back test
    v.emplace_back(3);
    MYSTL_ASSERT_EQ(3, v.size());
    MYSTL_ASSERT_EQ(3, v[2]);

    // pop_back 测试 / pop_back test
    v.pop_back();
    MYSTL_ASSERT_EQ(2, v.size());

    // insert 测试 / insert test
    auto it = v.insert(v.begin() + 1, 10);
    MYSTL_ASSERT_EQ(3, v.size());
    MYSTL_ASSERT_EQ(10, *it);
    MYSTL_ASSERT_EQ(1, v[0]);
    MYSTL_ASSERT_EQ(10, v[1]);
    MYSTL_ASSERT_EQ(2, v[2]);

    // erase 测试 / erase test
    it = v.erase(v.begin() + 1);
    MYSTL_ASSERT_EQ(2, v.size());
    MYSTL_ASSERT_EQ(2, *it);

    // clear 测试 / clear test
    v.clear();
    MYSTL_ASSERT_TRUE(v.empty());
    MYSTL_ASSERT_EQ(0, v.size());
}

void test_vector_resize()
{
    mystl::vector<int> v{1, 2, 3};

    // 扩大容器 / Expand container
    v.resize(5, 42);
    MYSTL_ASSERT_EQ(5, v.size());
    MYSTL_ASSERT_EQ(1, v[0]);
    MYSTL_ASSERT_EQ(2, v[1]);
    MYSTL_ASSERT_EQ(3, v[2]);
    MYSTL_ASSERT_EQ(42, v[3]);
    MYSTL_ASSERT_EQ(42, v[4]);

    // 缩小容器 / Shrink container
    v.resize(2);
    MYSTL_ASSERT_EQ(2, v.size());
    MYSTL_ASSERT_EQ(1, v[0]);
    MYSTL_ASSERT_EQ(2, v[1]);
}

void test_vector_comparison()
{
    mystl::vector<int> v1{1, 2, 3};
    mystl::vector<int> v2{1, 2, 3};
    mystl::vector<int> v3{1, 2, 4};

    // 相等比较 / Equality comparison
    MYSTL_ASSERT_TRUE(v1 == v2);
    MYSTL_ASSERT_FALSE(v1 == v3);

    // 不等比较 / Inequality comparison
    MYSTL_ASSERT_FALSE(v1 != v2);
    MYSTL_ASSERT_TRUE(v1 != v3);

    // 字典序比较 / Lexicographical comparison
    MYSTL_ASSERT_TRUE(v1 < v3);
    MYSTL_ASSERT_FALSE(v3 < v1);
}

void test_vector_swap()
{
    mystl::vector<int> v1{1, 2, 3};
    mystl::vector<int> v2{4, 5, 6, 7};

    v1.swap(v2);

    MYSTL_ASSERT_EQ(4, v1.size());
    MYSTL_ASSERT_EQ(3, v2.size());
    MYSTL_ASSERT_EQ(4, v1[0]);
    MYSTL_ASSERT_EQ(1, v2[0]);
}

void run_vector_tests()
{
    TestSuite suite("Vector Tests");

    suite.run_test("Constructors", test_vector_constructors);
    suite.run_test("Assignment", test_vector_assignment);
    suite.run_test("Element Access", test_vector_element_access);
    suite.run_test("Iterators", test_vector_iterators);
    suite.run_test("Capacity", test_vector_capacity);
    suite.run_test("Modifiers", test_vector_modifiers);
    suite.run_test("Resize", test_vector_resize);
    suite.run_test("Comparison", test_vector_comparison);
    suite.run_test("Swap", test_vector_swap);

    suite.print_report();
}

int main()
{
    run_vector_tests();
    return 0;
}
