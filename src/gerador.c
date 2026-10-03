#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gerador.h"

// --- funções internas de apoio (não expostas no .h) ---

static float aleatorio_float(float min, float max) {
    float escala = rand() / (float)RAND_MAX;
    return min + escala * (max - min);
}

static void gerar_timestamp(char *destino, int indice) {
    struct tm base = {0};
    base.tm_year = 126;  // 2026 (anos desde 1900)
    base.tm_mon  = 0;    //(0 = janeiro)
    base.tm_mday = 1;
    base.tm_hour = 0;
    base.tm_min  = 0;
    base.tm_sec  = 0;

    time_t base_segundos = mktime(&base);
    time_t atual = base_segundos + (time_t)(indice * 0.864);

    struct tm *tm_atual = localtime(&atual);
    strftime(destino, 20, "%Y-%m-%d %H:%M:%S", tm_atual);
}

// --- funções públicas (declaradas no gerador.h) ---

void gerar_aleatorios(RegistroImagem *dados, int n) {
    for (int i = 0; i < n; i++) {
        dados[i].id_imagem = i + 1;
        gerar_timestamp(dados[i].timestamp, i);
        dados[i].latitude = aleatorio_float(-10.0f, 5.0f);
        dados[i].longitude = aleatorio_float(-73.0f, -50.0f);
        dados[i].nivel_desmatamento = aleatorio_float(0.0f, 100.0f);
    }
}

// void escrever_arquivo(...) { ... }   //
void escrever_arquivo(const char *caminho, RegistroImagem *dados, int n) {
    FILE *f = fopen(caminho, "w");
    if (f == NULL) {
        printf("Erro ao criar o arquivo: %s\n", caminho);
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(f, "%d,%s,%.4f,%.4f,%.2f\n",
                dados[i].id_imagem,
                dados[i].timestamp,
                dados[i].latitude,
                dados[i].longitude,
                dados[i].nivel_desmatamento);
    }

    fclose(f);
}

// int  contar_linhas(...)   { ... }    // próxima etapa
int contar_linhas(const char *caminho) {
    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", caminho);
        return 0;
    }

    int contador = 0;
    char linha[256];
    while (fgets(linha, sizeof(linha), f) != NULL) {
        contador++;
    }

    fclose(f);
    return contador;
}

// void ler_arquivo(...)     { ... }    // próxima etapa
void ler_arquivo(const char *caminho, RegistroImagem *dados, int n) {
    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", caminho);
        return;
    }

    char linha[256];
    int i = 0;
    while (i < n && fgets(linha, sizeof(linha), f) != NULL) {
        sscanf(linha, "%d,%19[^,],%f,%f,%f",
               &dados[i].id_imagem,
               dados[i].timestamp,
               &dados[i].latitude,
               &dados[i].longitude,
               &dados[i].nivel_desmatamento);
        i++;
    }

    fclose(f);
}

void embaralhar_parcial(RegistroImagem **ponteiros, int inicio, int fim, float proporcao_embaralhada) {
    int n = fim - inicio + 1;
    int qtd_embaralhar = (int)(n * proporcao_embaralhada);

    if (qtd_embaralhar < 2) {
        return;
    }

    int *indices = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        indices[i] = inicio + i;
    }

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    for (int i = qtd_embaralhar - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        RegistroImagem *temp = ponteiros[indices[i]];
        ponteiros[indices[i]] = ponteiros[indices[j]];
        ponteiros[indices[j]] = temp;
    }

    free(indices);
}