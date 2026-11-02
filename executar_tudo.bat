@echo off
setlocal

REM Executa tudo de uma vez: compila em Release, roda a coleta completa de
REM desempenho (sem usar o menu), valida o CSV e gera os graficos.

REM Entra na pasta onde este script esta localizado.
cd /d "%~dp0"

REM Usa as ferramentas instaladas no Windows e disponiveis no PATH.
REM Compila em uma pasta separada para nao reutilizar o cache antigo do CLion.
set "BUILD_DIR=build_autonomo"

echo ========================================
echo      APS-Ordenacao - coleta completa
echo ========================================
echo.
echo Esta execucao SOBRESCREVE data\resultados.csv e os graficos em
echo data\graficos\, e leva cerca de 15 minutos. Feche outros programas
echo pesados antes de continuar, para nao interferir nos tempos.
echo.
choice /c SN /m "Deseja continuar"
if errorlevel 2 goto cancelado

REM 1. Verifica as ferramentas necessarias.
for %%T in (cmake gcc ninja python) do (
    where %%T >nul 2>&1
    if errorlevel 1 (
        echo Erro: %%T nao encontrado no PATH.
        goto erro
    )
)

if not exist "requirements.txt" (
    echo Erro: requirements.txt nao encontrado.
    goto erro
)

REM 2. Prepara o ambiente Python isolado.
echo.
echo [1/6] Preparando ambiente Python...
if not exist ".venv\Scripts\python.exe" (
    python -m venv .venv
    if errorlevel 1 (
        echo Erro ao criar o ambiente virtual.
        goto erro
    )
)
call ".venv\Scripts\activate.bat"
python -m pip install -r requirements.txt
if errorlevel 1 (
    echo Erro ao instalar as dependencias Python.
    goto erro
)

REM 3. Configura e compila em Release (otimizacao -O3).
echo.
echo [2/6] Configurando o projeto (Release)...
cmake -S . -B "%BUILD_DIR%" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc
if errorlevel 1 (
    echo.
    echo Erro na configuracao do CMake.
    echo Se houver conflito com uma compilacao anterior,
    echo remova a pasta %BUILD_DIR% e tente novamente.
    goto erro
)

echo.
echo [3/6] Compilando o programa...
cmake --build "%BUILD_DIR%" --config Release
if errorlevel 1 (
    echo Erro durante a compilacao.
    goto erro
)

if not exist "%BUILD_DIR%\APS_Ordenacao.exe" (
    echo Erro: executavel nao encontrado em %BUILD_DIR%.
    goto erro
)

REM 4. Executa a coleta completa: opcao 1 (coleta) e opcao 3 (sair).
echo.
echo [4/6] Executando a coleta de desempenho. Aguarde...
(echo 1& echo 3)> "%TEMP%\aps_entrada.txt"
"%BUILD_DIR%\APS_Ordenacao.exe" < "%TEMP%\aps_entrada.txt"
set RESULTADO=%errorlevel%
del /q "%TEMP%\aps_entrada.txt" 2>nul
if not "%RESULTADO%"=="0" (
    echo Erro durante a coleta de desempenho.
    goto erro
)

REM 5. Valida o CSV gerado.
echo.
echo [5/6] Validando data\resultados.csv...
python scripts\validar_csv.py
if errorlevel 1 (
    echo Erro ao validar o CSV.
    goto erro
)

REM 6. Gera os graficos.
echo.
echo [6/6] Gerando os graficos...
python scripts\gerar_graficos.py
if errorlevel 1 (
    echo Erro ao gerar os graficos.
    goto erro
)

del /q "data\.benchmark_concluido" 2>nul

echo.
echo Concluido! Resultados em data\resultados.csv e graficos em
echo data\graficos\ (desempenho, variabilidade e cenarios).
goto fim

:cancelado
echo.
echo Execucao cancelada.
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
