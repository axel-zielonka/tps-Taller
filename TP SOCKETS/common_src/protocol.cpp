#include <iostream>
#include <string>
#include <vector>

#include <netinet/in.h>

#include "protocol.h"

Protocol::Protocol(Socket& skt): socket(skt) {}

std::vector<uint8_t> Protocol::serializar(const std::string& msg, bool server) {
    std::vector<uint8_t> bytes_codificados;
    if (server) {
        // Si el tamaño es mayor a 1 quiere decir que estoy enviando texto o arma.
        if (msg.size() > 1) {
            uint16_t size = msg.size();
            // Convierto el size al endianness del host.
            size = htons(size);
            // Mascaras para poder dividir la longitud en 2 bytes
            uint8_t msb = (uint8_t)((size & 0xFF00) >> 8);
            uint8_t lsb = (uint8_t)(size & 0xFF);
            bytes_codificados.push_back(lsb);
            bytes_codificados.push_back(msb);
        }
    }
    for (size_t i = 0; i < msg.size(); i++) {
        bytes_codificados.push_back(toascii(msg[i]));
    }
    if (!server)
        bytes_codificados.push_back(0x00);
    return bytes_codificados;
}

bool Protocol::send_from_client(const std::string& msg) {
    bool was_closed;
    std::vector<uint8_t> bytes = serializar(msg, false);
    socket.sendall(bytes.data(), bytes.size(), &was_closed);
    return !was_closed;
}

bool Protocol::enviar_arma(const std::string& arma) {
    bool was_closed = false;
    std::vector<uint8_t> bytes = serializar(arma, true);
    socket.sendall(bytes.data(), bytes.size(), &was_closed);
    return !was_closed;
}

bool Protocol::enviar_municion_o_cuchillo(const uint8_t& info) {
    bool was_closed = false;
    std::vector<uint8_t> bytes(1, info);
    socket.sendall(bytes.data(), bytes.size(), &was_closed);
    return !was_closed;
}

bool Protocol::send_from_server(Armamento& inventario) {
    return (enviar_arma(inventario.get_arma(true)) &&
            enviar_municion_o_cuchillo(inventario.get_municion(true)) &&
            enviar_arma(inventario.get_arma(false)) &&
            enviar_municion_o_cuchillo(inventario.get_municion(false)) &&
            enviar_municion_o_cuchillo(inventario.get_cuchillo()));
}

bool Protocol::recibir_arma(std::string& msg) {
    bool was_closed = false;
    uint16_t length;
    socket.recvall(&length, sizeof(length), &was_closed);

    if (length == 0 || was_closed)
        return false;

    // Convierto la longitud al endianness del network (Big Endian)
    length = ntohs(length);
    uint8_t leido;
    std::vector<uint8_t> bytes_leidos;
    while (length > 0 && !was_closed) {
        socket.recvall(&leido, sizeof(leido), &was_closed);
        bytes_leidos.push_back(leido);
        length--;
    }

    msg = deserializar(bytes_leidos);
    return !was_closed;
}

bool Protocol::recibir_cuchillo_o_municion(std::string& msg) {
    bool was_closed = false;
    uint8_t leido;
    socket.recvall(&leido, sizeof(leido), &was_closed);
    if (was_closed)
        return false;
    std::vector<uint8_t> bytes;
    bytes.push_back(leido);
    msg = deserializar(bytes);
    return true;
}

std::string Protocol::deserializar(const std::vector<uint8_t>& bytes) {
    std::string texto;
    if (bytes.size() == 1) {
        texto = std::to_string((int)bytes[0]);
    } else {
        for (size_t i = 0; i < bytes.size(); i++) {
            texto += (char)bytes[i];
        }
    }
    return texto;
}

bool Protocol::receive_from_server(std::vector<std::string>& msg) {
    return (recibir_arma(msg[0]) && recibir_cuchillo_o_municion(msg[1]) && recibir_arma(msg[2]) &&
            recibir_cuchillo_o_municion(msg[3]) && recibir_cuchillo_o_municion(msg[4]));
}

bool Protocol::receive_welcome_msg(std::string& msg) { return recibir_arma(msg); }

bool Protocol::send_welcome_msg(const std::string& msg) { return enviar_arma(msg); }

bool Protocol::receive_from_client(std::vector<uint8_t>& compras) {
    bool was_closed = false;
    uint8_t leido;

    while (true) {
        socket.recvall(&leido, sizeof(leido), &was_closed);

        if (was_closed) {
            break;
        }

        if (leido == 0x00 || leido == 0x71) {
            break;
        }

        if (leido != 0x42) {
            compras.push_back(leido);
        }
    }
    return !was_closed;
}
