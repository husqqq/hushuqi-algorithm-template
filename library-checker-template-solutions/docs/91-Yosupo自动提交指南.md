# Yosupo 自动提交指南

收到章节编号时，第一步固定为从 `LC_TEMPLATE_MAPPING.tsv` 按两位章节号取全集，例如第 04 章运行 `rg "^04\t" library-checker-template-solutions/LC_TEMPLATE_MAPPING.tsv`。当前冻结题集没有模板缺口；不得用 manifest、已有目录或历史提交缩小范围。运行 `python library-checker-template-solutions/tools/refresh_untrusted_inventory.py` 后，若仍有缺口，可在 `不可完全置信清单/chapter-NN.md` 查看尚缺映射、当前源码或当前正确性证据的项目；零缺口时该目录不存在。

本目录的每道题均按 `<分类>/<题目名>/main.cpp` 保存。每个 `main.cpp` 都应当是由用户当前模板生成的唯一、自包含源码，已经包含所需算法和输入输出；它不得引用本地 `.hpp` 或仓库内其他源码。这个来源不能只靠 `meta.json` 声明，必须先按 [`90-模板复查与受控提交流程.md`](90-模板复查与受控提交流程.md) 完成确定性生成证明或两次独立人工复核。并列的 `../library-checker-solutions/` 是恢复的参考与历史源码目录，只能用于研究和性能比较，不能成为提交候选，也不能提供当前模板的验证结果。

2026-08-16 的 33 份旧源码曾以提交编号 `393990` 至 `394024` 取得最终 `AC`，但那些当前 `main.cpp` 已经变化且尚未重新在线提交。旧编号和成绩只在各题 `meta.json` 中作为历史索引保留，不能称为那些当前源码在线 AC；第 03 章固定 commit 的 47 份 `data_structure/` 当前源码均已生成，实际本地验证覆盖和性能门、当前 `online_ac` 的逐题状态见 `03-数据结构审计.md` 与默认收据，不能由候选数推断。目录中不保存旧源码、旧收据或中间副本。

## 源码约束

验证和提交始终使用题目目录中的同一个 `main.cpp`：

1. `main.cpp` 必须是完整翻译单元，只能引用尖括号系统头，禁止出现本地双引号 `include`。
2. `manifest.json`、逐题 `meta.json`、本地验证收据和服务端回显都核对这一个文件的 SHA256。
3. 脚本不得临时拼接仓库头文件，也不得从中间目录、历史归档或参考目录选择另一份源码。
4. 模板更新后，应同步替换对应题目的自包含 `main.cpp`，随后重新完成本地验证和在线提交；旧哈希与旧 verdict 自动失效。

## 准备

以下 PowerShell 命令均从工作区根目录 `template/` 执行，避免产生重复嵌套目录。

安装 Python 3.10 或更高版本、支持 GNU++20 的 `g++`，然后安装 Playwright：

```powershell
python -m pip install playwright
python -m playwright install chromium
```

脚本不会读取配置文件中的账号密码。提交时先无头检查本目录 `.local/yosupo-profile/` 中的既有登录态；会话有效时不会打开窗口，也不会要求按回车。只有没有有效会话、令牌即将过期或读取失败时，才会打开一个可见的 Chromium 窗口让用户完成一次登录；登录后立即重新无头检查。浏览器状态保存在该固定 profile，已被 `.gitignore` 排除，后续单题提交复用它而不重复登录。

账号和密码不得发送到聊天、写进脚本、README、Issue 或日志。需要在线提交时只能由用户本人在脚本打开的可见浏览器里手动输入；自动化只读取登录完成后的短期令牌。

