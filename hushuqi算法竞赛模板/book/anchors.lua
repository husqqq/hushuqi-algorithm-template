-- 给每个标题按其行首编号自动赋 id，让正文里的 (#71003) 这类交叉引用在
-- 合订 PDF 里真正跳得动（章节文件本身没有显式 {#anchor}）。
--   "# 7. 数论"          -> c7      章级加 c 前缀，避免 "11 章" 与 "1.1 节"→"11" 撞车
--   "## 7.1 整数运算..."  -> 71
--   "### 7.1.003 快速幂"  -> 71003   与全书 351 处 (#Nxyyy / #Nxxyyy) 引用一致
-- 没有行首编号的标题（如「本章公共前导」）保持 pandoc 自动 id。
function Header(el)
  local txt = pandoc.utils.stringify(el)
  local num = txt:match("^%s*([%d.]+)")
  if num then
    local id = num:gsub("%.", "")
    if id ~= "" then
      el.identifier = (el.level == 1) and ("c" .. id) or id
    end
  end
  return el
end
