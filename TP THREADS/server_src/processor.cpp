#include "processor.h"

#include <vector>

#include <syslog.h>

#include "../common_src/dto_mensaje.h"

Processor::Processor(Queue<Mensaje>& q, ListaQueues& l):
        queue(q), juego_activo(true), queues_clientes(l) {
    reward_boxes.push_back(MysteryBox(BOX_ID_BAZOOKA));
    reward_boxes.push_back(MysteryBox(BOX_ID_CHAINSAW));
    reward_boxes.push_back(MysteryBox(BOX_ID_DEATHRAY));
    reward_boxes.push_back(MysteryBox(BOX_ID_SHOTGUN));
}

void Processor::broadcast(Mensaje& msg) {
    msg.imprimir();
    queues_clientes.broadcast(msg);
}

void Processor::chequear_compras() {
    Mensaje msg;
    while (queue.try_pop(msg)) {
        uint8_t reward = msg.get_reward();
        for (auto& m: reward_boxes) {
            if (m.get_id() == reward) {
                if (m.comprar()) {
                    broadcast(msg);
                }
            }
        }
    }
}

void Processor::respawnear_cajas() {
    for (auto& m: reward_boxes) {
        if (m.pasar_iteracion()) {  // spawnea caja
            Mensaje msg;
            std::vector<uint8_t> codigo;
            codigo.push_back(BYTE_1);
            codigo.push_back(BYTE_2);
            DTO_Mensaje aux(TAMANIO_DTO_SERVER);
            aux.codigo = codigo;
            aux.texto = BOX_RESPAWN;
            msg.set_info(aux);
            broadcast(msg);
        }
    }
}

void Processor::run() {
    while (juego_activo) {
        chequear_compras();
        respawnear_cajas();
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP));
    }
}

Processor::~Processor() { this->join(); }
