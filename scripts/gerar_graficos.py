from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter


# ============================================================
# 1. CONFIGURAÇÕES
# ============================================================

PASTA_PROJETO = Path(__file__).resolve().parents[1]

ARQUIVO_CSV = PASTA_PROJETO / "data" / "resultados.csv"
PASTA_GRAFICOS = PASTA_PROJETO / "data" / "graficos"

# Pacotes (subpastas) em que os gráficos são organizados.
PACOTE_DESEMPENHO = "desempenho"
PACOTE_VARIABILIDADE = "variabilidade"
PACOTE_CENARIOS = "cenarios"

ALGORITMOS_N2 = ["Bubble", "Insertion", "Selection"]
ALGORITMOS_NLOGN = ["Merge", "Quick", "Heap"]

CENARIOS = ["ordenado", "semi_ordenado", "aleatorio"]

TAMANHOS = [100, 1000, 10000, 100000]

ROTULOS_TAMANHOS = [
    "100",
    "1.000",
    "10.000",
    "100.000"
]


# ============================================================
# 2. CARREGAMENTO E VALIDAÇÃO DOS DADOS
# ============================================================

def carregar_dados():
    if not ARQUIVO_CSV.exists():
        raise FileNotFoundError(
            f"Arquivo CSV não encontrado: {ARQUIVO_CSV}"
        )

    df = pd.read_csv(ARQUIVO_CSV)

    colunas_obrigatorias = [
        "algoritmo",
        "cenario",
        "tamanho",
        "mediana_us",
        "media_us",
        "desvio_us",
        "min_us",
        "max_us"
    ]

    colunas_ausentes = [
        coluna
        for coluna in colunas_obrigatorias
        if coluna not in df.columns
    ]

    if colunas_ausentes:
        raise ValueError(
            f"Colunas ausentes no CSV: {colunas_ausentes}"
        )

    if df.empty:
        raise ValueError("O CSV não possui dados.")

    colunas_numericas = [
        "tamanho",
        "mediana_us",
        "media_us",
        "desvio_us",
        "min_us",
        "max_us"
    ]

    for coluna in colunas_numericas:
        df[coluna] = pd.to_numeric(
            df[coluna],
            errors="raise"
        )

    if df[colunas_numericas].isna().any().any():
        raise ValueError(
            "Existem valores ausentes nas colunas numéricas."
        )

    if (df["mediana_us"] <= 0).any():
        raise ValueError(
            "A mediana deve ser positiva para utilizar escala logarítmica."
        )

    if (df["min_us"] <= 0).any():
        raise ValueError(
            "O tempo mínimo deve ser positivo para utilizar escala logarítmica."
        )

    if (df["desvio_us"] < 0).any():
        raise ValueError(
            "O desvio-padrão não pode ser negativo."
        )

    return df


# ============================================================
# 3. CONFIGURAÇÃO VISUAL DOS GRÁFICOS
# ============================================================

def formatar_tempo(valor, _posicao):
    # Converte microssegundos para a unidade mais legível no eixo Y.
    if valor >= 1e6:
        return f"{valor / 1e6:g} s"
    if valor >= 1e3:
        return f"{valor / 1e3:g} ms"
    return f"{valor:g} µs"


def configurar_eixos(titulo):
    plt.title(titulo)

    plt.xlabel("Tamanho da entrada (n)")
    plt.ylabel("Tempo (mediana)")

    plt.xscale("log")
    plt.yscale("log")

    plt.xticks(
        TAMANHOS,
        ROTULOS_TAMANHOS
    )

    plt.gca().yaxis.set_major_formatter(FuncFormatter(formatar_tempo))

    plt.legend()
    plt.grid(
        True,
        which="both",
        linestyle="--",
        alpha=0.5
    )

    plt.tight_layout()


# ============================================================
# 4. SALVAMENTO DOS GRÁFICOS
# ============================================================

def salvar_grafico(pacote, nome_arquivo):
    pasta = PASTA_GRAFICOS / pacote
    pasta.mkdir(parents=True, exist_ok=True)

    caminho = pasta / nome_arquivo

    plt.savefig(
        caminho,
        dpi=300,
        bbox_inches="tight"
    )

    plt.close()

    print(f"Gráfico salvo: {caminho}")


# ============================================================
# 5. PACOTE DESEMPENHO (ALGORITMOS DENTRO DE UM CENÁRIO)
# ============================================================

def plotar_grupo(
        dados_cenario,
        algoritmos,
        titulo,
        nome_arquivo
):
    plt.figure(figsize=(10, 6))

    for algoritmo in algoritmos:

        dados = dados_cenario[
            dados_cenario["algoritmo"] == algoritmo
            ].sort_values("tamanho")

        if dados.empty:
            continue

        plt.plot(
            dados["tamanho"],
            dados["mediana_us"],
            marker="o",
            label=algoritmo
        )

    configurar_eixos(titulo)

    salvar_grafico(PACOTE_DESEMPENHO, nome_arquivo)


