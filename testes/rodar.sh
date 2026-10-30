#!/bin/sh
# Roda todos os testes do TP1 no Git Bash. Uso: bash testes/rodar.sh [nome]
# Toda a logica fica em testes/rodar.cpp (o mesmo programa usado pelo rodar.bat).
cd "$(dirname "$0")/.." || exit 1
mkdir -p testes/bin
g++ -std=c++11 -Wall -Wextra -pedantic testes/rodar.cpp -o testes/bin/rodar.exe || exit 1
testes/bin/rodar.exe "$@"
