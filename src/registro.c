#include <stdio.h>
#include "registro.h"

void imprimir_registro(const RegistroImagem *r) {
    printf("ID: %d | Timestamp: %s | Lat: %.4f | Lon: %.4f | Desmatamento: %.2f%%\n",
           r->id_imagem, r->timestamp, r->latitude, r->longitude, r->nivel_desmatamento);
}