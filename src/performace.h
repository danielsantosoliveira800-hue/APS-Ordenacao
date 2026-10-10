#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "registro.h"

typedef void (*FuncaoOrdenacao)(RegistroImagem**, int, int);

double medir_tempo(FuncaoOrdenacao algoritmo, RegistroImagem **ponteiros, int inicio, int fim);

double medir_tempo_medio(FuncaoOrdenacao algoritmo, RegistroImagem **base, int n, int repeticoes);

#endif