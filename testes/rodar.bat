@echo off
rem Roda todos os testes do TP1. Uso, de qualquer pasta: testes\rodar.bat [nome]
rem (ex.: testes\rodar.bat senha). Toda a logica fica em testes\rodar.cpp.
rem Aberto por duplo clique, pausa no fim para a janela nao sumir; pelo terminal, nao.
setlocal
set "ESTE=%~f0"
set "CODIGO=1"
cd /d "%~dp0.."
where g++ >nul 2>nul || set "PATH=%PATH%;C:\MinGW\bin;%ProgramFiles%\CodeBlocks\MinGW\bin;%ProgramFiles(x86)%\CodeBlocks\MinGW\bin"
where g++ >nul 2>nul || (echo ERRO: g++ nao encontrado. Ponha a pasta bin do MinGW no PATH. & goto fim)
if not exist "testes\bin" mkdir "testes\bin"
g++ -std=c++11 -Wall -Wextra -pedantic testes\rodar.cpp -o testes\bin\rodar.exe || goto fim
testes\bin\rodar.exe %*
set "CODIGO=%errorlevel%"

:fim
rem Duplo clique = cmd /c com o caminho completo deste .bat no cmdcmdline. Expansao
rem atrasada (!) porque o cmdcmdline pode ter aspas e & que quebrariam o %...%.
setlocal EnableDelayedExpansion
set "CHAMADA=!cmdcmdline:"=!"
if /i not "!CHAMADA:%ESTE%=!"=="!CHAMADA!" pause
endlocal & exit /b %CODIGO%
