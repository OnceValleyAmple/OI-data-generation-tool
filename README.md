# OI/ACM 数据生成库

一个功能强大的 C++ 数据生成库，专门为 OI/ACM 竞赛题目设计，支持生成各种随机数据结构、梯度测试数据，并包含特判程序模板。

## 📋 目录

- [功能特性](#功能特性)
- [快速开始](#快速开始)
- [安装与使用](#安装与使用)
- [API 文档](#api-文档)
- [示例代码](#示例代码)
- [文件结构](#文件结构)
- [贡献指南](#贡献指南)

## 功能特性

### 🎯 核心功能

- **多种随机分布**: 均匀分布、偏左/偏右分布、自定义分布
- **数据结构生成**: 随机树、图、森林、DAG、二分图等
- **梯度数据生成**: 支持多种递增策略生成渐进难度的测试数据
- **文件批量生成**: 自动生成多组输入输出文件
- **高精度计算**: 内置大整数类
- **内存管理工具**: 类型安全的清零/填充工具

### 📊 支持的随机结构

- **树结构**: 普通树、二叉树、星形树、链式树、森林
- **图结构**: 无向图、有向图、连通图、DAG、二分图
- **随机数据**: 整数、实数、字符串、数组、排列

## 快速开始

### 1. 克隆仓库

```bash
git clone https://github.com/yourusername/oi-data-generator.git
cd oi-data-generator
```

### 2. 基本使用示例

```cpp
#include "DataGenerator.h"
using namespace std;

int main() {
    // 生成随机整数（均匀分布）
    long long num = random(1, 100);
    
    // 生成随机树（10个节点）
    auto tree_edges = RandomTree.random_tree(10);
    
    // 生成随机图（10个节点，15条边）
    auto graph_edges = RandomGraph.random_graph(10, 15);
    
    return 0;
}
```

### 3. 生成测试数据

```cpp
// 批量生成20个测试文件
if (batchGenerateFiles(1, 20, solve, 1e5, ins, ous)) {
    cout << "数据文件生成成功\n";
}
```

## 安装与使用

### 环境要求

- C++17 或更高版本
- 支持标准库（STL）
- Windows/Linux/macOS

### 编译示例

```bash
# 编译测试程序
g++ -std=c++17 -o test test.cpp DataGenerator.cpp -O2

# 编译特判程序
g++ -std=c++17 -o spj spj.cpp -O2
```

## API 文档

### 随机数生成

```cpp
// 生成区间 [l, r] 内的随机整数
long long random(long long l, long long r, double opt = 1.0);

// 生成随机实数
double randomDouble(double l, double r, double opt = 1.0);

// 参数说明：
// opt > 1.0: 偏向大值
// opt = 1.0: 均匀分布
// opt < 1.0: 偏向小值
```

### 随机字符串

```cpp
// 生成指定长度的随机字符串
std::string randomString(int n, std::string chars, bool allowZero = false);

// 从字符串数组中随机选择
std::string randomString(std::vector<std::string> chars);
```

### 随机树生成

```cpp
// 生成随机树
std::vector<std::pair<int, int>> RandomTree.random_tree(
    int n,                    // 节点数
    int base = 1,            // 节点偏移量
    int root = 0,            // 根节点
    double rho = 2.0,        // 深度控制参数
    bool vertices_rand = true // 是否打乱节点
);

// 特殊树结构
RandomTree.random_tree_chain(n);     // 链
RandomTree.random_tree_star(n);      // 星形树
RandomTree.random_binary_tree(n);    // 二叉树
RandomTree.random_forest_tree(n, k); // 森林（k棵树）
```

### 随机图生成

```cpp
// 生成随机图
std::vector<std::pair<int, int>> RandomGraph.random_graph(
    int n, int m,                    // 节点数和边数
    bool connected = true,           // 是否连通
    bool repeated_edges = false,     // 是否允许重边
    bool self_rings = false,         // 是否允许自环
    bool directional = false,        // 是否有向
    int base = 1                     // 节点偏移
);

// 特殊图结构
RandomGraph.random_dag_graph(n, m);       // DAG
RandomGraph.random_binary_graph(n1, n2, m); // 二分图
```

### 梯度数据生成

```cpp
// 批量生成文件
bool batchGenerateFiles(
    int startIndex,     // 起始编号
    int endIndex,       // 结束编号
    std::function<void(long long, long long, double)> solve, // 生成逻辑
    long long Limit,    // 数据上限
    std::stringstream &ins, // 输入流
    std::stringstream &ous, // 输出流
    int multiple = 1,   // 每组数据重复次数
    GradientStrategy strategy = GradientStrategy::SIGMOID // 递增策略
);

// 递增策略：
// LINEAR, EXPONENTIAL, LOGARITHMIC, QUADRATIC, SQRT, SIGMOID, UNIFORM
```

## 示例代码

### 基础示例 (test.cpp)

```cpp
#include "DataGenerator.h"
using namespace std;

stringstream ins, ous;

void solve(int DATAL, int DATAR, double OFFSET) {
    int n = random(DATAL, DATAR, OFFSET);
    ins << n << '\n';
    
    for (int i = 0; i < n; i++) {
        int x = random(1, 1000);
        ins << x << (i == n-1 ? '\n' : ' ');
    }
    
    // 计算答案
    // ous << answer << '\n';
}

int main() {
    // 生成20个测试文件，数据规模上限1e5
    if (batchGenerateFiles(1, 20, solve, 1e5, ins, ous)) {
        cout << "生成成功！\n";
    }
    return 0;
}
```

### 特判程序 (spj.cpp)

```cpp
// 完整示例见 spj.cpp
// 支持多种评测结果：AC, WA, PE, PC, ERROR
```

## 文件结构

```text
oi-data-generator/
├── DataGenerator.h      # 主头文件（所有声明）
├── DataGenerator.cpp    # 实现文件
├── test.cpp            # 测试和示例代码
├── spj.cpp             # 特判程序模板
├── README.md           # 说明文档
└── CMakeLists.txt      # CMake构建文件（可选）
```

## 贡献指南

### 提交问题

1. 在 Issues 页面查看是否已有类似问题
2. 创建新的 Issue，描述清晰的问题或建议

### 提交代码

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建 Pull Request

### 代码规范

- 使用 C++17 标准
- 遵循 Google C++ 风格指南
- 添加适当的注释
- 确保向后兼容性

## 作者

**Ample** - 初始开发者

## 致谢

感谢所有为这个项目做出贡献的开发者！

---

## 📚 更多资源

### 学习资料

- [OI/ACM 竞赛指南](https://github.com/OI-wiki/OI-wiki)
- [CP-Algorithms](https://cp-algorithms.com/)
- [Codeforces 题目集](https://codeforces.com/problemset)

### 相关工具

- [Testlib](https://github.com/MikeMirzayanov/testlib) - 另一个流行的测试数据生成库
- [Polygon](https://polygon.codeforces.com/) - Codeforces 的题目准备系统

### 社区

- [Codeforces](https://codeforces.com/)
- [洛谷](https://www.luogu.com.cn/)
- [AtCoder](https://atcoder.jp/)

---

⭐ **如果这个项目对你有帮助，请点个 Star！** ⭐

有任何问题或建议，欢迎提交 Issue 或 Pull Request！
