# hushuqi 算法竞赛模板

这是一份以 Markdown 为阅读与复制入口的 C++20 算法竞赛模板。全册含第 0–15 章，其中第 1–14 章共 671 个算法知识点，第 0 章为比赛检查清单，第 15 章为题面英文词表；代码、接口说明、复杂度和快速检查均收录在各章正文中，`include/` 保留可独立编译的实现，`tests/` 保留固定样例和随机对拍。

## 阅读入口

- [第 0 章：比赛注意](00-比赛注意.md)

- [全册目录](目录.md)
- [PDF 合订本](book/hushuqi算法竞赛模板.pdf)
- [跨章节知识点关系与选型](知识点关系.md)
- [第 1 章：基础与工程](01-基础与工程.md)
- [第 2 章：基础算法与离线算法](02-基础算法与离线算法.md)
- [第 3 章：数据结构](03-数据结构.md)
- [第 4 章：字符串](04-字符串.md)
- [第 5 章：图论](05-图论.md)
- [第 6 章：树论与动态树](06-树论与动态树.md)
- [第 7 章：数论](07-数论.md)
- [第 8 章：组合数学](08-组合数学.md)
- [第 9 章：线性代数与数值算法](09-线性代数与数值算法.md)
- [第 10 章：多项式、变换与线性递推](10-多项式、变换与线性递推.md)
- [第 11 章：计算几何](11-计算几何.md)
- [第 12 章：动态规划与优化](12-动态规划与优化.md)
- [第 13 章：博弈论、概率与杂项数学](13-博弈论、概率与杂项数学.md)
- [第 14 章：其他实用模板](14-其他实用模板.md)
- [第 15 章：ACM 常用英文单词](15-ACM常用英文单词.md)
- [分册打印版（上、中、下）](book/分册版/)

## 目录结构

- `00-*.md` 至 `15-*.md`：分章正文与比赛辅助资料。
- `目录.md`：全册目录与知识点编号。
- `知识点关系.md`：跨章节依赖、选型和学习路线。
- `include/full/`：性能优先版，按知识点编号划分的完整头文件。
- `include/print/`：打印版，与 Markdown/PDF 使用相同的普通 `int` 写法；`support/` 保存其独立公共依赖。
- `include/*.hpp`：性能优先版使用的跨条目聚合接口。
- `tests/*.cpp`：C++ 固定测试与随机对拍。
- `tests/*.py`：Python 条目测试。
- `f_cmp.py`：与对拍脚本放在同一目录使用，按题面 `|x-y| <= eps * max(1,y)` 比较两份浮点 token；比较器无标准输出，退出码表示通过或失败。
- `book/`：PDF 合订本及其构建脚本。

## 使用约定

- C++20，默认 $0$ 下标和左闭右开区间。
- 通用模板使用 `#include <bits/stdc++.h>`、`using namespace std;` 和 `#define int long long`。
- 需要明确位宽时使用 `int32_t`、`long long`、`unsigned long long` 或 `__int128`。
- 长模板附有固定种子测试；正文中的期望输出来自真实编译运行。

## 测试与构建

每个 C++ 测试都可在模板根目录独立编译，例如：

```powershell
g++ -std=c++20 -O2 .\tests\test_ch05_flow.cpp -o .\tests\test_ch05_flow.exe
.\tests\test_ch05_flow.exe

python .\tests\test_python_topics.py
```

PDF 的依赖与生成命令见 [`book/构建说明.md`](book/构建说明.md)。中间 Markdown、TeX、LaTeX 缓存、可执行文件和本地构建目录由顶层 `.gitignore` 排除。

运行 `python tools/generate_print_headers.py` 可从性能版重建打印头并同步章节中的整数类型；`python tools/generate_print_headers.py --check` 用于检查两者是否同步。打印版只允许整数类型、直接相关转换和存储布局与性能版不同，算法与公开接口保持一致。

运行 `python tools/check_print_variants.py` 可独立编译两套知识点头；运行 `python tools/check_test_variants.py` 可在系统临时目录中分别编译、运行两套头文件对应的全部非 benchmark C++ 测试。

运行 `python tools/check_non_lc_test_coverage.py` 可检查每个含 C++ 实现的知识点头是否至少有 Library Checker 映射，或被一份带断言的本地行为测试直接引用；纯文档索引单独列入白名单。
