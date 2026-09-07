#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "registro.h"
#include "gerador.h"

int main(void) {
    srand((unsigned int) time(NULL));

    int n = 5;
    RegistroImagem *dados = malloc(n * sizeof(RegistroImagem));
    gerar_aleatorios(dados, n);
    escrever_arquivo("data/dados.txt", dados, n);

    printf("--- Dados gerados ---\n");
    for (int i = 0; i < n; i++) imprimir_registro(&dados[i]);

    int n2 = contar_linhas("data/dados.txt");
    RegistroImagem *dados_lidos = malloc(n2 * sizeof(RegistroImagem));
    ler_arquivo("data/dados.txt", dados_lidos, n2);

    printf("\n--- Dados lidos de volta do arquivo (n=%d) ---\n", n2);
    for (int i = 0; i < n2; i++) imprimir_registro(&dados_lidos[i]);

    free(dados);
    free(dados_lidos);
    return 0;
}