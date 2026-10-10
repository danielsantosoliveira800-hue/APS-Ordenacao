#include <time.h>
#include "performace.h"
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

double medir_tempo(FuncaoOrdenacao algoritmo, RegistroImagem **ponteiros, int inicio, int fim) {
    clock_t t_inicio = clock();
    algoritmo(ponteiros, inicio, fim);
    clock_t t_fim = clock();

    return (double)(t_fim - t_inicio) / CLOCKS_PER_SEC;
}

static double agora_segundos(void) {
#ifdef _WIN32
    LARGE_INTEGER frequencia, contador;
    QueryPerformanceFrequency(&frequencia);
    QueryPerformanceCounter(&contador);
    return (double)contador.QuadPart / (double)frequencia.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
#endif
}

double medir_tempo_medio(FuncaoOrdenacao algoritmo, RegistroImagem **base, int n, int repeticoes) {
    RegistroImagem **copia = malloc(n * sizeof(RegistroImagem *));
    if (copia == NULL) {
        return -1.0;
    }

    // custo so de copiar o array (para descontar depois)
    double t0 = agora_segundos();
    for (int r = 0; r < repeticoes; r++) {
        memcpy(copia, base, n * sizeof(RegistroImagem *));
    }
    double custo_copia = agora_segundos() - t0;

    // custo de copiar + ordenar
    t0 = agora_segundos();
    for (int r = 0; r < repeticoes; r++) {
        memcpy(copia, base, n * sizeof(RegistroImagem *));
        algoritmo(copia, 0, n - 1);
    }
    double custo_total = agora_segundos() - t0;

    free(copia);

    double media = (custo_total - custo_copia) / repeticoes;
    return media > 0 ? media : 0.0;
}