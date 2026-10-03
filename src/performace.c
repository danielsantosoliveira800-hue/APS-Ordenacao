#include <time.h>
#include "performace.h"

double medir_tempo(FuncaoOrdenacao algoritmo, RegistroImagem **ponteiros, int inicio, int fim) {
    clock_t t_inicio = clock();
    algoritmo(ponteiros, inicio, fim);
    clock_t t_fim = clock();

    return (double)(t_fim - t_inicio) / CLOCKS_PER_SEC;
}