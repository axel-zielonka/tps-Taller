#ifndef MYSTERY_BOX_H
#define MYSTERY_BOX_H

#include <iostream>
#include <map>

#define BOX_ID_BAZOOKA 0x10
#define BOX_ID_CHAINSAW 0x11
#define BOX_ID_DEATHRAY 0x12
#define BOX_ID_SHOTGUN 0x13
#define LOOP_BAZOOKA 15
#define LOOP_CHAINSAW 5
#define LOOP_DEATHRAY 20
#define LOOP_SHOTGUN 10
#define CERO 0

class MysteryBox {
private:
    const uint8_t id;                     // cppcheck-suppress unusedStructMember
    const int iteraciones_hasta_respawn;  // cppcheck-suppress unusedStructMember
    int turnos_hasta_respawn_restantes;   // cppcheck-suppress unusedStructMember
    bool puede_ser_agarrada;              // cppcheck-suppress unusedStructMember

    bool puede_comprarse() { return this->puede_ser_agarrada; }

public:
    explicit MysteryBox(uint8_t id);

    // Devuelve true si la caja respawneo en la iteracion actual. Si no devuelve false
    // En caso de respawnear, asigna los turnos necesarios para volver a aparecer al correspondiente
    bool pasar_iteracion();

    // Devuelve true si la caja se peude comprar, si no devuelve false
    // Si la caja fue comprada, asigna los turnos necesarios para volver a aparecer al
    // correspondiente
    bool comprar();

    uint8_t get_id() { return this->id; }
};

#endif
