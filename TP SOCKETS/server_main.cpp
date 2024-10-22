#include <iostream>

#include "server.h"

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "Cantidad de argumentos invalida. Se espera: ./" << argv[0];
        std::cerr << "<servname>" << std::endl;
        return 1;
    }

    const char* servname = argv[1];

    Server server(servname);

    server.conexion_con_cliente();

    return 0;
}
