#ifndef REGISTRO_H
#define REGISTRO_H

typedef struct {
    int id_imagem;
    char timestamp[20];
    float latitude;
    float longitude;
    float nivel_desmatamento;
} RegistroImagem;

void imprimir_registro(const RegistroImagem *r);

#endif