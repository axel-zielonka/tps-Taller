#include "sender.h"

#include "../common_src/liberror.h"

Sender::Sender(Socket& skt, Queue<Mensaje>& q, std::atomic<bool>& is_alive):
        protocol(skt, false), queue(q), is_alive(is_alive) {}

void Sender::run() {
    while (is_alive) {
        try {
            Mensaje msg = queue.pop();
            DTO_Mensaje mensaje_a_enviar(TAMANIO_DTO_SERVER);
            mensaje_a_enviar.codigo = msg.get_codigo();
            mensaje_a_enviar.texto = msg.get_texto();
            mensaje_a_enviar.reward = msg.get_reward();
            if (!protocol.send_from_server(mensaje_a_enviar)) {
                break;
            }
        } catch (const ClosedQueue&) {
            break;
        }
    }
}

Sender::~Sender() { this->join(); }
