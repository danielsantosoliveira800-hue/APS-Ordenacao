import os
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("data/resultados.csv")

os.makedirs("data/graficos", exist_ok=True)

algoritmos_n2 = ["Bubble", "Insertion", "Selection"]
algoritmos_nlogn = ["Merge", "Quick", "Heap"]

cenarios = df["cenario"].unique()


def plotar_grupo(dados_cenario, algoritmos, titulo, nome_arquivo):
    plt.figure(figsize=(10, 6))

    for algoritmo in algoritmos:
        dados_algoritmo = dados_cenario[dados_cenario["algoritmo"] == algoritmo]
        dados_algoritmo = dados_algoritmo.sort_values("tamanho")

        plt.plot(dados_algoritmo["tamanho"], dados_algoritmo["tempo_segundos"],
                  marker="o", label=algoritmo)

    plt.title(titulo)
    plt.xlabel("Tamanho da entrada (n)")
    plt.ylabel("Tempo (segundos)")
    plt.legend()
    plt.grid(True)

    plt.savefig(nome_arquivo)
    plt.close()

    print(f"Gráfico salvo: {nome_arquivo}")


for cenario in cenarios:
    dados_cenario = df[df["cenario"] == cenario]

    plotar_grupo(
        dados_cenario, algoritmos_n2,
        f"Algoritmos O(n²) — cenário: {cenario}",
        f"data/graficos/{cenario}_n2.png"
    )

    plotar_grupo(
        dados_cenario, algoritmos_nlogn,
        f"Algoritmos O(n log n) — cenário: {cenario}",
        f"data/graficos/{cenario}_nlogn.png"
    )