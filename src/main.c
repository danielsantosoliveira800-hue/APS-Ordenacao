#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "registro.h"
#include "gerador.h"
#include "ordenacao.h"
#include "performace.h"

int ler_inteiro(void) {
    int valor;
    while (scanf("%d", &valor) != 1) {
        printf("Entrada invalida. Digite um numero inteiro: ");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return valor;
}

int ler_inteiro_positivo(void) {
    int valor;
    while (1) {
        valor = ler_inteiro();
        if (valor > 0) return valor;
        printf("O numero deve ser maior que zero. Tente novamente: ");
    }
}

float ler_float(void) {
    float valor;
    while (scanf("%f", &valor) != 1) {
        printf("Entrada invalida. Digite um numero: ");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return valor;
}

float ler_float_intervalo(float min, float max) {
    float valor;
    while (1) {
        valor = ler_float();
        if (valor >= min && valor <= max) return valor;
        printf("Valor fora do intervalo permitido (%.2f a %.2f). Tente novamente: ", min, max);
    }
}

void executar_coleta_performance(void) {
    int tamanhos[] = {100, 1000, 10000, 100000};
    int qtd_tamanhos = 4;

    const char *cenarios[] = {"ordenado", "semi_ordenado", "aleatorio"};
    int qtd_cenarios = 3;

    FuncaoOrdenacao algoritmos[] = {bubble_sort, insertion_sort, selection_sort, merge_sort, quick_sort, heap_sort};
    const char *nomes_algoritmos[] = {"Bubble", "Insertion", "Selection", "Merge", "Quick", "Heap"};
    int qtd_algoritmos = 6;

    FILE *csv = fopen("data/resultados.csv", "w");
    if (csv == NULL) {
        printf("Erro ao criar o arquivo CSV. Verifique se a pasta 'data/' existe no diretorio de trabalho atual.\n");
        return;
    }
    fprintf(csv, "algoritmo,cenario,tamanho,tempo_segundos\n");

    for (int t = 0; t < qtd_tamanhos; t++) {
        int n = tamanhos[t];

        RegistroImagem *dados = malloc(n * sizeof(RegistroImagem));
        if (dados == NULL) {
            printf("Erro ao alocar memoria para %d registros. Pulando esse tamanho.\n", n);
            continue;
        }
        gerar_aleatorios(dados, n);

        RegistroImagem **base_aleatorio = malloc(n * sizeof(RegistroImagem *));
        RegistroImagem **base_ordenado = malloc(n * sizeof(RegistroImagem *));
        RegistroImagem **base_semi = malloc(n * sizeof(RegistroImagem *));

        if (base_aleatorio == NULL || base_ordenado == NULL || base_semi == NULL) {
            printf("Erro ao alocar memoria para os arrays de ponteiros. Pulando n=%d.\n", n);
            free(base_aleatorio);
            free(base_ordenado);
            free(base_semi);
            free(dados);
            continue;
        }

        for (int i = 0; i < n; i++) base_aleatorio[i] = &dados[i];

        memcpy(base_ordenado, base_aleatorio, n * sizeof(RegistroImagem *));
        merge_sort(base_ordenado, 0, n - 1);

        memcpy(base_semi, base_ordenado, n * sizeof(RegistroImagem *));
        embaralhar_parcial(base_semi, 0, n - 1, 0.4f);

        RegistroImagem **bases[] = {base_ordenado, base_semi, base_aleatorio};

        for (int c = 0; c < qtd_cenarios; c++) {
            for (int a = 0; a < qtd_algoritmos; a++) {
                RegistroImagem **copia = malloc(n * sizeof(RegistroImagem *));
                if (copia == NULL) {
                    printf("Erro ao alocar memoria para copia. Pulando essa execucao.\n");
                    continue;
                }

                memcpy(copia, bases[c], n * sizeof(RegistroImagem *));

                double tempo = medir_tempo(algoritmos[a], copia, 0, n - 1);

                printf("%s | %s | n=%d | %.6f s\n", nomes_algoritmos[a], cenarios[c], n, tempo);
                fprintf(csv, "%s,%s,%d,%.6f\n", nomes_algoritmos[a], cenarios[c], n, tempo);

                free(copia);
            }
        }

        free(base_aleatorio);
        free(base_ordenado);
        free(base_semi);
        free(dados);
    }

    fclose(csv);
    printf("\nColeta concluida. Resultados salvos em data/resultados.csv\n");
}

float obter_valor(RegistroImagem *r) {
    switch (criterio_atual) {
        case CRITERIO_LATITUDE:
            return r->latitude;
        case CRITERIO_LONGITUDE:
            return r->longitude;
        default:
            return r->nivel_desmatamento;
    }
}

const char* nome_criterio(void) {
    switch (criterio_atual) {
        case CRITERIO_LATITUDE:
            return "Latitude";
        case CRITERIO_LONGITUDE:
            return "Longitude";
        default:
            return "Nivel de Desmatamento";
    }
}

void executar_modo_interativo(void) {
    printf("\nOrigem dos dados:\n");
    printf("1. Gerar dados aleatoriamente\n");
    printf("2. Digitar os dados manualmente\n");
    printf("3. Carregar de arquivo texto (data/dados.txt)\n");
    printf("Escolha uma opcao: ");
    int modo_entrada = ler_inteiro();

    int n;
    RegistroImagem *dados;

    if (modo_entrada == 3) {
        n = contar_linhas("data/dados.txt");
        if (n == 0) {
            printf("Arquivo data/dados.txt nao encontrado ou vazio.\n");
            return;
        }

        dados = malloc(n * sizeof(RegistroImagem));
        if (dados == NULL) {
            printf("Erro ao alocar memoria para %d registros.\n", n);
            return;
        }

        ler_arquivo("data/dados.txt", dados, n);
        printf("Carregados %d registros do arquivo.\n", n);
    } else {
        printf("\nQuantos registros voce quer usar? ");
        n = ler_inteiro_positivo();

        dados = malloc(n * sizeof(RegistroImagem));
        if (dados == NULL) {
            printf("Erro ao alocar memoria para %d registros.\n", n);
            return;
        }

        if (modo_entrada == 1) {
            gerar_aleatorios(dados, n);
        } else {
            for (int i = 0; i < n; i++) {
                dados[i].id_imagem = i + 1;
                gerar_timestamp(dados[i].timestamp, i);

                printf("\nRegistro %d de %d:\n", i + 1, n);
                printf("Latitude (-90 a 90): ");
                dados[i].latitude = ler_float_intervalo(-90.0f, 90.0f);
                printf("Longitude (-180 a 180): ");
                dados[i].longitude = ler_float_intervalo(-180.0f, 180.0f);
                printf("Nivel de desmatamento (0 a 100): ");
                dados[i].nivel_desmatamento = ler_float_intervalo(0.0f, 100.0f);
            }
        }
    }

    if (modo_entrada != 3) {
        printf("\nSalvar esses dados em data/dados.txt? (1 = sim, 2 = nao): ");
        if (ler_inteiro() == 1) {
            escrever_arquivo("data/dados.txt", dados, n);
            printf("Dados salvos em data/dados.txt (o arquivo anterior foi sobrescrito).\n");
        }
    }

    RegistroImagem **ponteiros = malloc(n * sizeof(RegistroImagem *));
    if (ponteiros == NULL) {
        printf("Erro ao alocar memoria para os ponteiros.\n");
        free(dados);
        return;
    }

    for (int i = 0; i < n; i++) {
        ponteiros[i] = &dados[i];
    }

    printf("\nCriterio de ordenacao:\n");
    printf("1. Nivel de desmatamento\n");
    printf("2. Latitude\n");
    printf("3. Longitude\n");
    printf("Escolha uma opcao: ");
    int escolha_criterio = ler_inteiro();

    switch (escolha_criterio) {
        case 2:
            criterio_atual = CRITERIO_LATITUDE;
            break;
        case 3:
            criterio_atual = CRITERIO_LONGITUDE;
            break;
        default:
            criterio_atual = CRITERIO_DESMATAMENTO;
    }

    printf("\nAlgoritmo de ordenacao:\n");
    printf("1. Bubble Sort\n");
    printf("2. Insertion Sort\n");
    printf("3. Selection Sort\n");
    printf("4. Merge Sort\n");
    printf("5. Quick Sort\n");
    printf("6. Heap Sort\n");
    printf("Escolha uma opcao: ");
    int escolha_algoritmo = ler_inteiro();

    FuncaoOrdenacao algoritmo_escolhido;
    const char *nome_algoritmo;

    switch (escolha_algoritmo) {
        case 2:
            algoritmo_escolhido = insertion_sort;
            nome_algoritmo = "Insertion Sort";
            break;
        case 3:
            algoritmo_escolhido = selection_sort;
            nome_algoritmo = "Selection Sort";
            break;
        case 4:
            algoritmo_escolhido = merge_sort;
            nome_algoritmo = "Merge Sort";
            break;
        case 5:
            algoritmo_escolhido = quick_sort;
            nome_algoritmo = "Quick Sort";
            break;
        case 6:
            algoritmo_escolhido = heap_sort;
            nome_algoritmo = "Heap Sort";
            break;
        default:
            algoritmo_escolhido = bubble_sort;
            nome_algoritmo = "Bubble Sort";
    }

    RegistroImagem **ordem_antes = malloc(n * sizeof(RegistroImagem *));
    if (ordem_antes == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(ponteiros);
        free(dados);
        return;
    }
    memcpy(ordem_antes, ponteiros, n * sizeof(RegistroImagem *));

    // --- ordena os dados (para exibir antes/depois) ---
    printf("\n--- Dados antes da ordenacao ---\n");
    for (int i = 0; i < n; i++) imprimir_registro(ponteiros[i]);

    algoritmo_escolhido(ponteiros, 0, n - 1);

    printf("\n--- Dados depois da ordenacao (%s) ---\n", nome_algoritmo);
    for (int i = 0; i < n; i++) imprimir_registro(ponteiros[i]);

    // --- mede a performance (media de varias execucoes) ---
    int repeticoes = 1;
    if (n < 5000) {
        repeticoes = 200000 / n;
        if (repeticoes < 1) repeticoes = 1;
        if (repeticoes > 10000) repeticoes = 10000;
    }

    double tempo = medir_tempo_medio(algoritmo_escolhido, ordem_antes, n, repeticoes);
    if (tempo < 0) {
        printf("Erro ao medir o tempo.\n");
        tempo = 0;
    }

    printf("\nTempo medio: %.3f microssegundos (media de %d execucoes)\n", tempo * 1e6, repeticoes);

    // --- grava o CSV para o grafico ---
    FILE *csv_interativo = fopen("data/resultado_interativo.csv", "w");
    if (csv_interativo == NULL) {
        printf("Erro ao criar data/resultado_interativo.csv\n");
    } else {
        fprintf(csv_interativo, "indice,valor_antes,valor_depois,algoritmo,tempo_segundos,criterio,repeticoes\n");
        for (int i = 0; i < n; i++) {
            fprintf(csv_interativo, "%d,%.4f,%.4f,%s,%.9f,%s,%d\n",
                    i + 1, obter_valor(ordem_antes[i]), obter_valor(ponteiros[i]),
                    nome_algoritmo, tempo, nome_criterio(), repeticoes);
        }
        fclose(csv_interativo);
        printf("Dados salvos em data/resultado_interativo.csv para gerar o grafico.\n");

        printf("\nGerar o grafico agora? (1 = sim, 2 = nao): ");
        if (ler_inteiro() == 1) {
            if (system("python scripts\\grafico_interativo.py") == 0) {
#ifdef _WIN32
                system("start data\\grafico_interativo.png");
#endif
            } else {
                printf("Nao foi possivel gerar o grafico. Verifique se o Python esta instalado e se o programa foi iniciado na pasta raiz do projeto.\n");
            }
        }
    }

    free(ordem_antes);
    free(ponteiros);
    free(dados);
}

int main(void) {
    srand((unsigned int) time(NULL));

    int opcao;

    do {
        printf("\n=== Sistema de Analise de Ordenacao ===\n");
        printf("1. Executar coleta completa de performance (gera CSV)\n");
        printf("2. Modo interativo (gerar/digitar dados e ordenar)\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        opcao = ler_inteiro();

        switch (opcao) {
            case 1:
                executar_coleta_performance();
                break;
            case 2:
                executar_modo_interativo();
                break;
            case 3:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 3);

    return 0;
}