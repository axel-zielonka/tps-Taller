#include "receiver.h"

#include <syslog.h>

#include "../common_src/dto_mensaje.h"

Receiver::Receiver(Socket& s, Queue<Mensaje>& queue):
        protocol(s, false), queue_juego(queue), alive(true) {}

void Receiver::run() {

    while (alive) {
        try {
            DTO_Mensaje msg(TAMANIO_DTO_CLIENT);
            Mensaje mensaje;
            if (protocol.receive_from_client(msg)) {
                mensaje.set_info(msg);
                queue_juego.push(mensaje);
            } else {
                return;
            }
        } catch (const InvalidInfo& e) {
            std::cerr << EXCEPCION_CONOCIDA << e.what() << std::endl;
            break;
        } catch (const ClosedQueue& e) {
            std::cerr << EXCEPCION_CONOCIDA << e.what() << std::endl;
            break;
        } catch (const std::exception& e) {
            std::cerr << EXCEPCION_INESPERADA << e.what() << std::endl;
            break;
        } catch (...) {
            std::cerr << EXCEPCION_DESCONOCIDA << std::endl;
            break;
        }
    }
}

Receiver::~Receiver() { this->join(); }
