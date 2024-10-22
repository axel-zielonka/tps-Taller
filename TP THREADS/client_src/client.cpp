#include "client.h"

#include "../common_src/dto_mensaje.h"
#include "../common_src/mensaje.h"

Client::Client(const char* hostname, const char* servname):
        s(hostname, servname), protocol(s, true) {
    this->nombre = obtener_nombre();
}

std::string Client::obtener_nombre() {
    std::cout << MENSAJE_BIENVENIDA << std::endl;
    std::string nombre_leido;
    std::getline(std::cin, nombre_leido);
    return nombre_leido;
}


bool Client::entrada_valida(const std::string& linea, entrada_t* entrada) {
    size_t indice_espacio = linea.find(' ');
    if (indice_espacio == std::string::npos) {
        entrada->texto = linea;
        entrada->numero = 0;
        return protocol.instruccion_compra_valida(*entrada);
    } else {
        entrada->texto = linea.substr(0, indice_espacio);
        entrada->numero = (stoi(linea.substr(indice_espacio + 1)));
        return protocol.instruccion_compra_valida(*entrada);
    }
}

entrada_t Client::leer_entrada() {
    entrada_t entrada;
    std::string linea_leida;
    do {
        std::getline(std::cin, linea_leida);
    } while (!entrada_valida(linea_leida, &entrada));
    return entrada;
}

void Client::procesar_entrada(const entrada_t& entrada, bool* seguir_leyendo) {
    if (entrada.texto == EXIT) {
        *seguir_leyendo = false;
        return;
    }
    if (entrada.texto == PICKUP) {
        DTO_Mensaje msg(TAMANIO_DTO_CLIENT);
        msg.texto = this->nombre;
        msg.codigo[0] = entrada.numero;
        protocol.send_from_client(msg);
    } else if (entrada.texto == READ) {
        int n = 0;
        while (n < entrada.numero) {
            DTO_Mensaje msg(TAMANIO_DTO_SERVER);
            protocol.receive_from_server(msg);
            Mensaje leido;
            leido.set_info(msg);
            leido.imprimir();
            n++;
        }
    }
}

void Client::conexion_con_el_server() {
    bool seguir_leyendo = true;
    do {
        entrada_t entrada;
        entrada = leer_entrada();
        procesar_entrada(entrada, &seguir_leyendo);
    } while (seguir_leyendo);
}

void Client::start() { conexion_con_el_server(); }

Client::~Client() {
    s.shutdown(RW_CLOSE);
    s.close();
}
