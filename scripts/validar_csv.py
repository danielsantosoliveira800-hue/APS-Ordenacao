from pathlib import Path
import sys

import numpy as np
import pandas as pd

RAIZ_PROJETO = Path(__file__).resolve().parents[1]
ARQUIVO_CSV = RAIZ_PROJETO / "data" / "resultados.csv"

try:
    df = pd.read_csv(ARQUIVO_CSV)
except FileNotFoundError:
    print(f"Erro: arquivo nao encontrado: {ARQUIVO_CSV}")
    sys.exit(1)

algoritmos_esperados = {"Bubble", "Insertion", "Selection", "Merge", "Quick", "Heap"}
cenarios_esperados = {"ordenado", "semi_ordenado", "aleatorio"}
tamanhos_esperados = {100, 1000, 10000, 100000}
repeticoes_por_tamanho = {100: 30, 1000: 30, 10000: 10, 100000: 3}
colunas_esperadas = ["algoritmo", "cenario", "tamanho", "repeticoes",
                     "media_us", "mediana_us", "desvio_us", "min_us", "max_us"]
colunas_tempo = ["media_us", "mediana_us", "desvio_us", "min_us", "max_us"]

print("=== ESTRUTURA ===")
print("Linhas de dados:", len(df), "(esperado 72)")
colunas_ok = list(df.columns) == colunas_esperadas
print("Colunas corretas:", colunas_ok)
if not colunas_ok:
    raise SystemExit("O CSV nao tem as colunas esperadas: " + str(list(df.columns)))

print("\n=== COMBINACOES ===")
chave = ["algoritmo", "cenario", "tamanho"]
print("Combinacoes duplicadas:", int(df.duplicated(subset=chave, keep=False).sum()), "(esperado 0)")
print("Combinacoes unicas:", len(df[chave].drop_duplicates()), "(esperado 72)")
print("Algoritmos corretos:", set(df["algoritmo"]) == algoritmos_esperados)
print("Cenarios corretos:", set(df["cenario"]) == cenarios_esperados)
print("Tamanhos corretos:", set(df["tamanho"]) == tamanhos_esperados)

print("\n=== REPETICOES (por tamanho) ===")
esperadas = df["tamanho"].map(repeticoes_por_tamanho)
rep_ok = esperadas.notna() & (df["repeticoes"] == esperadas)
print("Linhas com repeticoes corretas:", int(rep_ok.sum()), "de", len(df))

print("\n=== ESTATISTICAS ===")
print("Tempos iguais a zero:", int((df[["media_us", "mediana_us", "min_us", "max_us"]] == 0).sum().sum()), "(esperado 0)")
print("Valores negativos:", int((df[colunas_tempo] < 0).sum().sum()), "(esperado 0)")
print("Valores ausentes ou infinitos:", int((~np.isfinite(df[colunas_tempo])).sum().sum()), "(esperado 0)")

ordem_ok = (df["min_us"] <= df["mediana_us"]) & (df["mediana_us"] <= df["max_us"])
print("Linhas com min <= mediana <= max:", int(ordem_ok.sum()), "de", len(df))
media_ok = (df["min_us"] <= df["media_us"]) & (df["media_us"] <= df["max_us"])
print("Linhas com min <= media <= max:", int(media_ok.sum()), "de", len(df))

print("\n=== MINIMOS EM RELACAO A MEDIANA ===")
df["min_sobre_mediana"] = df["min_us"] / df["mediana_us"]
print(df.nsmallest(5, "min_sobre_mediana")[
          ["algoritmo", "cenario", "tamanho", "repeticoes", "min_us", "mediana_us", "min_sobre_mediana"]
      ].to_string(index=False))

print("\n=== RESULTADO ===")
validacao_ok = (
    len(df) == 72
    and colunas_ok
    and int(df.duplicated(subset=chave, keep=False).sum()) == 0
    and set(df["algoritmo"]) == algoritmos_esperados
    and set(df["cenario"]) == cenarios_esperados
    and set(df["tamanho"]) == tamanhos_esperados
    and bool(rep_ok.all())
    and int((df[["media_us", "mediana_us", "min_us", "max_us"]] <= 0).sum().sum()) == 0
    and int((df[colunas_tempo] < 0).sum().sum()) == 0
    and int((~np.isfinite(df[colunas_tempo])).sum().sum()) == 0
    and bool(ordem_ok.all())
    and bool(media_ok.all())
)
print("CSV valido:", "SIM" if validacao_ok else "NAO")
if not validacao_ok:
    sys.exit(1)

print("Validacao concluida.")