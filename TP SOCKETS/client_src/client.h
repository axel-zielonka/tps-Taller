#ifndef CLIENT_H
#define CLIENT_H


#include <string>
#include <vector>

#include "../common_src/armamento.h"
#include "../common_src/protocol.h"
#include "../common_src/socket.h"

class Client {
private:
    Socket socket;
    Armamento inventario;
    Protocol protocol;

    // Actualiza el arma principal o secundaria del inventario segun se necesite.
    // Si el arma recibida no es valida, devuelve false
    bool actualizar_arma(const std::string& arma, bool principal);

    // Actualiza las municiones del arma principal o secundaria. Si llegara a ser un numero
    // invalido, devuelve false
    bool actualizar_municiones(int municiones, bool principal);

    // Actualiza el estado del cuchillo. Si el estado recibido es igual al estado actual
    // no hace nada
    void actualizar_cuchillo(const std::string& estado_cuchillo);

    // Recibe un vector con el estado actual en el servidor y se encarga de actualizar todos los
    // elementos del inventario. En caso de que alguna actualizacion no se pueda realizar, devuelve
    // false
    bool procesar_inventario(const std::vector<std::string>& armas);

    // Recibe el mensaje inicial del servidor --> "Whay arya buying?".
    // En caso de que ocurra un error al recibirlo, devuelve false.
    bool recibir_mensaje_inicial();

    // Lee de la entrada standard por consola y devuelve en un string lo ingresado.
    // Se asegura de que la linea leida es un comando valido segun el protocolo.
    std::string leer_entrada();

public:
    // Elimino constructor y seteo el constuctor por movimiento a default
    Client(const Client&) = delete;
    Client(Client&&) = default;
    explicit Client(const char* hostname, const char* servname);

    // Elimino el operador de asignacion por copia y seteo el de movimiento a default
    Client& operator=(const Client&) = delete;
    Client& operator=(Client&&) = default;

    // Recibe el mensaje inicial del servidor. Luego hace un loop entre leer la entrada por consola
    //  del usuario, enviar la peticion al servidor, recibir el estado actual del armamento,
    // actualizar el estado propio e imprimir por salida standard. Realiza este loop hasta que el
    // usuario ingresa el comando para salir ("q") o hasta que se produzca un error en alguno de los
    // pasos del ciclo.
    void conexion_con_servidor();
};


#endif
