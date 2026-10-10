import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("data/resultado_interativo.csv")

algoritmo = df["algoritmo"].iloc[0]
tempo = df["tempo_segundos"].iloc[0]
criterio = df["criterio"].iloc[0]
repeticoes = int(df["repeticoes"].iloc[0])

x = df["indice"]
largura = 0.35

plt.figure(figsize=(10, 6))
plt.bar(x - largura/2, df["valor_antes"], largura, label="Antes")
plt.bar(x + largura/2, df["valor_depois"], largura, label="Depois")

plt.title(f"{algoritmo} — Criterio: {criterio}\nTempo medio: {tempo * 1e6:.2f} µs (media de {repeticoes} execucoes, n = {len(df)})")
plt.xlabel("Indice do registro")
plt.ylabel(criterio)
plt.legend()
plt.grid(True, axis="y")

plt.savefig("data/grafico_interativo.png")
plt.close()

print("Grafico salvo: data/grafico_interativo.png")