# ============================================================
# 6. PACOTE VARIABILIDADE (FAIXA DE MÍNIMO A MÁXIMO)
# ============================================================

def plotar_variabilidade(
        dados_cenario,
        algoritmos,
        titulo,
        nome_arquivo
):
    plt.figure(figsize=(10, 6))

    for algoritmo in algoritmos:

        dados = dados_cenario[
            dados_cenario["algoritmo"] == algoritmo
            ].sort_values("tamanho")

        if dados.empty:
            continue

        linha, = plt.plot(
            dados["tamanho"],
            dados["mediana_us"],
            marker="o",
            label=algoritmo
        )

        # Faixa do menor ao maior tempo medido, na mesma cor da linha.
        # Em n = 100.000 ela vem de apenas 3 repetições.
        plt.fill_between(
            dados["tamanho"],
            dados["min_us"],
            dados["max_us"],
            alpha=0.15,
            color=linha.get_color()
        )

    configurar_eixos(titulo)

    salvar_grafico(PACOTE_VARIABILIDADE, nome_arquivo)


# ============================================================
# 7. PACOTE CENARIOS (UM ALGORITMO EM TODOS OS CENÁRIOS)
# ============================================================

def plotar_cenarios(
        df,
        algoritmo,
        nome_arquivo
):
    plt.figure(figsize=(10, 6))

    for cenario in CENARIOS:

        dados = df[
            (df["algoritmo"] == algoritmo)
            & (df["cenario"] == cenario)
            ].sort_values("tamanho")

        if dados.empty:
            continue

        plt.plot(
            dados["tamanho"],
            dados["mediana_us"],
            marker="o",
            label=cenario
        )

    configurar_eixos(f"{algoritmo} Sort — mediana por cenário")

    salvar_grafico(PACOTE_CENARIOS, nome_arquivo)


# ============================================================
# 8. EXECUÇÃO PRINCIPAL
# ============================================================

def main():

    print("Iniciando geração dos gráficos...\n")

    PASTA_GRAFICOS.mkdir(
        parents=True,
        exist_ok=True
    )

    df = carregar_dados()

    contagem = {
        PACOTE_DESEMPENHO: 0,
        PACOTE_VARIABILIDADE: 0,
        PACOTE_CENARIOS: 0
    }

    for cenario in CENARIOS:

        dados_cenario = df[
            df["cenario"] == cenario
            ]

        if dados_cenario.empty:
            print(
                f"Aviso: nenhum dado encontrado para {cenario}."
            )
            continue

        # ----------------------------------------------------
        # Pacote desempenho
        # ----------------------------------------------------

        plotar_grupo(
            dados_cenario,
            ALGORITMOS_N2,
            f"Variabilidade O(n²) — cenário: {cenario}\n(linha: mediana; faixa: mínimo a máximo)",
            f"{cenario}_n2.png"
        )
        contagem[PACOTE_DESEMPENHO] += 1

        plotar_grupo(
            dados_cenario,
            ALGORITMOS_NLOGN,
            f"Variabilidade O(n log n) — cenário: {cenario}\n(linha: mediana; faixa: mínimo a máximo)",
            f"{cenario}_nlogn.png"
        )
        contagem[PACOTE_DESEMPENHO] += 1

        # ----------------------------------------------------
        # Pacote variabilidade
        # ----------------------------------------------------

        plotar_variabilidade(
            dados_cenario,
            ALGORITMOS_N2,
            f"Variabilidade O(n²) — cenário: {cenario}",
            f"{cenario}_n2_variabilidade.png"
        )
        contagem[PACOTE_VARIABILIDADE] += 1

        plotar_variabilidade(
            dados_cenario,
            ALGORITMOS_NLOGN,
            f"Variabilidade O(n log n) — cenário: {cenario}",
            f"{cenario}_nlogn_variabilidade.png"
        )
        contagem[PACOTE_VARIABILIDADE] += 1

    # --------------------------------------------------------
    # Pacote cenarios: um gráfico por algoritmo
    # --------------------------------------------------------

    for algoritmo in ALGORITMOS_N2 + ALGORITMOS_NLOGN:

        plotar_cenarios(
            df,
            algoritmo,
            f"cenarios_{algoritmo.lower()}.png"
        )
        contagem[PACOTE_CENARIOS] += 1

    print("\n===================================")
    print("GERAÇÃO CONCLUÍDA")
    for pacote, quantidade in contagem.items():
        print(f"  {pacote}: {quantidade} gráficos")
    print(f"Total de gráficos gerados: {sum(contagem.values())}")
    print(f"Diretório: {PASTA_GRAFICOS}")
    print("===================================")


if __name__ == "__main__":
    main()