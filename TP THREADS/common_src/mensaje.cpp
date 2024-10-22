#include "mensaje.h"

#include <iostream>
#include <sstream>

#include "protocol.h"

Mensaje::Mensaje():
        armas({{BOX_ID_BAZOOKA, BAZOOKA},
               {BOX_ID_CHAINSAW, CHAINSAW},
               {BOX_ID_DEATHRAY, DEATHRAY},
               {BOX_ID_SHOTGUN, SHOTGUN}}) {
    this->reward_id = DUMMY_REWARD;  // valor dummy para indicar que no fue una compra
    this->texto_msj = TEXTO_VACIO;
}

void Mensaje::set_info(const DTO_Mensaje& msg) {
    if (!Protocol::valid_info(msg))
        throw InvalidInfo();

    this->codigo_msj = msg.codigo;
    this->texto_msj = msg.texto;
    this->reward_id = msg.reward;
}

std::string Mensaje::obtener_arma(const uint8_t& reward) {
    std::map<uint8_t, std::string>::iterator it;
    it = armas.find(reward);
    if (it == armas.end())
        throw InvalidWeapon();
    return it->second;
}

void Mensaje::imprimir() {
    std::ostringstream oss;
    oss << texto_msj;
    if ((codigo_msj.size() == CODIGO_CLIENTE_SIZE) ||
        (codigo_msj.size() == CODIGO_SERVER_SIZE && codigo_msj[1] == BYTE_COMPRA)) {
        oss << PICKUP_MSG << obtener_arma(reward_id);
    }
    oss << std::endl;
    std::cout << oss.str();
}
