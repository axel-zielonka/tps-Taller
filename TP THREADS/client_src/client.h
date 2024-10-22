#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>

#include "../common_src/entrada.h"
#include "../common_src/protocol.h"
#include "../common_src/socket.h"

#define MENSAJE_BIENVENIDA "What is your name?"
#define RW_CLOSE 2

class Client {
private:
    std::string nombre;  // cppcheck-suppress unusedStructMember
    Socket s;
    Protocol protocol;

    // Lee la entrada standard y devuelve un struct entrada_t que contiene el comando y la
    // informacion del mismo, a menos que sea 'Exit'
    entrada_t leer_entrada();

    // Recibe el comando ingresado por el usuario y realiza las operaciones correspondientes. En
    // caso de un 'Read' lee <n> veces del socket. En caso de un 'Pickup' envia la informacion al
    // servidor. En caso de un 'Exit', termina la ejecucion
    void procesar_entrada(const entrada_t& entrada, bool* seguir_leyendo);

    // Lee la entrada standard y ejecuta los comandos segun corresponda. No termina hasta no recibir
    // 'Exit'
    void conexion_con_el_server();

    // Recibe una linea leida por entrada standard y devuelve true si es un comando valido segun el
    // protocolo. Los comandos validos son:
    //      * Pickup <box_id>
    //      * Read <n>
    //      * Exit
    bool entrada_valida(const std::string& linea, entrada_t* entrada);

    std::string obtener_nombre();

public:
    explicit Client(const char* hostname, const char* servname);
    Client(const Client&) = delete;
    Client(Client&&) = default;

    // Ejecuta toda la logica del cliente. Conecta con el servidor, y loopea pidiendo comandos al
    // cliente
    void start();

    ~Client();

private:
    Client& operator=(const Client&) = delete;
    Client& operator=(Client&&) = default;
};


#endif
