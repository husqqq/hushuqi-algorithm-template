@echo off
rem Combine chapters 0-15 and build the cover, TOC, page numbers, and PDF.
rem Usage: book\build.bat          (build PDF)
rem        book\build.bat tex      (build intermediate TeX only)
setlocal
set HERE=%~dp0
set HERE=%HERE:~0,-1%
cd /d "%HERE%\.."

set MD=%HERE%\hushuqi算法竞赛模板-合订.md

rem 1) Combine with the UTF-8 helper so Unicode filenames work on Windows.
python book\combine.py
rem Remove repeated chapter preamble sections; chapter 1 keeps the global preamble.
python book\strip_preamble.py
echo combined markdown ready

rem 2) Render
set PANDOC=pandoc "%MD%" --toc --toc-depth=3 --lua-filter="%HERE%\anchors.lua" --lua-filter="%HERE%\numberlines.lua" --include-in-header="%HERE%\head.tex" --include-before-body="%HERE%\cover.tex" -V documentclass=ctexart -V geometry:a4paper -V geometry:margin=2cm -V colorlinks=true -V linkcolor=teal -V urlcolor=blue -V toccolor=black --highlight-style="%HERE%\jblight.theme"

if "%~1"=="tex" (
    %PANDOC% -s -o "%HERE%\hushuqi算法竞赛模板.tex"
    echo TEX generated
) else (
    %PANDOC% --pdf-engine=xelatex -o "%HERE%\hushuqi算法竞赛模板.pdf"
    echo PDF generated
    python book\build_volumes.py
)
endlocal
