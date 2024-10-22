#ifndef ENTRADA_H
#define ENTRADA_H

#include <iostream>
#include <string>
#include <vector>


// Almacena la informacion del comando ingresado por pantalla por el cliente
typedef struct entrada {
    std::string texto;  // cppcheck-suppress unusedStructMember
    uint8_t numero;     // cppcheck-suppress unusedStructMember
} entrada_t;


#endif
