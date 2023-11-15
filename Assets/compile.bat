@echo off
g++ -o client.exe Client.cpp -lws2_32

if %ERRORLEVEL% NEQ 0 (
    echo Erro na compilacao!
    exit /b
)

echo Compilacao bem-sucedida!
.\client.exe
