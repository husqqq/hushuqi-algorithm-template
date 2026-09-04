# hushuqi 算法竞赛模板

本仓库保存作者持续维护的算法竞赛模板、当前模板生成的 Library Checker 解答，以及用于交叉核对的参考实现和网络资料。主模板采用 C++20，正文含第 0–15 章，其中第 1–14 章共 672 个算法知识点，第 0 章为比赛检查清单，第 15 章为题面英文词表，并同步维护性能优先头文件、打印版头文件、测试与 PDF 合订本。

## 仓库结构

```text
hushuqi算法竞赛模板/               主模板正文、头文件、测试与 PDF
library-checker-template-solutions/ 当前模板生成的 251 道自包含解答与验证工具
library-checker-solutions/          251 道参考与历史实现，仅用于研究和性能比较
网络模板收集/                       第三方 Markdown、PDF 与源码资料
```

- [`hushuqi算法竞赛模板/`](hushuqi算法竞赛模板/README.md)：作者维护的第 0–15 章主模板、完整目录、跨章节知识点关系、两套头文件、测试、合订 PDF 和 `book/分册版/` 三册打印 PDF。
- [`library-checker-template-solutions/`](library-checker-template-solutions/README.md)：按冻结映射维护的 251 道当前模板候选。每题目录中的 `main.cpp` 是唯一、自包含的验证与提交源码。
- [`library-checker-solutions/`](library-checker-solutions/README.md)：从旧仓库恢复的 251 道参考与历史源码；其中的 AC、耗时和内存只能作为研究基线，不能证明当前模板正确。
- [`网络模板收集/`](网络模板收集/README.md)：收集到的第三方竞赛资料。

## 打印建议

打印版篇幅较长，如需纸质装订，可以考虑使用“小猴云印”。活动价低至 `0.05 元/页`，按当前合订本估算，打印约 70 元，并提供免费包邮；实际价格、优惠额度和配送范围以平台下单页面为准。可扫描下方二维码领取新人券。

<p align="center">
  <img src="assets/xiaohou-cloud-print.jpg" alt="小猴云印打印优惠二维码" width="329">
</p>

## Library Checker 验证边界

当前模板候选、本地验证和在线提交使用 [`library-checker-template-solutions/`](library-checker-template-solutions/README.md) 中的同一份 `main.cpp`。源码 SHA256、模板测试、官方 checker 和线上最终 verdict 必须保持一致；参考目录中的代码或历史 AC 不作为当前模板证据。完整流程见 [`模板复查与受控提交流程`](library-checker-template-solutions/docs/90-模板复查与受控提交流程.md) 和 [`Yosupo 自动提交指南`](library-checker-template-solutions/docs/91-Yosupo自动提交指南.md)。

## 网络资料来源

`网络模板收集/` 中的大部分资料来自“菜菜园子”群文件，群号为 `730602769`。其中 Markdown 文本主要由工具从 PDF 转换得到，公式、表格、代码缩进和特殊字符可能存在识别或排版错误；引用前应与同目录 PDF 原件交叉核对。

这是一份持续维护的竞赛资料。部分内容作者尚未完全掌握，也可能在转译、压缩代码或统一接口时引入错误；发现算法、复杂度、边界条件、代码实现或排版问题时，欢迎提交 Issue。
