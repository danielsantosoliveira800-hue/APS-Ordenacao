
#!/usr/bin/env bash

# Interrompe o script se algum comando falhar.
set -e

# Entra na pasta onde este script está localizado.
cd "$(dirname "$0")"

echo "=== APS-Ordenacao ==="

# Verifica se o CMake está instalado.
command -v cmake >/dev/null 2>&1 || {
    echo "Erro: instale o CMake."
    exit 1
}

# Verifica se o Python 3 está instalado.
command -v python3 >/dev/null 2>&1 || {
    echo "Erro: instale o Python 3."
    exit 1
}

# Seleciona o compilador C disponível.
if command -v gcc >/dev/null 2>&1; then
    COMPILADOR="gcc"
elif command -v clang >/dev/null 2>&1; then
    COMPILADOR="clang"
else
    echo "Erro: instale o GCC ou o Clang."
    exit 1
fi

# Verifica se o arquivo de dependências existe.
if [ ! -f requirements.txt ]; then
    echo "Erro: requirements.txt não encontrado."
    exit 1
fi

# Cria um ambiente virtual Python se ainda não existir.
if [ ! -d ".venv" ]; then
    python3 -m venv .venv
fi

# Ativa o ambiente virtual.
source .venv/bin/activate

# Instala as dependências Python do projeto.
python -m pip install -r requirements.txt

# Configura o projeto com o compilador selecionado.
cmake -S . -B build -DCMAKE_C_COMPILER="$COMPILADOR"

# Compila o programa em C.
cmake --build build

# Remove um sinal antigo para detectar apenas uma nova coleta.
rm -f "data/.benchmark_concluido"

# Executa o programa compilado.
./build/APS_Ordenacao

# Gera os gráficos somente se uma coleta de desempenho foi concluída.
if [ -f "data/.benchmark_concluido" ]; then
    echo
    echo "Benchmark detectado. Gerando graficos de desempenho..."

    if .venv/bin/python scripts/gerar_graficos.py; then
        rm -f "data/.benchmark_concluido"
        echo "Graficos de desempenho atualizados."
    else
        echo "Erro ao gerar os graficos." >&2
        rm -f "data/.benchmark_concluido"
        exit 1
    fi
fi