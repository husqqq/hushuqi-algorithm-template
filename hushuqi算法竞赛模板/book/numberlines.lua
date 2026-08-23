-- 给每个代码块加 numberLines 类：合订本里所有代码块左侧显示行号（对标 WIDA / IDE 观感）。
function CodeBlock(el)
  el.classes:insert("numberLines")
  return el
end
