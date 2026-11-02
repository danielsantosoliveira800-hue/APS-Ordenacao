#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ordenacao.h"

#define TAMANHO 12

typedef void (*Algoritmo)(RegistroImagem **, int, int);

typedef struct {
    const char *nome;
    Algoritmo executar;
} CasoAlgoritmo;

static const CasoAlgoritmo algoritmos[] = {
    {"Bubble Sort", bubble_sort},
    {"Insertion Sort", insertion_sort},
    {"Selection Sort", selection_sort},
    {"Merge Sort", merge_sort},
    {"Quick Sort", quick_sort},
    {"Heap Sort", heap_sort}
};

static float obter_chave(const RegistroImagem *registro, CriterioOrdenacao criterio) {
    switch (criterio) {
        case CRITERIO_LATITUDE:
            return registro->latitude;
        case CRITERIO_LONGITUDE:
            return registro->longitude;
        case CRITERIO_DESMATAMENTO:
        default:
            return registro->nivel_desmatamento;
    }
}

static void preparar_registros(RegistroImagem dados[TAMANHO], RegistroImagem *ponteiros[TAMANHO]) {
    const float desmatamento[TAMANHO] = {45, 12, 12, 99, 0, 73, 45, -2, 18, 18, 100, 3};
    const float latitude[TAMANHO] = {1, -3, 1, 5, 0, -10, 5, 2, 2, -3, 8, 0};
    const float longitude[TAMANHO] = {-50, -72, -60, -50, -73, -51, -60, -65, -65, -72, -55, -70};

    for (int i = 0; i < TAMANHO; i++) {
        dados[i].id_imagem = i + 1;
        snprintf(dados[i].timestamp, sizeof(dados[i].timestamp), "2026-01-%02d 00:00:00", i + 1);
        dados[i].nivel_desmatamento = desmatamento[i];
        dados[i].latitude = latitude[i];
        dados[i].longitude = longitude[i];
        ponteiros[i] = &dados[i];
    }
}

static int verificar_ordenacao(RegistroImagem *const ponteiros[], int inicio, int fim,
                               CriterioOrdenacao criterio) {
    for (int i = inicio + 1; i <= fim; i++) {
        if (obter_chave(ponteiros[i - 1], criterio) > obter_chave(ponteiros[i], criterio)) {
            return 0;
        }
    }
    return 1;
}

static int verificar_mesmos_ponteiros(RegistroImagem *const antes[],
                                      RegistroImagem *const depois[], int n) {
    for (int i = 0; i < n; i++) {
        int ocorrencias = 0;
        for (int j = 0; j < n; j++) {
            if (antes[i] == depois[j]) ocorrencias++;
        }
        if (ocorrencias != 1) return 0;
    }
    return 1;
}

static int testar_ordenacao_completa(const CasoAlgoritmo *caso, CriterioOrdenacao criterio) {
    RegistroImagem dados[TAMANHO];
    RegistroImagem *ponteiros[TAMANHO];
    RegistroImagem *originais[TAMANHO];
    preparar_registros(dados, ponteiros);
    memcpy(originais, ponteiros, sizeof(ponteiros));

    criterio_atual = criterio;
    caso->executar(ponteiros, 0, TAMANHO - 1);

    if (!verificar_ordenacao(ponteiros, 0, TAMANHO - 1, criterio)) {
        fprintf(stderr, "FALHOU: %s nao ordenou corretamente (criterio %d).\n", caso->nome, criterio);
        return 0;
    }
    if (!verificar_mesmos_ponteiros(originais, ponteiros, TAMANHO)) {
        fprintf(stderr, "FALHOU: %s perdeu ou duplicou registros (criterio %d).\n", caso->nome, criterio);
        return 0;
    }
    return 1;
}

static int testar_subvetor(const CasoAlgoritmo *caso) {
    RegistroImagem dados[TAMANHO];
    RegistroImagem *ponteiros[TAMANHO];
    preparar_registros(dados, ponteiros);

    RegistroImagem *primeiro = ponteiros[0];
    RegistroImagem *ultimo = ponteiros[TAMANHO - 1];
    criterio_atual = CRITERIO_DESMATAMENTO;
    caso->executar(ponteiros, 2, TAMANHO - 3);

    if (ponteiros[0] != primeiro || ponteiros[TAMANHO - 1] != ultimo) {
        fprintf(stderr, "FALHOU: %s alterou elementos fora do intervalo.\n", caso->nome);
        return 0;
    }
    if (!verificar_ordenacao(ponteiros, 2, TAMANHO - 3, CRITERIO_DESMATAMENTO)) {
        fprintf(stderr, "FALHOU: %s nao ordenou corretamente o subvetor.\n", caso->nome);
        return 0;
    }
    return 1;
}

static int testar_casos_limite(const CasoAlgoritmo *caso) {
    RegistroImagem unico = {0};
    RegistroImagem *um_elemento[] = {&unico};
    RegistroImagem *vazio[] = {NULL};

    criterio_atual = CRITERIO_DESMATAMENTO;

    /* Caso 1: vetor com um único elemento */
    caso->executar(um_elemento, 0, 0);

    if (um_elemento[0] != &unico) {
        fprintf(stderr,
                "FALHOU: %s alterou o ponteiro do vetor unitario.\n",
                caso->nome);
        return 0;
    }

    /* Caso 2: intervalo vazio */
    caso->executar(vazio, 0, -1);

    if (vazio[0] != NULL) {
        fprintf(stderr,
                "FALHOU: %s alterou o vetor vazio.\n",
                caso->nome);
        return 0;
    }

    return 1;
}

int main(void) {
    const CriterioOrdenacao criterios[] = {
        CRITERIO_DESMATAMENTO,
        CRITERIO_LATITUDE,
        CRITERIO_LONGITUDE
    };
    int total = 0;

    srand(12345);
    for (size_t a = 0; a < sizeof(algoritmos) / sizeof(algoritmos[0]); a++) {
        for (size_t c = 0; c < sizeof(criterios) / sizeof(criterios[0]); c++) {
            if (!testar_ordenacao_completa(&algoritmos[a], criterios[c])) return EXIT_FAILURE;
            total++;
        }
        if (!testar_subvetor(&algoritmos[a])) return EXIT_FAILURE;
        if (!testar_casos_limite(&algoritmos[a])) return EXIT_FAILURE;
        total += 2;
    }

    printf("OK: %d verificacoes passaram para os seis algoritmos.\n", total);
    return EXIT_SUCCESS;
}
