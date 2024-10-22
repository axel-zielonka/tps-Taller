#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#include "../common_src/mensaje.h"

#include "queue.h"
#include "receiver.h"
#include "sender.h"

#define RW_CLOSE 2

class ClientThread {
private:
    Socket skt;
    Queue<Mensaje> queue_juego;
    std::atomic<bool> is_alive;
    Receiver r;
    Sender s;
    int id_client;  // cppcheck-suppress unusedStructMember

public:
    explicit ClientThread(Socket&& skt, Queue<Mensaje>& queue, int id);

    // Inicia los threads receiver y sender
    void conectar_con_cliente();

    // Devuelve true si el thread actual finalizo
    bool is_dead() { return !is_alive; }

    // Cierra el socket
    void cortar_conexion();

    // Devuelve la queue de mensajes del cliente
    Queue<Mensaje>& get_queue() { return this->queue_juego; }

    // Devuelve el id del cliente
    int get_id() { return this->id_client; }

    ~ClientThread();
};


#endif
