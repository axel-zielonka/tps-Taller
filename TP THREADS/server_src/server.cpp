#include "server.h"

#include <string>

Server::Server(const char* servname):
        queue_juego(CANTIDAD_MAXIMA_MENSAJES),
        lista_queues(),
        aceptador(servname, queue_juego, lista_queues),
        procesador(queue_juego, lista_queues) {}

void Server::comenzar_a_aceptar() { aceptador.start(); }

void Server::comenzar_juego() { procesador.start(); }

void Server::leer_entrada() {
    std::string linea;
    while (getline(std::cin, linea)) {
        if (linea == EXIT_SERVER)
            break;
    }
}

void Server::start() {
    comenzar_a_aceptar();
    comenzar_juego();
    leer_entrada();
}

Server::~Server() {
    procesador.terminar_juego();
    aceptador.dejar_de_aceptar();
}
