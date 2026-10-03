#include <stdlib.h>
#include "ordenacao.h"

void bubble_sort(RegistroImagem **ponteiros, int inicio, int fim) {
    for (int i = inicio; i < fim; i++) {
        for (int j = inicio; j < fim - (i - inicio); j++) {
            if (ponteiros[j]->nivel_desmatamento > ponteiros[j + 1]->nivel_desmatamento) {
                RegistroImagem *temp = ponteiros[j];
                ponteiros[j] = ponteiros[j + 1];
                ponteiros[j + 1] = temp;
            }
        }
    }
}

void insertion_sort(RegistroImagem **ponteiros, int inicio, int fim) {
    for (int i = inicio + 1; i <= fim; i++) {
        RegistroImagem *atual = ponteiros[i];
        int j = i - 1;

        while (j >= inicio && ponteiros[j]->nivel_desmatamento > atual->nivel_desmatamento) {
            ponteiros[j + 1] = ponteiros[j];
            j--;
        }

        ponteiros[j + 1] = atual;
    }
}

void selection_sort(RegistroImagem **ponteiros, int inicio, int fim) {
    for (int i = inicio; i < fim; i++) {
        int indice_menor = i;

        for (int j = i + 1; j <= fim; j++) {
            if (ponteiros[j]->nivel_desmatamento < ponteiros[indice_menor]->nivel_desmatamento) {
                indice_menor = j;
            }
        }

        if (indice_menor != i) {
            RegistroImagem *temp = ponteiros[i];
            ponteiros[i] = ponteiros[indice_menor];
            ponteiros[indice_menor] = temp;
        }
    }
}

static void merge(RegistroImagem **ponteiros, int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    RegistroImagem **esquerda = malloc(n1 * sizeof(RegistroImagem *));
    RegistroImagem **direita = malloc(n2 * sizeof(RegistroImagem *));

    for (int i = 0; i < n1; i++) esquerda[i] = ponteiros[inicio + i];
    for (int j = 0; j < n2; j++) direita[j] = ponteiros[meio + 1 + j];

    int i = 0, j = 0, k = inicio;

    while (i < n1 && j < n2) {
        if (esquerda[i]->nivel_desmatamento <= direita[j]->nivel_desmatamento) {
            ponteiros[k] = esquerda[i];
            i++;
        } else {
            ponteiros[k] = direita[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        ponteiros[k] = esquerda[i];
        i++;
        k++;
    }

    while (j < n2) {
        ponteiros[k] = direita[j];
        j++;
        k++;
    }

    free(esquerda);
    free(direita);
}

void merge_sort(RegistroImagem **ponteiros, int inicio, int fim) {
    if (inicio >= fim) {
        return;
    }

    int meio = (inicio + fim) / 2;

    merge_sort(ponteiros, inicio, meio);
    merge_sort(ponteiros, meio + 1, fim);
    merge(ponteiros, inicio, meio, fim);
}

static int particionar(RegistroImagem **ponteiros, int inicio, int fim) {
    int indice_aleatorio = inicio + rand() % (fim - inicio + 1);
    RegistroImagem *temp_pivo = ponteiros[indice_aleatorio];
    ponteiros[indice_aleatorio] = ponteiros[fim];
    ponteiros[fim] = temp_pivo;

    float pivo = ponteiros[fim]->nivel_desmatamento;
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        if (ponteiros[j]->nivel_desmatamento <= pivo) {
            i++;
            RegistroImagem *temp = ponteiros[i];
            ponteiros[i] = ponteiros[j];
            ponteiros[j] = temp;
        }
    }

    RegistroImagem *temp = ponteiros[i + 1];
    ponteiros[i + 1] = ponteiros[fim];
    ponteiros[fim] = temp;

    return i + 1;
}
void quick_sort(RegistroImagem **ponteiros, int inicio, int fim) {
    if (inicio < fim) {
        int posicao_pivo = particionar(ponteiros, inicio, fim);

        quick_sort(ponteiros, inicio, posicao_pivo - 1);
        quick_sort(ponteiros, posicao_pivo + 1, fim);
    }
}

static void heapify(RegistroImagem **ponteiros, int inicio, int tamanho, int raiz) {
    int maior = raiz;
    int esquerda = inicio + 2 * (raiz - inicio) + 1;
    int direita = inicio + 2 * (raiz - inicio) + 2;

    if (esquerda < inicio + tamanho && ponteiros[esquerda]->nivel_desmatamento > ponteiros[maior]->nivel_desmatamento) {
        maior = esquerda;
    }

    if (direita < inicio + tamanho && ponteiros[direita]->nivel_desmatamento > ponteiros[maior]->nivel_desmatamento) {
        maior = direita;
    }

    if (maior != raiz) {
        RegistroImagem *temp = ponteiros[raiz];
        ponteiros[raiz] = ponteiros[maior];
        ponteiros[maior] = temp;

        heapify(ponteiros, inicio, tamanho, maior);
    }
}

void heap_sort(RegistroImagem **ponteiros, int inicio, int fim) {
    int n = fim - inicio + 1;

    for (int i = inicio + n / 2 - 1; i >= inicio; i--) {
        heapify(ponteiros, inicio, n, i);
    }

    for (int atual_fim = fim; atual_fim > inicio; atual_fim--) {
        RegistroImagem *temp = ponteiros[inicio];
        ponteiros[inicio] = ponteiros[atual_fim];
        ponteiros[atual_fim] = temp;

        heapify(ponteiros, inicio, atual_fim - inicio, inicio);
    }
}