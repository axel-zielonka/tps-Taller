#ifndef ARMAMENTO_H
#define ARMAMENTO_H

#include <iostream>
#include <string>
#include <vector>

class Armamento {
private:
    std::string arma_principal;
    std::string arma_secundaria;
    uint8_t municion_principal;
    uint8_t municion_secundaria;
    uint8_t cuchillo;

public:
    Armamento();

    // Actualiza el arma principal/secundaria. Devuelve el nombre del arma que se equipo
    std::string actualizar_arma(const std::string& arma, bool arma_principal);

    // Actualiza la municion principal/secundaria. Devuelve la cantidad de balas del arma en
    // cuestion que se poseen actualmente. En caso de que se quiera actualizar las municiones de un
    // arma no equipada, devuelve 0
    uint8_t actualizar_municion(uint8_t municiones, bool municion_principal);

    // Actualiza el cuchillo y devuelve true o false segun si se tiene equipado actualmente o no
    bool actualizar_cuchillo();

    // Sobrecarga del operador << para poder imprimir el estado del Armamento en un solo lugar
    friend std::ostream& operator<<(std::ostream& out, const Armamento& a) {
        out << "Primary Weapon: " << a.arma_principal << " | Ammo: " << (int)a.municion_principal
            << std::endl;
        out << "Secondary Weapon: " << a.arma_secundaria
            << " | Ammo: " << (int)a.municion_secundaria << std::endl;
        out << "Knife Equipped: " << ((a.cuchillo == 0x00) ? "No" : "Yes");
        return out;
    }

    std::string get_arma(bool principal) {
        return (principal ? this->arma_principal : this->arma_secundaria);
    }
    uint8_t get_municion(bool principal) {
        return (principal ? this->municion_principal : this->municion_secundaria);
    }
    uint8_t get_cuchillo() { return this->cuchillo; }

    // Devuelve un vector de strings con los nombres de todas las posibles armas que se pueden tener
    static const std::vector<std::string> armas_validas();
};


#endif
