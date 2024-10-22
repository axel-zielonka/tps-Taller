#ifndef RECEIVER_H
#define RECEIVER_H

#include <atomic>

#include "../common_src/mensaje.h"
#include "../common_src/protocol.h"
#include "../common_src/socket.h"

#include "queue.h"
#include "thread.h"

#define EXCEPCION_CONOCIDA "Se produjo una excepcion conocida: "
#define EXCEPCION_INESPERADA "Se produjo una excepcion inesperada: "
#define EXCEPCION_DESCONOCIDA "Se produjo una exepcion desconocida"

class Receiver: public Thread {
private:
    Protocol protocol;
    Queue<Mensaje>& queue_juego;
    std::atomic<bool> alive;

public:
    explicit Receiver(Socket& skt, Queue<Mensaje>& queue_juego);

    // Recibe el mensaje del cliente y lo agrega a la queue de mensajes del procesador
    virtual void run() override;

    // Asigna el valor del atomic "alive" a false, terminando la ejecucion
    void terminar_ejecucion() { this->alive = false; }

    virtual ~Receiver();
};

#endif
