#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <iostream>
#include <string>
#include <vector>

#include "armamento.h"
#include "socket.h"

class Protocol {
private:
    Socket& socket;

    // Recibe el nombre del arma que se quiere enviar. La convierte en una tira de bytes y la envia
    // al cliente. En caso de que se produzca un error, devuelve false
    bool enviar_arma(const std::string& arma);

    // Recibe el valor de la variable que se quiere enviar (un entero entre 0 y 255 si es la
    // la municion; o 0 o 1 si es el cuchillo). Envia la informacion al cliente y si se produce un
    // error devuelve false
    bool enviar_municion_o_cuchillo(const uint8_t& info);

    /*
     * Convierte la tira de bytes recibida por el cliente en el nombre de un arma. Como por
     * protocolo los primeros 2 bytes son la longitud, en caso de que se lea una longitud igual a 0
     * devuelve false. Luego lee byte a byte hasta que se hayan leido la cantidad indicada por la
     * longitud. Si en algun punto de esta lectura se produce un error, se interrumpe y devuelve
     * false.
     */
    bool recibir_arma(std::string& msg);

    /*
     * Convierte la tira de bytes recibida por el cliente en un valor para municion o para cuchillo.
     * En caso de que se produzca alfun error en la lectura, se interrumpe y devuelve false
     */
    bool recibir_cuchillo_o_municion(std::string& msg);

    /*
     * Convierte un string en una tira de bytes. En caso de que se este llamando a la funcion desde
     * el servidor, si se esta enviando un arma o el mensaje de bienvenida, antes de convertir
     * a esos caracteres en bytes, envia la longitud del mensaje en 2 bytes en formato big endian.
     * En caso de estar llamandola desde el cliente, al final de cada pedido de compra envia un
     * byte nulo (0x00)
     */
    std::vector<uint8_t> serializar(const std::string& msg, bool server);

    /*
     * Recibe una tira de bytes y devuelve el string que lo representa segun el protocolo.
     * En caso de ser un arma, devuelve el nombre de esta. En caso de ser municion o cuchillo,
     * devuelve un string representando la cantidad leida.
     */
    std::string deserializar(const std::vector<uint8_t>& bytes);

public:
    // Elimino el constructor por copia y seteo el constructor por movimiento a default
    Protocol(const Protocol&) = delete;
    Protocol(Protocol&&) = default;
    explicit Protocol(Socket& socket);


    // Envia la informacion de la compra desde el cliente al servidor ya serializada.
    // Devuelve false si se produjo un error
    bool send_from_client(const std::string& msg);

    // Envia la informacion del estado del inventario desde el servidor al cliente ya serializada.
    // Devuelve false en caso de error
    bool send_from_server(Armamento& inventario);

    // Envia el mensaje de bienvenida desde el servidor al cliente. En caso de error devuelve false
    bool send_welcome_msg(const std::string& msg);

    // Recibe el mensaje de bienvenida en el cliente. En caso de error devuelve false
    bool receive_welcome_msg(std::string& msg);

    // Recibe la informacion serializada enviada por el servidor y la procesa para poder ser usada
    // por el cliente. Devuelve false si se prudujo un error
    bool receive_from_server(std::vector<std::string>& msg);

    // Recibe la informacion serializada enviada por el cliente y la procesa para poder ser usada
    // por el servidor. Devuelve false en caso de error.
    bool receive_from_client(std::vector<uint8_t>& compras);

    // Elimino el operador de asignacion por copia y seteo el operador de asignacion por movimiento
    // a default.
    Protocol& operator=(Protocol& protocol) = delete;
    Protocol& operator=(Protocol&&) = default;
};

#endif
