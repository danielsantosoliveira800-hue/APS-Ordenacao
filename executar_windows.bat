
@echo off
setlocal

REM Entra na pasta onde este script esta localizado.
cd /d "%~dp0"

echo ========================================
echo          APS-Ordenacao
echo ========================================
echo.

REM 1. Verifica as ferramentas necessarias.
where cmake >nul 2>&1
if errorlevel 1 (
    echo Erro: CMake nao encontrado no PATH.
    echo Instale o CMake e habilite sua inclusao no PATH.
    goto erro
)

where gcc >nul 2>&1
if errorlevel 1 (
    echo Erro: GCC nao encontrado no PATH.
    echo Instale o GCC, por exemplo pelo MSYS2 MinGW-w64.
    goto erro
)

where ninja >nul 2>&1
if errorlevel 1 (
    echo Erro: Ninja nao encontrado no PATH.
    echo Instale o Ninja e habilite sua inclusao no PATH.
    goto erro
)

REM 2. Verifica o Python.
where python >nul 2>&1
if errorlevel 1 (
    echo Erro: Python nao encontrado no PATH.
    echo Instale o Python 3 e habilite sua inclusao no PATH.
    goto erro
)

echo Verificando ferramentas...
cmake --version
gcc --version
ninja --version
python --version

REM 3. Verifica os arquivos necessarios.
if not exist "requirements.txt" (
    echo Erro: requirements.txt nao encontrado.
    goto erro
)

if not exist "CMakeLists.txt" (
    echo Erro: CMakeLists.txt nao encontrado.
    goto erro
)

REM 4. Cria um ambiente virtual Python isolado.
echo.
echo [1/4] Preparando ambiente Python...

if not exist ".venv\Scripts\python.exe" (
    python -m venv .venv
    if errorlevel 1 (
        echo Erro ao criar o ambiente virtual.
        goto erro
    )
)

call ".venv\Scripts\activate.bat"
if errorlevel 1 (
    echo Erro ao ativar o ambiente virtual.
    goto erro
)

python -m pip install -r requirements.txt
if errorlevel 1 (
    echo Erro ao instalar as dependencias Python.
    goto erro
)

REM 5. Configura o projeto com CMake.
echo.
echo [2/4] Configurando o projeto...

cmake -S . -B build -G Ninja
if errorlevel 1 (
    echo.
    echo Erro na configuracao do CMake.
    echo Se houver conflito com uma compilacao anterior,
    echo feche o programa e remova a pasta build antes de tentar novamente.
    goto erro
)

REM 6. Compila o programa em C.
echo.
echo [3/4] Compilando o programa...

cmake --build build
if errorlevel 1 (
    echo Erro durante a compilacao.
    goto erro
)

REM 7. Executa o programa.
echo.
echo [4/4] Iniciando o programa...
echo.

if not exist "build\APS_Ordenacao.exe" (
    echo Erro: executavel nao encontrado em build.
    echo Verifique o CMakeLists.txt e o local de saida do executavel.
    goto erro
)

del /q "data\.benchmark_concluido" 2>nul
"build\APS_Ordenacao.exe"

REM 8. Gera os graficos somente se houve um benchmark.
if not exist "data\.benchmark_concluido" goto programa_encerrado

echo.
echo Benchmark detectado. Gerando graficos de desempenho...

python scripts\gerar_graficos.py
if errorlevel 1 (
    echo Erro ao gerar os graficos.
    del /q "data\.benchmark_concluido" 2>nul
    goto erro
)

REM Remove o sinal para evitar regeneracao em execucoes futuras.
del /q "data\.benchmark_concluido" 2>nul

echo.
echo Graficos de desempenho atualizados.

:programa_encerrado
echo.
echo Programa encerrado.
goto fim

:erro
echo.
echo A execucao foi interrompida devido a um erro.
pause
exit /b 1

:fim
echo.
pause
endlocal
