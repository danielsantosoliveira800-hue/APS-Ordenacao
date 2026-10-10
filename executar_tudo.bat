@echo off
echo Executando o programa em C (coleta de performance)...

echo 1 > temp_input.txt
echo 3 >> temp_input.txt
cmake-build-debug\APS_Ordenacao.exe < temp_input.txt
del temp_input.txt

echo.
echo Gerando graficos em Python...
python scripts\gerar_graficos.py

echo.
echo Concluido! Resultados em data\resultados.csv e graficos em data\graficos\
pause