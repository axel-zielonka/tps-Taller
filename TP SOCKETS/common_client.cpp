#include "armamento.h"
#include "client.h"


Client::Client(const char* hostname, const char* servname):
        socket(hostname, servname), protocol(socket) {}

bool Client::actualizar_arma(const std::string& msg, bool principal) {
    std::vector<std::string> armas = Armamento::armas_validas();
    size_t actual = 0;
    bool esta = false;
    // Recorro el vector de posibles armas validas para chequear que el nuevo arma que se desea
    // ingresar sea valida.
    while (actual < armas.size() && !esta) {
        if (msg == armas[actual]) {
            esta = true;
        } else {
            actual++;
        }
    }
    if (esta) {  // si es valida, actualizo y devuelvo true. Si no, devuelvo false.
        this->inventario.actualizar_arma(msg, principal);
        return true;
    } else {
        return false;
    }
}

bool Client::actualizar_municiones(int municiones, bool principal) {
    // en el armamento, las municiones son enteros de 1 byte sin signo --> 0 <= municiones <= 255
    if (municiones < 0 || municiones > 255) {
        return false;
    } else {
        this->inventario.actualizar_municion(municiones, principal);
        return true;
    }
}

void Client::actualizar_cuchillo(const std::string& estado_cuchillo) {
    if (std::to_string(this->inventario.get_cuchillo()) == estado_cuchillo)
        return;
    else
        this->inventario.actualizar_cuchillo();
}

bool Client::procesar_inventario(const std::vector<std::string>& armas) {
    bool armas_y_municiones_actualizadas_correctamente =
            actualizar_arma(armas[0], true) && actualizar_municiones(stoi(armas[1]), true) &&
            actualizar_arma(armas[2], false) && actualizar_municiones(stoi(armas[3]), false);
    // si alguna de las actualizaciones fue invalida, termino y devuelvo false
    if (!armas_y_municiones_actualizadas_correctamente)
        return false;
    actualizar_cuchillo(armas[4]);
    return true;
}

bool comandos_validos(std::string& comandos) {
    if (comandos == "q")
        return true;

    // Por el protocolo, todos los mensajes del usario deben comenzar con una 'B'
    if (comandos[0] != 'B')
        return false;
    if (comandos.size() < 2)
        return false;
    size_t actual = 1;
    bool entrada_valida = true;
    while (entrada_valida && actual < comandos.size()) {
        // luego de la 'B', los unicos comandos validos estan representados por numeros entre el 1 y
        // el 9 (en ASCII)
        entrada_valida = (toascii(comandos[actual]) >= 0x31 && toascii(comandos[actual]) <= 0x39);
        actual++;
    }
    return entrada_valida;
}

bool Client::recibir_mensaje_inicial() {
    std::string mensaje_inicial;
    if (!protocol.receive_welcome_msg(mensaje_inicial)) {
        return false;
    }
    std::cout << mensaje_inicial << std::endl;
    return true;
}

std::string Client::leer_entrada() {
    std::string linea;
    do {
        std::getline(std::cin, linea);
    } while (!comandos_validos(linea));
    return linea;
}

void Client::conexion_con_servidor() {
    if (!recibir_mensaje_inicial()) {
        return;
    }

    while (true) {
        std::string compra = leer_entrada();

        if (compra == "q") {
            break;
        }

        if (!protocol.send_from_client(compra)) {
            std::cerr << "Error al enviar el mensaje desde el cliente" << std::endl;
            break;
        }

        std::vector<std::string> nuevo_inventario(5, " ");
        if (!protocol.receive_from_server(nuevo_inventario)) {
            std::cerr << "Error en el cliente al recibir el mensaje del servidor" << std::endl;
            break;
        }

        if (!procesar_inventario(nuevo_inventario)) {
            std::cerr << "Se produjo un error al recibir un estado de armamento invalido";
            std::cout << std::endl;
            break;
        }

        std::cout << this->inventario << std::endl << std::endl;
    }
}
