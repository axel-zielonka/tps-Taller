#include <iostream>
#include <string>

#include "client_src/client.h"

#define SUCCESS 0
#define ERROR 1

#define ARGUMENTOS_ESPERADOS 3
#define ARG_ARCHIVO 0
#define ARG_HOSTNAME 1
#define ARG_SERVNAME 2

#define EXCEPCION_CONOCIDA "Se produjo una excepcion de tipo: "
#define EXCEPCION_DESCONOCIDA "Se produjo una excepcion desconocida"


int main(int argc, char* argv[]) {
    try {
        if (argc != ARGUMENTOS_ESPERADOS) {
            std::cerr << "Llamada al programa invalida. Se espera: " << argv[ARG_ARCHIVO]
                      << " <hostname> <servname>" << std::endl;
            return ERROR;
        }

        const char* hostname = argv[ARG_HOSTNAME];
        const char* servname = argv[ARG_SERVNAME];

        Client client(hostname, servname);
        client.start();

        return SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << EXCEPCION_CONOCIDA << e.what() << std::endl;
        return ERROR;
    } catch (...) {
        std::cerr << EXCEPCION_DESCONOCIDA << std::endl;
        return ERROR;
    }
}
