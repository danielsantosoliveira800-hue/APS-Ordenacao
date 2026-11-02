#include <time.h>
#include "performace.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

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
    if (algoritmo == NULL || base == NULL || n <= 0 || repeticoes <= 0 ||
        (size_t)n > SIZE_MAX / sizeof(RegistroImagem *)) {
        return -1.0;
    }

    RegistroImagem **copia = malloc((size_t)n * sizeof(RegistroImagem *));
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

static int comparar_double(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
}

int medir_tempo_estatisticas(FuncaoOrdenacao algoritmo, RegistroImagem **base, int n,
                             int repeticoes, EstatisticasTempo *saida) {
    if (algoritmo == NULL || base == NULL || saida == NULL || n <= 0 || repeticoes < 1 ||
        (size_t)n > SIZE_MAX / sizeof(RegistroImagem *) ||
        (size_t)repeticoes > SIZE_MAX / sizeof(double)) {
        return -1;
    }

    RegistroImagem **copia = malloc((size_t)n * sizeof(RegistroImagem *));
    double *tempos = malloc((size_t)repeticoes * sizeof(double));
    if (copia == NULL || tempos == NULL) {
        free(copia);
        free(tempos);
        return -1;
    }

    // 1) cada repeticao: restaura o array (fora da medicao) e cronometra so a ordenacao
    for (int r = 0; r < repeticoes; r++) {
        memcpy(copia, base, n * sizeof(RegistroImagem *));
        double t0 = agora_segundos();
        algoritmo(copia, 0, n - 1);
        tempos[r] = agora_segundos() - t0;
    }

    // 2) media
    double soma = 0.0;
    for (int r = 0; r < repeticoes; r++) soma += tempos[r];
    double media = soma / repeticoes;

    // 3) desvio-padrao amostral (divide por n-1)
    double soma_quad = 0.0;
    for (int r = 0; r < repeticoes; r++) {
        double d = tempos[r] - media;
        soma_quad += d * d;
    }
    double desvio = repeticoes > 1 ? sqrt(soma_quad / (repeticoes - 1)) : 0.0;

    // 4) ordena os tempos para achar mediana, minimo e maximo
    qsort(tempos, repeticoes, sizeof(double), comparar_double);
    double mediana = (repeticoes % 2 == 1)
        ? tempos[repeticoes / 2]
        : (tempos[repeticoes / 2 - 1] + tempos[repeticoes / 2]) / 2.0;

    saida->repeticoes = repeticoes;
    saida->media = media;
    saida->mediana = mediana;
    saida->desvio_padrao = desvio;
    saida->minimo = tempos[0];
    saida->maximo = tempos[repeticoes - 1];

    free(copia);
    free(tempos);
    return 0;
}