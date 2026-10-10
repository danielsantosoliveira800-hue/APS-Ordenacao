#ifndef GERADOR_H
#define GERADOR_H

#include "registro.h"

void gerar_aleatorios(RegistroImagem *dados, int n);
void escrever_arquivo(const char *caminho, RegistroImagem *dados, int n);
int  contar_linhas(const char *caminho);
void ler_arquivo(const char *caminho, RegistroImagem *dados, int n);
void embaralhar_parcial(RegistroImagem **ponteiros, int inicio, int fim, float proporcao_embaralhada);
void gerar_timestamp(char *destino, int indice);

#endif