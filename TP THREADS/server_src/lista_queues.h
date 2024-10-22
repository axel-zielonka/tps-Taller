#ifndef LISTA_QUEUES_H
#define LISTA_QUEUES_H

#include <list>
#include <mutex>
#include <string>
#include <utility>

#include "../common_src/mensaje.h"

#include "queue.h"

class ListaQueues {
private:
    std::mutex mtx;
    std::list<std::pair<Queue<Mensaje>&, int>>
            lista_queues;  // cppcheck-suppress unusedStructMember

public:
    ListaQueues();

    // Recibe un mensaje y lo agrega a todas las queues de clientes que tiene en su lista, a menos
    // que esten cerradas
    void broadcast(const Mensaje& msg);

    // Agrega una nueva queue a la lista de queues, con su id correspondiente
    void agregar_queue(Queue<Mensaje>& q, int id_client);

    // Elimina la queue con el id recibido de la lista
    void eliminar_queue(int id_client);
};


#endif
