#include "protocol.h"
#include "server.h"

#define MENSAJE_BIENVENIDA "What arya buying?"
#define NOT_EQUIPPED "Not Equipped"
#define BANANA "Banana"
#define MAGNUM "Magnum"
#define OLD_PISTOL "Old Pistol"
#define PP_LASER "Pew Pew Laser"
#define PHASER "Phaser"
#define CHAINDART "Chaindart"
#define P_AMMO "Primary Ammo: "
#define S_AMMO "Secondary Ammo: "
#define KNIFE "Equipped knife"
#define KNIFE_NE "Knife not equipped"
#define QUIT "q"

Server::Server(const char* servname): socket(Socket(servname).accept()), protocol(socket) {}

// Recorre el vector con los nombres de las armas posibles y devuelve el nombre del arma que se
// desea comprar
std::string compra_arma(const uint8_t& compra) {
    int indice = (int)(compra - 0x30);
    return Armamento::armas_validas()[indice];
}

void Server::actualizar_armas(const uint8_t& compra, std::vector<std::string>& string_compras) {
    std::string arma;
    bool principal;
    arma = compra_arma(compra);
    (compra < 0x34) ? principal = false : principal = true;
    string_compras.push_back(this->inventario.actualizar_arma(arma, principal));
}

void Server::actualizar_municion(const uint8_t& compra, std::vector<std::string>& string_compras) {
    std::string municiones;
    std::string municion_comprada;
    bool principal;

    (compra == 0x37) ? principal = false : principal = true;
    (compra == 0x37) ? municion_comprada = S_AMMO : municion_comprada = P_AMMO;

    int cantidad_comprada;
    principal ? cantidad_comprada = 10 : cantidad_comprada = 7;

    int cantidad_actual = this->inventario.actualizar_municion(cantidad_comprada, principal);
    municiones = std::to_string(cantidad_actual);
    string_compras.push_back(municion_comprada + municiones);
}

void Server::actualizar_cuchillo(std::vector<std::string>& string_compras) {
    bool tiene_cuchillo = this->inventario.actualizar_cuchillo();
    std::string estado_cuchillo;
    (tiene_cuchillo) ? estado_cuchillo = KNIFE : estado_cuchillo = KNIFE_NE;
    string_compras.push_back(estado_cuchillo);
}

std::vector<std::string> Server::actualizar_inventario(
        const std::vector<uint8_t>& compras_cliente) {
    std::vector<std::string> string_compras;

    for (uint8_t compra: compras_cliente) {
        if (compra < 0x37) {
            actualizar_armas(compra, string_compras);
        } else if (compra <= 0x39) {
            (compra < 0x39) ? actualizar_municion(compra, string_compras) :
                              actualizar_cuchillo(string_compras);
        } else {
            string_compras.push_back(QUIT);
        }
    }
    return string_compras;
}

// Recibe las compras hechas por el usuario y las imprime por la salida standard del servidor
void imprimir_compras(const std::vector<std::string>& informacion_compras) {
    for (const std::string& compra: informacion_compras) {
        std::cout << compra << std::endl;
    }
}

bool Server::enviar_mensaje_inicial() { return protocol.send_welcome_msg(MENSAJE_BIENVENIDA); }

void Server::conexion_con_cliente() {
    if (!enviar_mensaje_inicial()) {
        return;
    }

    while (true) {
        std::vector<uint8_t> compras_cliente;
        if (!protocol.receive_from_client(compras_cliente)) {
            break;
        }

        std::vector<std::string> informacion_compras = actualizar_inventario(compras_cliente);

        imprimir_compras(informacion_compras);
        if (informacion_compras.size() == 1 && informacion_compras[0] == "Quit") {
            break;
        }

        if (!protocol.send_from_server(this->inventario))
            break;
    }
}
