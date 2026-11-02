import pandas as pd

# Medianas (us) da coleta Debug em n = 100000, copiadas do terminal daquela coleta.
debug_us = {
    ("Bubble", "ordenado"): 99606642.2,
    ("Insertion", "ordenado"): 2360.9,
    ("Selection", "ordenado"): 93521813.4,
    ("Merge", "ordenado"): 40691.7,
    ("Quick", "ordenado"): 23880.8,
    ("Heap", "ordenado"): 51436.8,
    ("Bubble", "semi_ordenado"): 111240371.6,
    ("Insertion", "semi_ordenado"): 11722103.8,
    ("Selection", "semi_ordenado"): 91130351.8,
    ("Merge", "semi_ordenado"): 56465.8,
    ("Quick", "semi_ordenado"): 33690.7,
    ("Heap", "semi_ordenado"): 65673.8,
    ("Bubble", "aleatorio"): 96630946.4,
    ("Insertion", "aleatorio"): 35252937.8,
    ("Selection", "aleatorio"): 44987412.4,
    ("Merge", "aleatorio"): 57576.8,
    ("Quick", "aleatorio"): 33367.7,
    ("Heap", "aleatorio"): 69603.4,
}

release = pd.read_csv("data/resultados.csv")
print("Menor tempo medido em Release:", release["min_us"].min(), "us")

r = release[release["tamanho"] == 100000].copy()
r["debug_us"] = [debug_us[(a, c)] for a, c in zip(r["algoritmo"], r["cenario"])]
r["debug_s"] = r["debug_us"] / 1e6
r["release_s"] = r["mediana_us"] / 1e6
r["aceleracao"] = r["debug_us"] / r["mediana_us"]

print("\n=== n = 100000: mediana em segundos (Debug x Release) ===")
print(r.sort_values(["algoritmo", "cenario"])[
          ["algoritmo", "cenario", "debug_s", "release_s", "aceleracao"]
      ].round(3).to_string(index=False))

print("\n=== Selection n = 100000: ordenado x aleatorio ===")
sel = r[r["algoritmo"] == "Selection"].set_index("cenario")[["debug_s", "release_s"]]
print(sel.round(2))