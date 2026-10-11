# APS-Ordenacao

Projeto acadêmico em linguagem C para estudar e comparar o desempenho de algoritmos de ordenação usando conjuntos de dados com diferentes características.

## Funcionalidades

- Execução de benchmarks de desempenho.
- Geração e ordenação de registros.
- Modo interativo para selecionar dados e algoritmos.
- Exportação de resultados em CSV.
- Geração de gráficos com Python, Pandas e Matplotlib.
- Compilação multiplataforma com CMake.

## Requisitos

### Windows

Instale:

- Python 3, com a opção de adicionar Python ao PATH.
- CMake.
- GCC compatível com o ambiente MinGW-w64.
- Ninja.

O CLion não é obrigatório. As ferramentas podem ser instaladas separadamente.

### Linux (Ubuntu/Debian)

Instale as dependências:

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build python3 python3-venv python3-pip git
```

## Como executar

### Windows

1. Instale os requisitos listados acima.
2. Clone o repositório.
3. Abra o PowerShell na pasta do projeto.
4. Execute:

```powershell
.\executar_windows.bat
```

O script prepara o ambiente Python, instala as dependências, compila o programa e inicia a aplicação.

### Linux

1. Instale os requisitos listados acima.
2. Clone o repositório.
3. Entre na pasta do projeto.
4. Dê permissão de execução ao script e execute:

```bash
chmod +x executar_linux.sh
./executar_linux.sh
```

O script prepara o ambiente Python, instala as dependências, compila o programa e inicia a aplicação.

## Clonar o repositório

```bash
git clone https://github.com/danielsantosoliveira800-hue/APS-Ordenacao.git
cd APS-Ordenacao
```

No Windows, os comandos podem ser executados pelo PowerShell, desde que o Git esteja instalado.

## Dependências Python

As bibliotecas utilizadas pelo projeto estão declaradas em `requirements.txt` e são instaladas automaticamente pelos scripts de execução.

## Estrutura do projeto

```text
APS-Ordenacao/
├── src/                 # Código-fonte em C
├── scripts/             # Scripts Python para gráficos
├── data/                # Dados e resultados gerados
├── CMakeLists.txt       # Configuração da compilação
├── requirements.txt     # Dependências Python
├── executar_windows.bat # Execução no Windows
└── executar_linux.sh    # Execução no Linux
```

## Observações

- A primeira execução pode demorar porque as dependências Python precisam ser instaladas e o programa compilado.
- Os benchmarks podem levar bastante tempo, especialmente para conjuntos de dados grandes e algoritmos quadráticos.
- Os tempos medidos variam conforme o computador, o compilador e as condições de execução.
- Os resultados e gráficos são gerados localmente pelo programa.

## Objetivo acadêmico

O projeto tem como objetivo estudar algoritmos de ordenação, comparar complexidades e analisar experimentalmente o desempenho em diferentes padrões de entrada.

Desenvolvido para fins acadêmicos.