本地验证还需要 [Library Checker Problems](https://github.com/yosupo06/library-checker-problems) 官方题库。题库放在本目录已忽略的 `.local/library-checker-problems/`，并切到 `manifest.json` 的 `official_repository.commit`：

```powershell
git clone https://github.com/yosupo06/library-checker-problems library-checker-template-solutions/.local/library-checker-problems
git -C library-checker-template-solutions/.local/library-checker-problems checkout 04c8de378bab67be926325de2871f0babb8e6451
git -C library-checker-template-solutions/.local/library-checker-problems sparse-checkout set big_integer common convolution data_structure enumerative_combinatorics geo graph linear_algebra number_theory other polynomial sample set_power_series string tree
```

`library-checker-template-solutions/.local/` 是工作区唯一的 `.local/`。其中的题库工作树、`yosupo-profile/` 浏览器状态和 `yosupo-local-verification.json` 收据都服务于本目录的独立 `main.cpp`，不应移动到参考目录或主模板目录，也不包含任何提交源码。官方题库是验证依赖，不是第三棵题解目录。

官方题库工作树展开全部官方问题分类和 `common/` 验证依赖；验证器从固定 commit 读取题目，并在系统临时目录组装 checker 所需文件，不会把官方源码复制进题解目录。每次复查任何章节前，先按 `LC_TEMPLATE_MAPPING.tsv` 冻结本章题集并运行映射校验器；再用 `git ls-tree -d --name-only <commit>:<category>` 反向核对主责分类全集与 `93-其他分类审计.md` 唯一分配，确保 TSV 没有漏题。`sample` 也包含真实官方题。当前 manifest 的分类或题数不能替代该审计，某章候选为零只表示该章生成工作尚未完成。

## 查看待提交源码

主候选以外的一题多实现证据使用 `variants.json`，不会替换主 `manifest.json`。生成与核对命令为：

```powershell
python library-checker-template-solutions/tools/generate_variants.py --check
python library-checker-template-solutions/tools/generate_cpp_variants.py --check
python library-checker-template-solutions/tools/verify_variants.py --only <variant_id>
```

取得当前变体收据后，严格一次提交一个变体并等待终态：

```powershell
python library-checker-template-solutions/tools/submit_variants.py --only <variant_id>
```

变体标识、源码哈希、模板条目和线上结果以 `variants.json` 及各 `variants/<variant_id>/meta.json` 为准；完整结果与不纳入理由见 `97-多实现在线验证审计.md`。

以下命令会读取清单中的当前 `main.cpp`，检查 UTF-8 无 BOM、LF 换行和无本地双引号 `include`，并打印文件大小与源码 SHA256；它不会登录或发送请求：

```powershell
python library-checker-template-solutions/tools/submit_yosupo.py --dry-run
```

只查看指定题目：

```powershell
python library-checker-template-solutions/tools/submit_yosupo.py --dry-run `
  --only exp_of_formal_power_series,log_of_formal_power_series
```

需要人工检查提交内容时，直接打开该题目录中的 `main.cpp`。脚本不会生成或导出另一份“最终源码”；磁盘上的这一个文件就是验证和提交对象。

提交器和验证器的单元测试可独立运行，但它们不代替题目级本地验证：

```powershell
python library-checker-template-solutions/tools/tests/test_submit_yosupo.py
python library-checker-template-solutions/tools/tests/test_verify_local.py
```

## 本地验证

在线提交前必须先运行 `verify_local.py`。下列“全量”只指当前 manifest 的全部候选；它不检查任何尚无候选目录的官方题，因此必须先完成流程文档规定的本章官方分类反向覆盖核对。命令为：

```powershell
python library-checker-template-solutions/tools/verify_local.py --all
```

只验证指定题目时使用 `--only`，多个题目名用逗号分隔：

```powershell
python library-checker-template-solutions/tools/verify_local.py `
  --only exp_of_formal_power_series,log_of_formal_power_series
```

验证器直接对各题 `main.cpp` 执行以下步骤：

1. 交叉核对清单、逐题元数据、源码路径和当前源码 SHA256，并拒绝本地双引号 `include`。
2. 用 `g++ -std=gnu++20 -O2 -pipe -DONLINE_JUDGE` 编译并链接这份 `main.cpp`，同时运行对应的模板测试；Windows/MinGW 自动给候选、模板测试和官方验证辅助程序追加 `-Wl,--stack,1073741824`。Linux 和在线评测不传该 Windows 专用参数。
3. 运行固定样例、边界和固定种子的随机数据；程序输出交给对应题目的官方 checker 判定，不用参考 solver 的输出冒充模板结果。
4. 生成本目录 `.local/yosupo-local-verification.json`，以 `source_sha256`、`source_compile`、`source_checks` 和 `cases` 记录每题唯一源码哈希、编译、模板测试和题目用例结果，并绑定官方题库提交版本。

`--only` 验证成功时，只更新本次题目的记录；已有收据的清单、官方 commit 和编译配置一致时，其他题目的记录可以保留。`--receipt <路径>` 只可用于不会进入任何正式授权模式的临时诊断：相对路径以 `library-checker-template-solutions/` 为基准并限制在其 `.local/` 内，显式绝对路径只用于外部临时环境。进入 `direct_authorized` 或 `approval_pending` 前都必须不带 `--receipt` 重新验证，让唯一当前收据落在 `.local/yosupo-local-verification.json`；批次身份和后续提交都只能使用这份默认收据。`--problems-root` 使用同一相对路径安全规则。

收据只对记录的 `main.cpp` 和官方题库版本有效。修改源码、清单、测试集合或题库版本后，SHA256 不再匹配，必须重新验证。提交器在认证前逐题确认编译、模板测试、固定样例、边界和随机测试全部通过；缺少收据、字段不完整、测试记录重复或源码哈希过期都会直接拒绝提交。

第 03 章不得通过 PowerShell 的 `>`、`Tee-Object` 或文本管道保存性能、批次或结果 JSON。含中文路径的控制台文本可能因代码页发生重编码，从而改变规范化 JSON 和 SHA256；必须使用各 Python 工具的 `--output`（以及批次工具的 `--performance-file`）直接写入 `.local/`：

```powershell
python library-checker-template-solutions/tools/benchmark_ch03.py `
  --only <problem> --output .local/ch03-performance/<problem>.json

python library-checker-template-solutions/tools/ch03_batch_report.py `
  --only <problem> `
  --performance-file .local/ch03-performance/<problem>.json `
  --output .local/ch03-batches/<problem>.json

Copy-Item library-checker-template-solutions/data_structure/<problem>/meta.json `
  library-checker-template-solutions/.local/ch03-pre-meta/<problem>.json

python library-checker-template-solutions/tools/ch03_result_report.py `
  --batch library-checker-template-solutions/.local/ch03-batches/<problem>.json `
  --pre-meta library-checker-template-solutions/.local/ch03-pre-meta/<problem>.json `
  --only <problem> --output .local/ch03-results/<problem>.json

python library-checker-template-solutions/tools/submit_yosupo.py --submit `
  --only <problem> `
  --batch .local/ch03-batches/<problem>.json `
  --jobs 1
```

## 提交

本节只说明脚本能力；实际线上操作还必须满足《模板复查与受控提交流程》的章节覆盖、模板订正、来源门、批次摘要和授权模式。默认无需逐批征求用户同意：题目完成真实源码级映射，参考实现审视中发现的模板问题已订正，当前模板来源证明、SHA256、本地收据、模板与题目测试、官方 checker、性能结论以及工具和题库版本门禁全部通过并形成报告后，进入 `direct_authorized` 并直接逐题提交。只有用户明确要求本章、本批或本题提交前确认时才进入 `approval_pending`，并等待用户批准当前批次 SHA256。缺少技术门禁不能进入任何可提交状态。

提交前，脚本依次完成以下检查：

1. 只从清单选择 `library-checker-template-solutions` 中的当前题目，拒绝参考源码和旧题解。
2. 确认 `main.cpp` 自包含且没有本地双引号 `include`，并交叉核对清单、逐题元数据和实际文件的唯一源码 SHA256。
3. 读取 `.local/yosupo-local-verification.json`，核对官方题库版本、源码 SHA256、编译记录、模板测试及 sample、boundary、random 三类 checker 记录。
4. 在系统临时目录中再次完整编译同一份 `main.cpp`；临时可执行文件会在预检结束后自动删除。
5. 全部本地检查通过后才无头检查受控浏览器认证，并固定使用 Library Checker 的 `cpp20` 语言。仅在会话失效时打开一次登录窗口；不得按题目创建新的 profile 或重复要求登录。认证完成后、创建提交线程前和每次发送前都会重新计算 `main.cpp` 的 SHA256，认证期间源码、清单或测试集合变化时拒绝发送旧快照。
6. 将 `main.cpp` 原文发送给 `POST /submit`。成功取得编号后输出 `SUBMITTED`，随后轮询 `GET /submissions/{id}`，直到取得最终 verdict 或超时。
7. 核对提交编号、题目名、题目标题、语言和服务端回显源码 SHA256，再把 verdict、时间、内存和源码哈希原子写入该题 `meta.json`。

脚本参数层面接受 `--only` 或 `--all`，但当前脚本没有经过验证的 fail-fast 批量模式。当前 manifest、验证器和提交器按裸题名选择；冻结 commit 的 251 个官方题名已核对为全局唯一。题库 commit 变化后必须重新枚举所有分类并检查重名；若出现重名，必须停在流程文档定义的 `tool_gap`，升级选择链后才能继续，禁止猜测分类。受控流程规定每次线上命令只能提交一个已经通过全部门禁且授权模式允许发送的题目：

```powershell
python library-checker-template-solutions/tools/submit_yosupo.py `
  --submit --only exp_of_formal_power_series --jobs 1
```

同一章节可以连续推进多题，但每个提交批次严格只含一道题，并使用一条命令；章节级审批要求也必须逐题生成批次 SHA256 和逐题批准。只有上一题取得可认证的最终 AC、结果写回逐题元数据和覆盖矩阵并生成最终单题报告后，才能为下一题生成新批次。任一题非 AC、状态未知、源码哈希不符或工具异常时立即停止。即使一个章节全部题目都允许提交，线上阶段也禁止使用逗号分隔的多题 `--only` 或 `--all`；`--jobs 1` 只表示串行，本身不保证遇到首个失败就停止。

可以调整本地编译和在线评测超时：

```powershell
python library-checker-template-solutions/tools/submit_yosupo.py --submit `
  --only exp_of_formal_power_series `
  --compile-timeout 180 --judge-timeout 1800 --poll-interval 3
```

`SUBMITTED` 只表示服务器已经返回提交编号，不能当作 AC。只有同时满足以下条件时，当前 `main.cpp` 才能记录为 `online_ac`：

- 最终 verdict 为 `AC`；
- `is_latest` 为真，即使用的是当前测试数据；
- 服务端回显源码 SHA256 与本次 `main.cpp` 的 SHA256 相同；
- 评测结束时磁盘上的 `main.cpp` 仍与本次提交相同。

如果评测期间源码发生变化，本次提交仍可作为一次历史尝试记录，但不能认证为当前模板通过。已有 `online_ac` 时，也只有题目、语言、提交编号、最新测试标记、服务端源码和当前 `main.cpp` SHA256 全部一致，脚本才可以跳过相同提交。非 AC、轮询超时、身份字段不匹配或源码哈希不一致都会使脚本以非零状态退出。

若已经取得提交编号后轮询超时，保留该编号并继续查询同一次提交。若请求可能已经送达服务器但客户端没有取得编号，状态记为 `submission_unknown`，立即停止整个批次，并按题目、发送时间窗口和源码 SHA256 查询近期提交；状态查清前不得重交。仍无法确认而确需重试时，必须先报告歧义和重复提交风险，再取得用户对“该批次该题再提交一次”的明确批准。

第 03 章的 47 份 `data_structure/` 当前 `main.cpp` 均已按模板来源门生成；是否通过本地验证只能以默认收据和 `03-数据结构审计.md` 的当前行状态判定。具体性能门与线上状态由该审计实时生成；在所有行的性能门与线上状态均满足要求前，不能称为“所有数据结构题已提交”。第 04 至第 08 章的当前候选为零同样只表示流程尚未执行：冻结 commit 下对应主责分类至少有 113 题，另须计入 `93-其他分类审计.md` 分配给各章的 `other` 题，不能据此称为没有 LC 题或章节复查完成。其余现有当前 `main.cpp` 若尚未重新在线提交，旧提交只作为 `meta.json` 中的历史索引，不得用于跳过当前源码，也不得写成当前模板 AC。

## 隐私与清理

- 不要把账号、密码、Firebase ID Token、Cookie 或 `.local/` 提交到 Git，也不要把它们发送到聊天中。
- 不要在脚本、README、Issue 或提交日志中粘贴个人凭据；账号密码只在用户可见的登录页面中手动输入。
- ID Token 只保存在脚本进程内存中；结束脚本后不会写入仓库。
- 如果浏览器登录状态不再需要，可手动删除本目录 `.local/yosupo-profile/`。
- 编译产物、临时源码和测试输出只放在系统临时目录，并在任务结束后清理；`.local/` 保留官方题库工作树、浏览器状态、当前验证收据和当前性能/批次/结果证据，不得建立任何提交源码副本目录。已结束命令的日志、一次性登录辅助脚本和 `__pycache__/` 可以在确认无活进程后删除。
