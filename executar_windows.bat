@echo off
setlocal

REM Entra na pasta onde este script esta localizado.
cd /d "%~dp0"

echo ========================================
echo          APS-Ordenacao
echo ========================================
echo.

REM ========================================
REM 1. CONFIGURACAO DO CAMINHO DO CMAKE
REM ========================================

set "CMAKE_EXE=C:\Program Files\JetBrains\CLion 2026.2.2\bin\cmake\win\x64\bin\cmake.exe"

if not exist "%CMAKE_EXE%" (
    echo Erro: CMake nao encontrado.
    echo Verifique se o CLion esta instalado no caminho configurado.
    pause
    exit /b 1
)

echo Verificando CMake...
"%CMAKE_EXE%" --version

if errorlevel 1 (
    echo Erro: nao foi possivel executar o CMake.
    pause
    exit /b 1
)

REM ========================================
REM 2. CONFIGURACAO DO COMPILADOR E NINJA
REM ========================================

set "GCC_EXE=C:\Program Files\JetBrains\CLion 2026.2.2\bin\mingw\bin\gcc.exe"
set "NINJA_EXE=C:\Program Files\JetBrains\CLion 2026.2.2\bin\ninja\win\x64\ninja.exe"

if not exist "%GCC_EXE%" (
    echo Erro: compilador GCC nao encontrado.
    echo Verifique a instalacao do CLion e do MinGW.
    pause
    exit /b 1
)

if not exist "%NINJA_EXE%" (
    echo Erro: Ninja nao encontrado.
    echo Verifique a instalacao do CLion.
    pause
    exit /b 1
)

REM ========================================
REM 3. VERIFICACAO DO PYTHON
REM ========================================

where python >nul 2>&1

if errorlevel 1 (
    echo Erro: Python nao encontrado.
    echo Instale o Python 3 e habilite sua inclusao no PATH.
    pause
    exit /b 1
)

echo.
echo Verificando Python...
python --version

REM ========================================
REM 4. VERIFICACAO DAS DEPENDENCIAS
REM ========================================

if not exist "requirements.txt" (
    echo Erro: requirements.txt nao encontrado.
    pause
    exit /b 1
)

echo.
echo [1/3] Instalando dependencias Python...

python -m pip install -r requirements.txt

if errorlevel 1 (
    echo Erro ao instalar as dependencias Python.
    pause
    exit /b 1
)

REM ========================================
REM 5. CONFIGURACAO DO PROJETO COM CMAKE
REM ========================================

echo.
echo [2/3] Configurando o projeto com CMake...

"%CMAKE_EXE%" -S . -B build -G Ninja -DCMAKE_C_COMPILER="%GCC_EXE%" -DCMAKE_MAKE_PROGRAM="%NINJA_EXE%"

if errorlevel 1 (
    echo Erro ao configurar o projeto.
    pause
    exit /b 1
)

REM ========================================
REM 6. COMPILACAO DO PROGRAMA EM C
REM ========================================

echo.
echo Compilando o programa...

"%CMAKE_EXE%" --build build

if errorlevel 1 (
    echo Erro ao compilar o projeto.
    pause
    exit /b 1
)

REM ========================================
REM 7. EXECUCAO DO PROGRAMA
REM ========================================

echo.
echo [3/3] Iniciando o programa...
echo.

if exist "build\APS_Ordenacao.exe" (
    "build\APS_Ordenacao.exe"
) else (
    echo Erro: executavel nao encontrado em build.
    pause
    exit /b 1
)

echo.
echo Programa encerrado.
pause

endlocal
