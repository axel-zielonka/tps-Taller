#include "client_thread.h"

#include <utility>

#include <syslog.h>

#include "../common_src/liberror.h"

ClientThread::ClientThread(Socket&& s, Queue<Mensaje>& queue, int id):
        skt(std::move(s)),
        queue_juego(CANTIDAD_MAXIMA_MENSAJES),
        is_alive(true),
        r(skt, queue),
        s(skt, queue_juego, is_alive),
        id_client(id) {}

void ClientThread::conectar_con_cliente() {
    r.start();
    s.start();
}

void ClientThread::cortar_conexion() { is_alive = false; }

ClientThread::~ClientThread() {
    s.terminar_ejecucion();
    r.terminar_ejecucion();
    queue_juego.close();
    skt.shutdown(RW_CLOSE);
    skt.close();
}
