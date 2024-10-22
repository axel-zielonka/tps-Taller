#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <list>

#include "../common_src/mensaje.h"

#include "lista_queues.h"
#include "mystery_box.h"
#include "queue.h"
#include "thread.h"

#define SLEEP 200
#define BOX_RESPAWN "A new box has appeared"
#define BYTE_1 0x06
#define BYTE_2 0x05
#define EXCEPCION_AL_CARGAR_MENSAJE "Excepcion en la traduccion del mensaje. "

class Processor: public Thread {
private:
    Queue<Mensaje>& queue;
    bool juego_activo;  // cppcheck-suppress unusedStructMember
    ListaQueues& queues_clientes;
    std::list<MysteryBox> reward_boxes;  // cppcheck-suppress unusedStructMember

    // Hace un try_pop() de la queue de mensajes. Obtiene el id de la compra que se quiere realizar
    // e intenta comprar dicha caja. En caso de que se pueda comprar, la compra y notifica a todos
    // los clientes que se realizo dicha compra
    void chequear_compras();

    // Revisa para todas las cajas si en esa iteracion respawneo, en caso de que asi sea, notifica a
    // todos los clientes
    void respawnear_cajas();

    // Imprime por pantalla el mensaje y envia a todos los clientes la informacion del mensaje
    // recibido
    void broadcast(Mensaje& msg);

public:
    explicit Processor(Queue<Mensaje>& q, ListaQueues& l);

    // Realiza el loop entre chequear_compras() y respawnear_cajas() hasta que el juego no se este
    // jugando. Luego de cada iteracion, duerme durante 200 milisegundos
    virtual void run() override;

    // Cierra la queue de mensajes.
    void terminar_juego() { this->juego_activo = false; }

    ~Processor();
};


#endif
