# APS-Ordenacao

Projeto acadêmico desenvolvido em C para estudar e comparar o desempenho
de seis algoritmos de ordenação aplicados a registros de imagens de
satélite.

O sistema permite trabalhar com dados geográficos e níveis de
desmatamento, medir tempos de execução, exportar resultados em CSV e
gerar gráficos para análise comparativa.

## Funcionalidades

-   Geração de registros aleatórios.
-   Entrada manual de registros.
-   Leitura e gravação de dados em arquivo de texto.
-   Ordenação por nível de desmatamento, latitude ou longitude.
-   Implementação de seis algoritmos de ordenação.
-   Medição do tempo de execução no modo interativo.
-   Benchmark com diferentes tamanhos de entrada e cenários, incluindo
    repetições e estatísticas: mediana, média, desvio-padrão, mínimo e
    máximo.
-   Exportação dos resultados em CSV.
-   Geração de gráficos com Python.
-   Compilação em C com CMake.
-   Testes automatizados dos algoritmos.

## Algoritmos implementados

-   Bubble Sort
-   Insertion Sort
-   Selection Sort
-   Merge Sort
-   Quick Sort
-   Heap Sort

## Requisitos

Consulte o [guia de instalação](docs/INSTALACAO.md) para obter
instruções detalhadas de configuração no Windows e no Linux.

### Windows

São necessárias as seguintes ferramentas:

-   **Python 3**, com suporte a ambientes virtuais.
-   **CMake 3.20 ou superior.**
-   **GCC** compatível com MinGW-w64, por exemplo, instalado pelo MSYS2.
-   **Ninja**, disponível no `PATH`.
-   **Git**, para clonar o repositório.

O CLion não é obrigatório. É possível compilar e executar o projeto pelo
PowerShell, desde que as ferramentas estejam instaladas e acessíveis
pelo `PATH`.

### Linux (Ubuntu/Debian)

Instale as dependências com:

``` bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build python3 python3-venv python3-pip git
```

O CMake deve ser da versão 3.20 ou superior.

## Como obter o projeto

Depois que o repositório público estiver criado, clone-o usando a URL
exibida na página do GitHub:

``` bash
git clone <URL_DO_REPOSITORIO>
cd APS-Ordenacao
```

Substitua `<URL_DO_REPOSITORIO>` pela URL real do repositório.

## Como executar no Windows

1.  Instale os requisitos indicados acima.
2.  Abra o PowerShell na pasta do projeto.
3.  Para compilar e iniciar o programa, execute:

``` powershell
.\executar_windows.bat
```

O script prepara o ambiente Python, instala as dependências, compila o
programa e inicia a aplicação.

### Executar o benchmark completo

Para executar a coleta de desempenho, validar o CSV e gerar os gráficos
em uma única operação, execute:

``` powershell
.\executar_tudo.bat
```

O script compila em Release, executa o benchmark, valida
`data/resultados.csv` e gera os gráficos em `data/graficos/`. Antes de
começar, solicita confirmação. **A execução sobrescreve o CSV e os
gráficos de referência** e pode levar cerca de 15 minutos, dependendo do
computador.

## Como executar no Linux

1.  Instale os requisitos indicados acima.
2.  Entre na pasta do projeto.
3.  Execute:

``` bash
chmod +x executar_linux.sh
./executar_linux.sh
```

O script prepara o ambiente Python, instala as dependências, compila o
programa e inicia a aplicação.

## Como executar no CLion

1.  Abra a pasta `APS-Ordenacao` no CLion.
2.  Aguarde o CLion carregar e configurar o projeto pelo CMake.
3.  Verifique se existe uma toolchain C configurada, com GCC ou Clang e
    CMake.
4.  Abra **Run \> Edit Configurations** e selecione `APS_Ordenacao`.
5.  Defina o **Working directory** como a raiz do projeto
    (`$PROJECT_DIR$`).
6.  Compile usando **Build \> Build Project**.
7.  Execute a configuração `APS_Ordenacao`.

**Importante:** o diretório de trabalho deve ser a raiz do projeto para
que os caminhos relativos de `data/` funcionem corretamente.

Para gerar gráficos manualmente, instale as dependências listadas em
`requirements.txt`. Os scripts `.bat` e `.sh` preparam um ambiente
virtual Python e instalam essas dependências ao serem executados
conforme as instruções.

## Tipo de build

Se nenhum tipo de build for informado, o `CMakeLists.txt` define
`Release` como padrão para builds de configuração única. O modo Release
é o indicado para comparar tempos de execução. Um build Debug pode
produzir tempos absolutos bem maiores e alterar comparações entre
algoritmos.

No CLion, selecione o perfil **Release** em **File \> Settings \> Build,
Execution, Deployment \> CMake** antes de realizar medições.

## Como utilizar o programa

Ao iniciar, o sistema apresenta três opções principais:

1.  **Coleta completa de desempenho:** executa o benchmark dos
    algoritmos.
2.  **Modo interativo:** permite escolher a origem dos dados, o critério
    de ordenação e o algoritmo.
3.  **Sair:** encerra o programa.

### Modo interativo

É possível escolher entre três origens de dados:

-   Gerar registros aleatoriamente.
-   Digitar os registros manualmente.
-   Carregar os registros de `data/dados.txt`.

Os critérios disponíveis são nível de desmatamento, latitude e
longitude.

Após selecionar o algoritmo, o programa exibe os registros antes e
depois da ordenação, mede o tempo de execução e salva os resultados em
CSV. Também é possível solicitar a geração de um gráfico interativo.

Ao criar dados aleatórios ou digitá-los manualmente, o usuário pode
optar por salvá-los em `data/dados.txt`. Essa operação sobrescreve o
arquivo anterior.

