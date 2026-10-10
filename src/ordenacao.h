#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "registro.h"

typedef enum {
    CRITERIO_DESMATAMENTO,
    CRITERIO_LATITUDE,
    CRITERIO_LONGITUDE
} CriterioOrdenacao;

extern CriterioOrdenacao criterio_atual;

void bubble_sort(RegistroImagem **ponteiros, int inicio, int fim);
void insertion_sort(RegistroImagem **ponteiros, int inicio, int fim);
void selection_sort(RegistroImagem **ponteiros, int inicio, int fim);
void merge_sort(RegistroImagem **ponteiros, int inicio, int fim);
void quick_sort(RegistroImagem **ponteiros, int inicio, int fim);
void heap_sort(RegistroImagem **ponteiros, int inicio, int fim);

#endif