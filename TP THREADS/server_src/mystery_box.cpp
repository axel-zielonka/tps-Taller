#include "mystery_box.h"

static std::map<uint8_t, uint8_t> iteraciones = {{BOX_ID_BAZOOKA, LOOP_BAZOOKA},
                                                 {BOX_ID_CHAINSAW, LOOP_CHAINSAW},
                                                 {BOX_ID_DEATHRAY, LOOP_DEATHRAY},
                                                 {BOX_ID_SHOTGUN, LOOP_SHOTGUN}};

MysteryBox::MysteryBox(uint8_t id):
        id(id),
        iteraciones_hasta_respawn(iteraciones.at(id)),
        turnos_hasta_respawn_restantes(iteraciones_hasta_respawn),
        puede_ser_agarrada(true) {}

bool MysteryBox::pasar_iteracion() {
    if (puede_ser_agarrada)
        return false;
    this->turnos_hasta_respawn_restantes--;
    if (this->turnos_hasta_respawn_restantes == CERO) {
        turnos_hasta_respawn_restantes = iteraciones_hasta_respawn;
        puede_ser_agarrada = true;
        return true;
    }
    return false;
}

bool MysteryBox::comprar() {
    if (puede_comprarse()) {
        puede_ser_agarrada = !puede_ser_agarrada;
        turnos_hasta_respawn_restantes = iteraciones_hasta_respawn;
        return true;
    } else {
        return false;
    }
}
