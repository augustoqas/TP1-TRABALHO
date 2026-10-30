@echo off
rem Roda todos os testes do TP1. Uso, de qualquer pasta: testes\rodar.bat [nome]
rem (ex.: testes\rodar.bat senha). Toda a logica fica em testes\rodar.cpp.
setlocal
cd /d "%~dp0.."
where g++ >nul 2>nul || set "PATH=%PATH%;C:\MinGW\bin;%ProgramFiles%\CodeBlocks\MinGW\bin;%ProgramFiles(x86)%\CodeBlocks\MinGW\bin"
where g++ >nul 2>nul || (echo ERRO: g++ nao encontrado. Ponha a pasta bin do MinGW no PATH. & exit /b 1)
if not exist "testes\bin" mkdir "testes\bin"
g++ -std=c++11 -Wall -Wextra -pedantic testes\rodar.cpp -o testes\bin\rodar.exe || exit /b 1
testes\bin\rodar.exe %*
exit /b %errorlevel%
