# APS-Ordenacao

Projeto acadêmico desenvolvido em linguagem C para estudar e comparar o desempenho de seis algoritmos de ordenação aplicados a registros de imagens de satélite.

O sistema permite trabalhar com dados geográficos e níveis de desmatamento, medir tempos de execução, exportar resultados em CSV e gerar gráficos para análise comparativa.

## Funcionalidades

- Geração de registros aleatórios.
- Entrada manual de registros.
- Leitura e gravação de dados em arquivo de texto.
- Ordenação por nível de desmatamento, latitude ou longitude.
- Implementação de seis algoritmos de ordenação.
- Medição do tempo médio de execução no modo interativo.
- Benchmark com diferentes tamanhos de entrada e cenários.
- Exportação dos resultados em arquivos CSV.
- Geração de gráficos com Python.
- Compilação do programa em C usando CMake.

## Algoritmos implementados

- Bubble Sort
- Insertion Sort
- Selection Sort
- Merge Sort
- Quick Sort
- Heap Sort

## Requisitos

### Windows

Instale as seguintes ferramentas:

- **Git:** para clonar o repositório.
- **Python 3:** com a opção de adicionar Python ao `PATH`.
- **CMake 3.20 ou superior.**
- **GCC:** compatível com o ambiente MinGW-w64, por exemplo, pelo MSYS2.
- **Ninja:** sistema de compilação utilizado pelo script.

O CLion não é obrigatório. É possível executar o projeto pelo PowerShell, desde que as ferramentas necessárias estejam instaladas e acessíveis pelo `PATH`.

### Linux (Ubuntu/Debian)

Instale as dependências com:

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build python3 python3-venv python3-pip git
```

O CMake deve ser da versão 3.20 ou superior.

## Como clonar o repositório

Execute:

```bash
git clone https://github.com/danielsantosoliveira800-hue/APS-Ordenacao.git
cd APS-Ordenacao
```

## Como executar no Windows

1. Instale os requisitos indicados acima.
2. Abra o PowerShell na pasta do projeto.
3. Execute:

```powershell
.\executar_windows.bat
```

O script verifica as ferramentas necessárias, prepara o ambiente virtual Python, instala as bibliotecas, configura e compila o programa e inicia a aplicação.

## Como executar no Linux

1. Instale os requisitos indicados acima.
2. Entre na pasta do projeto.
3. Execute:

```bash
chmod +x executar_linux.sh
./executar_linux.sh
```

O script prepara o ambiente Python, instala as bibliotecas, compila o programa e inicia a aplicação.

## Como utilizar o programa

Ao iniciar, o sistema apresenta três opções principais:

1. **Coleta completa de desempenho:** executa o benchmark dos algoritmos.
2. **Modo interativo:** permite escolher a origem dos dados, o critério de ordenação e o algoritmo.
3. **Sair:** encerra o programa.

### Modo interativo

É possível escolher entre três origens de dados:

- Gerar registros aleatoriamente.
- Digitar os registros manualmente.
- Carregar os registros de `data/dados.txt`.

Os critérios disponíveis são nível de desmatamento, latitude e longitude.

Após selecionar o algoritmo, o programa exibe os registros antes e depois da ordenação, mede o tempo médio de execução e salva os resultados em CSV. Também é possível solicitar a geração do gráfico interativo.

Ao criar dados aleatórios ou digitá-los manualmente, o usuário pode optar por salvá-los em `data/dados.txt`. Essa operação sobrescreve o arquivo anterior.

### Benchmark de desempenho

A coleta completa compara seis algoritmos em três cenários:

- Dados ordenados.
- Dados parcialmente ordenados.
- Dados aleatórios.

São utilizados quatro tamanhos de entrada: 100, 1.000, 10.000 e 100.000 registros, totalizando até 54 combinações de algoritmo, cenário e tamanho.

**Atenção:** a coleta pode demorar bastante, principalmente com entradas grandes e algoritmos de complexidade quadrática, como Bubble Sort e Selection Sort.

## Arquivos gerados

| Arquivo ou diretório | Finalidade |
|---|---|
| `data/dados.txt` | Registros salvos para reutilização |
| `data/resultados.csv` | Resultados da coleta completa |
| `data/resultado_interativo.csv` | Resultados da ordenação interativa |
| `data/grafico_interativo.png` | Gráfico da execução interativa |
| `data/graficos/` | Gráficos comparativos do benchmark |

Os arquivos gerados são locais e podem ser recriados executando o programa. Os tempos variam de acordo com o computador, o compilador e as condições de execução.

## Dependências Python

As bibliotecas utilizadas são declaradas em `requirements.txt`:

- Pandas
- Matplotlib

Os scripts de execução instalam essas dependências em um ambiente virtual Python (`.venv`), evitando a necessidade de instalá-las globalmente.

## Estrutura do projeto

```text
APS-Ordenacao/
├── src/                  # Código-fonte em C
├── scripts/              # Scripts Python para gráficos
├── data/                 # Dados, CSVs e gráficos gerados
├── CMakeLists.txt        # Configuração da compilação
├── requirements.txt      # Dependências Python
├── executar_windows.bat  # Script de execução no Windows
├── executar_linux.sh     # Script de execução no Linux
└── README.md             # Documentação do projeto
```

## Objetivo acadêmico

O projeto tem como objetivo aplicar conceitos de algoritmos e estruturas de dados, analisar complexidades e comparar experimentalmente o desempenho de diferentes métodos de ordenação em conjuntos de dados com características distintas.

Desenvolvido para fins acadêmicos.

## Como executar no CLion

1. Clone o repositório e abra a pasta `APS-Ordenacao` no CLion usando **File > Open**.
2. Aguarde o CLion carregar e configurar o projeto pelo CMake.
3. Verifique se o CLion possui uma toolchain C configurada, com compilador GCC ou Clang e CMake.
4. Abra **Run > Edit Configurations** e selecione a configuração `APS_Ordenacao`.
5. Defina o **Working directory** como a raiz do projeto (`$PROJECT_DIR$`).
6. Compile usando **Build > Build Project**.
7. Execute a configuração `APS_Ordenacao`.

**Importante:** o diretório de trabalho deve ser a raiz do projeto para que os caminhos relativos de `data/` funcionem corretamente.

Para gerar gráficos, instale as dependências listadas em `requirements.txt`. Os scripts `.bat` e `.sh` preparam um ambiente virtual Python e instalam essas dependências automaticamente quando executados conforme as instruções anteriores.
