@echo off
if "%1"=="teste" (
    gcc tests\teste.c src\jogo.c -o teste.exe -Wall && .\teste.exe
    exit /b
)
gcc src\main.c src\jogo.c -o cata-estrelas.exe -O2 -Wall -mwindows -static-libgcc -lraylib
