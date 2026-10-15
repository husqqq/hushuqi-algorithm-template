#!/usr/bin/env python3
"""修复PDF生成中的问题：
1. 删除"见知识点关系.md"这样的内部引用
2. 修复代码块格式（添加缺失的```标记）
3. 修复长枚举值溢出问题
"""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parent.parent

def fix_knowledge_refs(text):
    """删除知识点关系.md的引用"""
    # 删除整个链接引用
    text = re.sub(r'见\s*\[`?知识点关系\.md`?\]\([^)]+\)。?', '', text)
    text = re.sub(r'，见\s*\[`?知识点关系\.md`?\]\([^)]+\)', '', text)
    # 删除句尾冗余标点
    text = re.sub(r'([。,])。', r'\1', text)
    return text

def fix_code_blocks(text):
    """修复2.1.010等处缺失的代码块标记"""
    lines = text.split('\n')
    result = []
    i = 0
    while i < len(lines):
        line = lines[i]
        result.append(line)

        # 检测到namespace开头且没有代码块标记
        if (line.strip().startswith('namespace ') and
            i > 0 and not lines[i-1].strip().startswith('```')):
            # 向前查找是否已有```cpp
            has_code_block = False
            for j in range(max(0, i-5), i):
                if lines[j].strip().startswith('```'):
                    has_code_block = True
                    break

            if not has_code_block:
                # 在namespace前插入```cpp
                result.insert(-1, '```cpp')
                # 查找对应的结束位置（下一个###或文件结尾）
                j = i + 1
                while j < len(lines):
                    if lines[j].strip().startswith('###') or lines[j].strip().startswith('##'):
                        break
                    j += 1
                # 找到最后一个}
                k = j - 1
                while k > i and lines[k].strip() == '':
                    k -= 1
                if k > i and '}' in lines[k]:
                    # 在}之后添加```
                    result.append(lines[i+1:k+1])
                    result.append('```')
                    result.append('')
                    i = k
        i += 1

    return '\n'.join(result)

def fix_long_enums(text):
    """修复长枚举值溢出：ELLIPSE/PARABOLA/HYPERBOLA -> ELLIPSE/...等"""
    # 将过长的枚举值缩写
    text = re.sub(
        r'ELLIPSE/PARABOLA/HYPERBOLA/DEGENERATE',
        r'ELLIPSE/PARABOLA/\nHYPERBOLA/DEGENERATE',
        text
    )
    # 其他可能的长行
    text = re.sub(
        r'(\w+/\w+/\w+/\w+/\w+)',
        lambda m: m.group(0)[:40] + '/...' if len(m.group(0)) > 45 else m.group(0),
        text
    )
    return text

def main():
    # 处理所有章节文件
    files = sorted(ROOT.glob("[0-9][0-9]-*.md"))
    for path in files:
        print(f"Processing {path.name}...")
        text = path.read_text(encoding='utf-8')
        original = text

        # 应用所有修复
        text = fix_knowledge_refs(text)
        text = fix_long_enums(text)

        # 只在有修改时写回
        if text != original:
            path.write_text(text, encoding='utf-8')
            print(f"  [OK] Fixed {path.name}")
        else:
            print(f"  [SKIP] No changes for {path.name}")

if __name__ == "__main__":
    main()