### Benchmark de desempenho

A coleta completa compara seis algoritmos em três cenários:

-   Dados ordenados.
-   Dados parcialmente ordenados.
-   Dados aleatórios.

São utilizados quatro tamanhos de entrada: 100, 1.000, 10.000 e 100.000
registros. Isso totaliza **72 combinações** de algoritmo, cenário e
tamanho (6 × 3 × 4).

Cada combinação é executada várias vezes:

-   30 repetições para tamanhos de até 1.000 registros.
-   10 repetições para 10.000 registros.
-   3 repetições para 100.000 registros.

Antes de cada repetição, o vetor de ponteiros é restaurado à ordem
original, e somente a chamada do algoritmo é cronometrada. O arquivo
`data/resultados.csv` contém a mediana, a média, o desvio-padrão, o
mínimo e o máximo dos tempos, em microssegundos.

**Atenção:** a coleta pode demorar bastante, principalmente com entradas
grandes e algoritmos de complexidade quadrática, como Bubble Sort e
Selection Sort. No computador utilizado para a coleta de referência, o
processo levou cerca de 15 minutos. Para reduzir interferências nas
medições, evite executar programas pesados em segundo plano.

## Testes automatizados

O projeto inclui testes automatizados para os seis algoritmos. Eles
verificam:

-   Ordenação pelos três critérios disponíveis.
-   Preservação dos registros, sem perda ou duplicação de ponteiros.
-   Ordenação de subvetores e preservação das extremidades externas.
-   Casos-limite de vetor com um elemento e intervalo vazio.

Para configurar, compilar e executar os testes no Windows ou Linux com
CMake e Ninja instalados:

``` bash
cmake -S . -B build_testes -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DBUILD_TESTING=ON
cmake --build build_testes
ctest --test-dir build_testes --output-on-failure
```

O resultado esperado, quando todos os testes passam, inclui
`100% tests passed`. O projeto foi validado localmente com o teste
`algoritmos_ordenacao` aprovado.

## Validação do CSV e geração dos gráficos

Depois da coleta, valide o CSV e regenere os gráficos com:

``` bash
python scripts/validar_csv.py
python scripts/gerar_graficos.py
```

O primeiro script verifica a estrutura do arquivo, as 72 combinações, as
repetições por tamanho e a consistência entre mínimo, mediana, média e
máximo. Essa validação verifica a estrutura e a consistência dos dados;
não garante, por si só, que todas as medições estejam livres de
interferências externas.

O segundo script gera 18 gráficos em `data/graficos/`, organizados em
três pastas:

-   `desempenho/`: comparação dos algoritmos dentro de um cenário.
-   `variabilidade/`: mediana com faixa entre o mínimo e o máximo.
-   `cenarios/`: comparação de um algoritmo nos três cenários.

Os gráficos usam escala logarítmica nos dois eixos.

## Arquivos de dados e resultados

  -----------------------------------------------------------------------
  Arquivo ou diretório                Finalidade
  ----------------------------------- -----------------------------------
  `data/dados.txt`                    Registros salvos para reutilização

  `data/resultados.csv`               Resultados de referência do
                                      benchmark, com 72 linhas e tempos
                                      em microssegundos

  `data/graficos/`                    18 gráficos comparativos do
                                      benchmark

  `data/resultado_interativo.csv`     Resultado gerado localmente pelo
                                      modo interativo

  `data/grafico_interativo.png`       Gráfico gerado localmente pelo modo
                                      interativo
  -----------------------------------------------------------------------

O repositório inclui `data/resultados.csv` e os gráficos estáticos em
`data/graficos/` como resultados de referência. Executar novamente o
benchmark sobrescreve esses arquivos. Os artefatos do modo interativo
são locais e podem ser recriados pelo programa; eles não precisam ser
enviados ao repositório.

## Ambiente dos resultados de referência

A coleta de referência foi realizada em um computador com:

-   Processador Intel Core i5-6200U, 2,30 GHz.
-   15 GB de RAM.
-   Windows 10 Pro.
-   GCC 15.2.0 (MinGW).
-   Build Release com as opções `-O3 -DNDEBUG`.

Os tempos variam conforme o computador, o compilador, as opções de
compilação e as condições de execução. Portanto, os valores de
referência não devem ser generalizados para outros equipamentos.

## Dependências Python

As bibliotecas utilizadas estão declaradas em `requirements.txt`:

-   Pandas
-   Matplotlib

Os scripts de execução instalam essas dependências em um ambiente
virtual Python (`.venv`), evitando a instalação global.

## Estrutura do projeto

``` text
APS-Ordenacao/
├── src/                  # Código-fonte em C
├── tests/                # Testes automatizados dos algoritmos
├── scripts/              # Validação do CSV e geração de gráficos
├── data/
│   ├── dados.txt         # Dados de exemplo
│   ├── resultados.csv    # Resultados de referência
│   └── graficos/         # Gráficos comparativos
├── docs/
│   └── INSTALACAO.md     # Guia de instalação
├── CMakeLists.txt        # Configuração da compilação e dos testes
├── requirements.txt      # Dependências Python
├── .gitignore            # Arquivos e pastas locais ignorados pelo Git
├── executar_windows.bat  # Execução no Windows
├── executar_tudo.bat     # Benchmark, validação e gráficos no Windows
├── executar_linux.sh     # Execução no Linux
└── README.md             # Documentação do projeto
```

## Objetivo acadêmico

O projeto aplica conceitos de algoritmos e estruturas de dados, analisa
o comportamento de métodos de ordenação e compara experimentalmente seu
desempenho em conjuntos de dados com características distintas.

Desenvolvido para fins acadêmicos.
