#ifndef SERVER_H
#define SERVER_H


#include <map>
#include <string>
#include <vector>

#include "../common_src/armamento.h"
#include "../common_src/protocol.h"
#include "../common_src/socket.h"
class Server {

private:
    Socket socket;
    Armamento inventario;
    Protocol protocol;

    // Actualiza el arma principal o secundaria segun la compra recibida. Setea el valor de las
    // municiones de dicha arma a 0
    void actualizar_armas(const uint8_t& compra, std::vector<std::string>& string_compras);

    // Actualiza la municion principal o secundaria segun la compra recibida. Agrega la cantidad
    // de municiones segun corresponda al tipo de arma. Si el tipo de arma en cuestion no estaba
    // equipada, no hace nada.
    void actualizar_municion(const uint8_t& compra, std::vector<std::string>& string_compras);

    // Actualiza el estado del cuchillo segun la compra recibida. Si estaba equipado lo desequipa,
    // y viceversa
    void actualizar_cuchillo(std::vector<std::string>& string_compras);

    // Recibe las compras y realiza las actualizaciones pertinentes segun si se realizo un pedido de
    //  compra de arma (principal o secundaria), municion (principal o secundaria) o cuchillo
    std::vector<std::string> actualizar_inventario(const std::vector<uint8_t>& compras);

    // Envia el primer mensaje desde el servidor al cliente con el mensaje de bienvenida:
    //"What arya buying?". En caso de que se produzca un error, devuelve false
    bool enviar_mensaje_inicial();

public:
    explicit Server(const char* servname);

    // Elimino el constructor por copia y seteo a default el constructor por movimiento
    Server(const Server&) = delete;
    Server(Server&&) = default;

    // Elimino el operador de asignacion por copia y seteo a default el operador de asignacion por
    // movimiento
    Server& operator=(const Server&) = delete;
    Server& operator=(Server&&) = default;

    // Envia el mensaje de bienvenida al cliente. Luego loopea entre recibir ordenes de compra,
    // procesarlas, actualizar el inventario y enviar el inventario actualizado. Realiza este loop
    // hasta que se produzca un error en alguno de los pasos o hasta que reciba una orden de compra
    // con el comando de terminacion: "q"
    void conexion_con_cliente();
};

#endif
