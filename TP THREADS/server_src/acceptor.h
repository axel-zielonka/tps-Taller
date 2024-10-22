#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <atomic>
#include <list>

#include "../common_src/mensaje.h"
#include "../common_src/socket.h"

#include "client_thread.h"
#include "lista_queues.h"
#include "queue.h"
#include "thread.h"

#define RW_CLOSE 2
#define EXCEPCION_ESPERADA "Excepcion esperada: "

class Acceptor: public Thread {
private:
    Socket skt;
    std::list<ClientThread*> clients;  // cppcheck-suppress unusedStructMember
    std::atomic<bool> aceptando_clientes;
    Queue<Mensaje>& queue_juego;
    ListaQueues& queues_clientes;

    // Recorre la lista de clientes y elimina a los clientes que ya no se encuentran activos,
    // asegurando que no hay memoria ocupada por threads que ya terminaron
    void recolectar();

    // Corta la conexion del thread con el cliente verdadero, elimina su queue de mensajes y luego
    // elimina al thread
    void eliminar_cliente(ClientThread* client);

public:
    explicit Acceptor(const char* servname, Queue<Mensaje>& q, ListaQueues& l);

    // Realiza el loop aceptando nuevos clientes y lanzando nuevos hilos para manejar a cada nuevo
    // cliente. En cada iteracion se encarga de eliminar a los clientes que ya hayan terminado
    virtual void run() override;

    // Cambia el atomics 'aceptando_clientes' a false para no aceptar nuevos clientes
    void dejar_de_aceptar();

    virtual ~Acceptor();
};

#endif
