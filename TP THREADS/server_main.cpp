#include "server_src/server.h"

#define SUCCESS 0
#define FAIL 1
#define ARGUMENTOS_ESPERADOS 2
#define ARG_ARCHIVO 0
#define ARG_SERVNAME 1


int main(int argc, char* argv[]) {
    try {
        if (argc != ARGUMENTOS_ESPERADOS) {
            std::cerr << "Llamada al programa invalida. Se espera: " << argv[ARG_ARCHIVO]
                      << " <servname>" << std::endl;
            return FAIL;
        }

        const char* servname = argv[ARG_SERVNAME];
        Server server(servname);

        server.start();

        return SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << EXCEPCION_CONOCIDA << e.what() << std::endl;
        return FAIL;
    } catch (...) {
        std::cerr << EXCEPCION_DESCONOCIDA << std::endl;
        return FAIL;
    }
}
