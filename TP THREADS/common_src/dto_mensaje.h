#ifndef DTO_MENSAJE_H
#define DTO_MENSAJE_H

#include <iostream>
#include <string>
#include <vector>

#define TAMANIO_DTO_CLIENT 1
#define TAMANIO_DTO_SERVER 2
#define DUMMY 0xFF

// DTO para enviar y recibir mensajes a través de los Sockets. Como se usa tanto para los mensajes
// que se envian desde el cliente como desde el servidor, el valor de reward es inicializado con un
// valor dummy (0xFF) y se modifica en caso de ser necesario
typedef struct DTO_Mensaje {
    std::vector<uint8_t> codigo;  // cppcheck-suppress unusedStructMember
    std::string texto;            // cppcheck-suppress unusedStructMember
    uint8_t reward;               // cppcheck-suppress unusedStructMember

    explicit DTO_Mensaje(int size): codigo(size), reward(DUMMY) {}
} DTO_Mensaje_t;

#endif
