@echo off
rem 一键：合订 14 章 -> 生成带封面 / 目录 / 自动页码的 PDF。
rem 用法： book\build.bat          (默认出 PDF)
rem        book\build.bat tex      (只出中间 .tex，快，便于排查)
setlocal
set HERE=%~dp0
set HERE=%HERE:~0,-1%
cd /d "%HERE%\.."

set MD=%HERE%\hushuqi算法竞赛模板-合订.md

rem 1) 合订：按章号顺序拼接，并在章间补空行，避免章标题粘到上一章末行
break > "%MD%"
type 01-*.md >> "%MD%"
echo.>> "%MD%"
type 02-*.md >> "%MD%"
echo.>> "%MD%"
type 03-*.md >> "%MD%"
echo.>> "%MD%"
type 04-*.md >> "%MD%"
echo.>> "%MD%"
type 05-*.md >> "%MD%"
echo.>> "%MD%"
type 06-*.md >> "%MD%"
echo.>> "%MD%"
type 07-*.md >> "%MD%"
echo.>> "%MD%"
type 08-*.md >> "%MD%"
echo.>> "%MD%"
type 09-*.md >> "%MD%"
echo.>> "%MD%"
type 10-*.md >> "%MD%"
echo.>> "%MD%"
type 11-*.md >> "%MD%"
echo.>> "%MD%"
type 12-*.md >> "%MD%"
echo.>> "%MD%"
type 13-*.md >> "%MD%"
echo.>> "%MD%"
type 14-*.md >> "%MD%"
echo.>> "%MD%"
rem 剔除各章「本章公共前导」冗余小节（ch01 全局前导已覆盖全书）
python "%HERE%\strip_preamble.py" "%MD%"
echo combined -^> %MD%

rem 2) 渲染
set PANDOC=pandoc "%MD%" --toc --toc-depth=3 --lua-filter="%HERE%\anchors.lua" --lua-filter="%HERE%\numberlines.lua" --include-in-header="%HERE%\head.tex" --include-before-body="%HERE%\cover.tex" -V documentclass=ctexart -V geometry:a4paper -V geometry:margin=2cm -V colorlinks=true -V linkcolor=teal -V urlcolor=blue -V toccolor=black --highlight-style="%HERE%\jblight.theme"

if "%~1"=="tex" (
    %PANDOC% -s -o "%HERE%\hushuqi算法竞赛模板.tex"
    echo TEX -^> %HERE%\hushuqi算法竞赛模板.tex
) else (
    %PANDOC% --pdf-engine=xelatex -o "%HERE%\hushuqi算法竞赛模板.pdf"
    echo PDF -^> %HERE%\hushuqi算法竞赛模板.pdf
)
endlocal
