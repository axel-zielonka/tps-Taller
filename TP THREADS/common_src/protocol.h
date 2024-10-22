#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <iostream>
#include <string>
#include <vector>

#include "dto_mensaje.h"
#include "entrada.h"
#include "mensaje.h"
#include "socket.h"

#define EXIT "Exit"
#define PICKUP "Pickup"
#define READ "Read"
#define MENSAJE_RESPAWN "A new box has appeared"
#define ID_BAZOOKA 1
#define ID_CHAINSAW 2
#define ID_DEATHRAY 3
#define ID_SHOTGUN 4
#define CERO 0
#define BYTE_CONSTANTE_CLIENTE 0x03
#define BYTE_COMPRA 0x04
#define BYTE_RESPAWN 0x05
#define BYTE_INICIO_MSG 0x06
#define OFFSET_ID \
    0x0F  // Los ids de las armas van del 1 al 4 y los ids de las boxes van del 16 al 19
#define DUMMY_REWARD 0xFF

class Protocol {
private:
    Socket& socket;
    const bool is_client;                          // cppcheck-suppress unusedStructMember
    static const uint8_t constante_compra = 0x03;  // cppcheck-suppress unusedStructMember

    // Chequea que el id que de la caja que se desea comprar sea uno valido segun el protocolo
    static bool box_id_valido(const int& id);

    // Envia la constante de compra
    bool send_cte();

    // Envia el largo del texto en 2 bytes en formato big endian y luego envia el texto
    // correspondiente
    bool send_texto(const std::string& nombre);

    // Envia el byte correspondiente al id de la caja
    bool send_box(const uint8_t& box);

    // Realiza la logica repetida entre los receives del server y client. Lee 2 bytes del largo,
    // luego lee el string con la informacion
    bool receive(DTO_Mensaje& msg);

public:
    explicit Protocol(Socket& skt, bool is_client);

    // Recibe la instruccion y el codigo de compra, y devuelve si es o no un comando valido
    bool instruccion_compra_valida(const entrada_t& entrada);

    // Envia la informacion de compra desde el cliente al servidor. En caso de que se produzca un
    // error devuelve false
    bool send_from_client(const DTO_Mensaje& entrada);

    // Recibe la informacion enviada desde el cliente. En caso de que se produzca un error devuelve
    // false
    bool receive_from_client(DTO_Mensaje_t& msg);

    // Envia la informacion desde el servidor. En caso de que se produzca un error devuelve false
    // El DTO se pasa por copia ya que en algunos casos es necesario cambiar el codigo del mensaje
    bool send_from_server(DTO_Mensaje_t msg);

    // Recibe la informacion enviada desde el servidor. En caso de que se produzca un error devuelve
    // false
    bool receive_from_server(DTO_Mensaje_t& msg);

    // Realiza el chequeo de los codigos del mensaje recibido y del id de la caja.
    static bool valid_info(const DTO_Mensaje& msg);
};

#endif
