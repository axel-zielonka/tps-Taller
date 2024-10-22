#include "protocol.h"

#include <netinet/in.h>

#include "dto_mensaje.h"

Protocol::Protocol(Socket& skt, bool is_client): socket(skt), is_client(is_client) {}

bool Protocol::box_id_valido(const int& id) { return id >= ID_BAZOOKA && id <= ID_SHOTGUN; }

bool Protocol::instruccion_compra_valida(const entrada_t& entrada) {
    if (entrada.numero == CERO)
        return entrada.texto == EXIT;
    if (entrada.texto == PICKUP)
        return Protocol::box_id_valido(entrada.numero);
    else
        return entrada.numero > CERO;
}

bool Protocol::send_cte() {
    uint8_t cte = Protocol::constante_compra;
    bool was_closed = false;
    socket.sendall(&cte, sizeof(cte), &was_closed);
    return !was_closed;
}

bool Protocol::send_texto(const std::string& texto) {
    bool was_closed = false;
    uint16_t largo = texto.size();
    largo = htons(largo);
    socket.sendall(&largo, sizeof(largo), &was_closed);
    if (was_closed)
        return false;
    socket.sendall(texto.data(), texto.size(), &was_closed);
    return !was_closed;
}

bool Protocol::send_box(const uint8_t& box) {
    bool was_closed = false;
    socket.sendall(&box, sizeof(box), &was_closed);
    return !was_closed;
}

bool Protocol::send_from_client(const DTO_Mensaje& entrada) {
    return (send_cte() && send_texto(entrada.texto) && send_box(entrada.codigo[0]));
}

bool Protocol::send_from_server(DTO_Mensaje_t msg) {
    bool was_closed = false;
    if (msg.codigo[0] == BYTE_CONSTANTE_CLIENTE) {
        msg.codigo = {BYTE_INICIO_MSG, BYTE_COMPRA};
    }
    socket.sendall(msg.codigo.data(), msg.codigo.size(), &was_closed);
    if (was_closed)
        return false;

    if (msg.codigo[1] == BYTE_COMPRA) {
        return (send_texto(msg.texto) && send_box(msg.reward));
    } else {
        return true;
    }
}

bool Protocol::receive(DTO_Mensaje& msg) {
    bool was_closed = false;
    socket.recvall(msg.codigo.data(), msg.codigo.size(), &was_closed);
    if (was_closed)
        return false;

    if (msg.codigo[0] == BYTE_INICIO_MSG && msg.codigo[1] == BYTE_RESPAWN) {
        msg.texto = MENSAJE_RESPAWN;
        return true;
    }

    uint16_t length;
    socket.recvall(&length, sizeof(length), &was_closed);
    if (was_closed || length == 0)
        return false;

    length = ntohs(length);

    char leido;
    while (length > 0 && !was_closed) {
        socket.recvall(&leido, sizeof(leido), &was_closed);
        msg.texto += leido;
        length--;
    }

    return !was_closed;
}

bool Protocol::receive_from_client(DTO_Mensaje& msg) {
    if (!receive(msg))
        return false;

    bool was_closed = false;
    socket.recvall(&(msg.reward), sizeof(msg.reward), &was_closed);

    msg.reward += OFFSET_ID;

    return !was_closed;
}

bool Protocol::receive_from_server(DTO_Mensaje& msg) {
    if (!receive(msg))
        return false;
    bool was_closed = false;
    if (msg.codigo[1] == BYTE_COMPRA) {
        socket.recvall(&(msg.reward), sizeof(msg.reward), &was_closed);
    }
    return !was_closed;
}

bool Protocol::valid_info(const DTO_Mensaje& msg) {
    if (msg.codigo.size() > 2)
        return false;
    if (msg.codigo.size() == 1 && msg.codigo[0] != BYTE_CONSTANTE_CLIENTE)
        return false;
    if (msg.codigo.size() == 2 && msg.codigo[0] != BYTE_INICIO_MSG)
        return false;
    if (msg.codigo.size() == 2 && (msg.codigo[1] != BYTE_COMPRA && msg.codigo[1] != BYTE_RESPAWN))
        return false;
    if (!(msg.reward >= BOX_ID_BAZOOKA && msg.reward <= BOX_ID_SHOTGUN) &&
        msg.reward != DUMMY_REWARD)
        return false;
    return true;
}
