@echo off
setlocal
python "%~dp0build.py" %*
exit /b %errorlevel%
