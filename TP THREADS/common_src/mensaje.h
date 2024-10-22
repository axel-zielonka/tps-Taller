#ifndef MENSAJE_H
#define MENSAJE_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "dto_mensaje.h"

#define BOX_ID_BAZOOKA 0x10
#define BOX_ID_CHAINSAW 0x11
#define BOX_ID_DEATHRAY 0x12
#define BOX_ID_SHOTGUN 0x13
#define PICKUP_MSG " picked up a "
#define TEXTO_VACIO " "
#define BAZOOKA "Bazooka"
#define CHAINSAW "Chainsaw"
#define DEATHRAY "Death ray"
#define SHOTGUN "Shotgun"
#define CODIGO_CLIENTE_SIZE 1
#define CODIGO_SERVER_SIZE 2

struct InvalidInfo: public std::runtime_error {
    InvalidInfo(): std::runtime_error("Error en la informacion del mensaje") {}
};

struct InvalidWeapon: public std::runtime_error {
    InvalidWeapon(): std::runtime_error("Se ingreso un código de arma inválido") {}
};

class Mensaje {
private:
    std::vector<uint8_t> codigo_msj;       // cppcheck-suppress unusedStructMember
    std::string texto_msj;                 // cppcheck-suppress unusedStructMember
    uint8_t reward_id;                     // cppcheck-suppress unusedStructMember
    std::map<uint8_t, std::string> armas;  // cppcheck-suppress unusedStructMember

public:
    Mensaje();

    // Setea la informacion del mensaje segun corresponda
    void set_info(const DTO_Mensaje& msg);

    // Devuelve el codigo almacenado.
    std::vector<uint8_t> get_codigo() { return this->codigo_msj; }

    // Devuelve el texto almacenado
    std::string get_texto() { return this->texto_msj; }

    // Devuelve la reward almacenada
    uint8_t get_reward() { return this->reward_id; }

    // Imprime por pantalla la informacion del mensaje con el formato correcto segun si es un
    // respawn o una compra
    void imprimir();

    // Devuelve el arma correspondiente al codigo que se recibe por parametro. En caso de que se
    // ingrese un numero invalido, devuelve un caracter nulo.
    std::string obtener_arma(const uint8_t& reward);
};


#endif
