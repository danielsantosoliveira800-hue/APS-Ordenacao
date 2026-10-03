#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "registro.h"
#include "gerador.h"
#include "ordenacao.h"
#include "performace.h"
#include "performace.h"

int main(void) {
    srand((unsigned int) time(NULL));

    int tamanhos[] = {100, 1000, 10000, 100000};
    int qtd_tamanhos = 4;

    const char *cenarios[] = {"ordenado", "semi_ordenado", "aleatorio"};
    int qtd_cenarios = 3;

    FuncaoOrdenacao algoritmos[] = {bubble_sort, insertion_sort, selection_sort, merge_sort, quick_sort, heap_sort};
    const char *nomes_algoritmos[] = {"Bubble", "Insertion", "Selection", "Merge", "Quick", "Heap"};
    int qtd_algoritmos = 6;

    FILE *csv = fopen("data/resultados.csv", "w");
    if (csv == NULL) {
        printf("Erro ao criar o arquivo CSV. Verifique se a pasta 'data/' existe no diretorio de trabalho atual.\n");
        return 1;
    }
    fprintf(csv, "algoritmo,cenario,tamanho,tempo_segundos\n");

    for (int t = 0; t < qtd_tamanhos; t++) {
        int n = tamanhos[t];

        RegistroImagem *dados = malloc(n * sizeof(RegistroImagem));
        gerar_aleatorios(dados, n);

        RegistroImagem **base_aleatorio = malloc(n * sizeof(RegistroImagem *));
        for (int i = 0; i < n; i++) base_aleatorio[i] = &dados[i];

        RegistroImagem **base_ordenado = malloc(n * sizeof(RegistroImagem *));
        memcpy(base_ordenado, base_aleatorio, n * sizeof(RegistroImagem *));
        merge_sort(base_ordenado, 0, n - 1);

        RegistroImagem **base_semi = malloc(n * sizeof(RegistroImagem *));
        memcpy(base_semi, base_ordenado, n * sizeof(RegistroImagem *));
        embaralhar_parcial(base_semi, 0, n - 1, 0.4f);

        RegistroImagem **bases[] = {base_ordenado, base_semi, base_aleatorio};

        for (int c = 0; c < qtd_cenarios; c++) {
            for (int a = 0; a < qtd_algoritmos; a++) {
                RegistroImagem **copia = malloc(n * sizeof(RegistroImagem *));
                memcpy(copia, bases[c], n * sizeof(RegistroImagem *));

                double tempo = medir_tempo(algoritmos[a], copia, 0, n - 1);

                printf("%s | %s | n=%d | %.6f s\n", nomes_algoritmos[a], cenarios[c], n, tempo);
                fprintf(csv, "%s,%s,%d,%.6f\n", nomes_algoritmos[a], cenarios[c], n, tempo);

                free(copia);
            }
        }

        free(base_aleatorio);
        free(base_ordenado);
        free(base_semi);
        free(dados);
    }

    fclose(csv);
    return 0;
}