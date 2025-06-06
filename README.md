MyTinySTL - Refactored Edition
=====
[![Build Status](https://travis-ci.org/Alinshans/MyTinySTL.svg?branch=master)](https://travis-ci.org/Alinshans/MyTinySTL) [![Build Status](https://ci.appveyor.com/api/projects/status/github/Alinshans/MyTinySTL?branch=master&svg=true)](https://ci.appveyor.com/project/Alinshans/mytinystl) [![Release](https://img.shields.io/github/release/Alinshans/MyTinySTL.svg)](https://github.com/Alinshans/MyTinySTL/releases) [![License](https://img.shields.io/badge/License-MIT%20License-blue.svg)](https://opensource.org/licenses/MIT) [![Chat](https://img.shields.io/badge/chat-on%20gitter-FF6EB4.svg)](https://gitter.im/alinshans/MyTinySTL)

## 项目简介 / Introduction
   这是一个基于 `C++11` 标准的轻量级 STL 实现，经过全面重构以提高代码质量、可维护性和测试覆盖率。本项目实现了 STL 中的主要容器、算法和工具类，采用现代 C++ 设计模式，具有良好的异常安全保证和性能优化。

   This is a lightweight STL implementation based on `C++11` standard, comprehensively refactored to improve code quality, maintainability, and test coverage. The project implements major STL containers, algorithms, and utilities with modern C++ design patterns, providing strong exception safety guarantees and performance optimizations.

## 重构特性 / Refactoring Features

### 🏗️ 架构改进 / Architecture Improvements
- **模块化设计**：重新组织代码结构，按功能模块分类
- **现代C++设计**：采用现代C++11特性和最佳实践
- **异常安全**：提供强异常安全保证
- **性能优化**：针对常见操作进行性能优化

### 📁 新目录结构 / New Directory Structure
```
MyTinySTL/
├── include/mystl/           # 头文件库
│   ├── core/               # 核心组件
│   ├── containers/         # 容器类
│   ├── algorithms/         # 算法
│   ├── iterators/          # 迭代器
│   ├── memory/             # 内存管理
│   └── utilities/          # 工具类
├── tests/                  # 测试代码
│   ├── unit/              # 单元测试
│   ├── integration/       # 集成测试
│   └── performance/       # 性能测试
├── docs/                   # 文档
└── examples/              # 示例代码
```

### 🧪 测试框架 / Testing Framework
- **现代测试框架**：自定义轻量级测试框架
- **全面覆盖**：单元测试、集成测试、性能测试
- **持续集成**：支持CI/CD流水线

## 系统要求 / System Requirements

### 支持的操作系统 / Supported Operating Systems
- **Linux** (Ubuntu 16.04+, CentOS 7+)
- **Windows** (Windows 10+)
- **macOS** (10.12+)

### 编译器要求 / Compiler Requirements
- **GCC** 5.0+ (推荐 7.0+)
- **Clang** 3.5+ (推荐 6.0+)
- **MSVC** 19.0+ (Visual Studio 2015+, 推荐 2019+)

### 构建工具 / Build Tools
- **CMake** 3.10+ (必需)
- **Git** (用于克隆仓库)

## 快速开始 / Quick Start

### 1. 克隆仓库 / Clone Repository
```bash
git clone https://github.com/milianks/MyTinySTL.git
cd MyTinySTL
```

### 2. 构建项目 / Build Project
```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译
cmake --build . --parallel

# 运行测试
ctest --output-on-failure
```

### 3. 安装 / Installation
```bash
# 安装到系统目录
sudo cmake --install .

# 或安装到自定义目录
cmake --install . --prefix /path/to/install
```

## 使用方法 / Usage

### 基本使用 / Basic Usage
```cpp
#include <mystl/containers/vector.h>
#include <mystl/algorithms/algorithm.h>
#include <iostream>

int main() {
    // 创建vector
    mystl::vector<int> vec{1, 2, 3, 4, 5};

    // 添加元素
    vec.push_back(6);

    // 遍历元素
    for (const auto& item : vec) {
        std::cout << item << " ";
    }

    return 0;
}
```

### CMake集成 / CMake Integration
```cmake
find_package(mystl REQUIRED)
target_link_libraries(your_target mystl::mystl)
```

## 测试 / Testing

### 运行所有测试 / Run All Tests
```bash
cd build
ctest --verbose
```

### 运行特定测试 / Run Specific Tests
```bash
# 运行vector测试
./test_vector

# 运行性能测试
./performance_tests
```

### 代码覆盖率 / Code Coverage
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DMYSTL_ENABLE_COVERAGE=ON
make
ctest
gcov *.gcno
```

---

## Introduction

This is a `tinySTL` based on `C++11`, which is my first project for practice. I use the Chinese documents and annotations for convenience, maybe there will be an English version later, but now I have no time to do that yet. Now I have released version `2.0.0`. I have achieved the vast majority of the containers and functions of `STL`, and there may be some deficiencies and bugs. From the version `2.x.x`, the project will enter the stage of long-term maintenance, i.e., I probably will not add new content but only fix bugs found. If you find any bugs, please point out that in [`Issues`](https://github.com/Alinshans/MyTinySTL/issues), or make a [`Pull requests`](https://github.com/Alinshans/MyTinySTL/pulls) to improve it, thanks!

## Supported

* os
  * linux
  * windows
  * osx
* complier
  * g++ 5.4 or later
  * clang++ 3.5 or later
  * msvc 14.0 or later

## Required

* Use cmake 2.8 to build this project (**Optional**)

## Run test

If you want to run the test, please read [this](https://github.com/Alinshans/MyTinySTL/blob/master/Test/README.md) first.

* gcc/clang on linux/osx

1. git clone
```bash
$ git clone git@github.com:Alinshans/MyTinySTL.git
$ cd MyTinySTL
```
2. build and run
```bash
$ mkdir build && cd build
$ cmake ..
$ make
$ cd ../bin && ./stltest
```

* msvc on windows

1. git clone or [Download ZIP](https://github.com/Alinshans/MyTinySTL/archive/master.zip)
2. use `vs2015`(or `vs2017`) open the file `MSVC/MyTinySTL_VS2015.sln`, configured in `Release`, run this project(Ctrl + F5).

## Documents

See [Wiki](https://github.com/Alinshans/MyTinySTL/wiki).

## Test

See [Test](https://github.com/Alinshans/MyTinySTL/tree/master/Test).
