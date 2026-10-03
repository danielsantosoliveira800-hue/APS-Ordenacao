#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "registro.h"

typedef void (*FuncaoOrdenacao)(RegistroImagem**, int, int);

double medir_tempo(FuncaoOrdenacao algoritmo, RegistroImagem **ponteiros, int inicio, int fim);

#endif