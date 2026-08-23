# 《C++ 的 pb_ds 库在 OI 中的应用》转换记录

- 原文件：`【于纪平】C++的pb_ds库在OI中的应用.pdf`
- 尝试方式：`pdftotext -layout -enc UTF-8`，未使用 OCR。
- 结果：无法可靠转换。PDF 使用的内嵌字体缺少可用的 Unicode 映射，直接提取后的中文大面积乱码；继续保留乱码会误导后续 AI，因此没有把乱码正文作为资料使用。
- 可辨认的章节主题：`priority_queue`、`tree`、`hash_table`。
- 替代整理：见 [PBDS 使用说明](./大模板代码/03-数据结构/3.9-GNU%20PBDS/PBDS使用说明.md)。该说明按 PPT 可辨认的主题组织，并明确区分可提交代码、策略说明与兼容性注意事项。

若以后需要还原幻灯片全文，应安装带 `chi_sim` 语言包的 OCR 工具，再逐页渲染并 OCR；当前环境没有可用的 Tesseract OCR。
