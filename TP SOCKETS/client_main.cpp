#include "client_src/client.h"

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Cantidad de argumentos invalida. Se espera: ./" << argv[0];
        std::cerr << "<hostname> <servname>" << std::endl;
        return 1;
    }

    const char* hostname = argv[1];
    const char* servname = argv[2];

    Client cliente(hostname, servname);

    cliente.conexion_con_servidor();

    return 0;
}
