#include "armamento.h"

#define NOT_EQUIPPED "Not Equipped"

Armamento::Armamento() {
    this->arma_principal = NOT_EQUIPPED;
    this->arma_secundaria = NOT_EQUIPPED;
    this->municion_principal = 0x00;
    this->municion_secundaria = 0x00;
    this->cuchillo = 0x00;
}

std::string Armamento::actualizar_arma(const std::string& arma, bool principal) {
    if (principal) {
        this->arma_principal = arma;
        this->municion_principal = 0;
    } else {
        this->arma_secundaria = arma;
        this->municion_secundaria = 0;
    }
    return principal ? this->arma_principal : this->arma_secundaria;
}

uint8_t Armamento::actualizar_municion(uint8_t municiones, bool principal) {
    std::string arma = principal ? this->arma_principal : this->arma_secundaria;

    if (arma == NOT_EQUIPPED) {
        return 0;
    }
    principal ? this->municion_principal += municiones : this->municion_secundaria += municiones;
    return principal ? this->municion_principal : this->municion_secundaria;
}

bool Armamento::actualizar_cuchillo() {
    (this->cuchillo == 0x00) ? this->cuchillo = 0x01 : this->cuchillo = 0x00;
    return this->cuchillo;
}

const std::vector<std::string> Armamento::armas_validas() {
    std::vector<std::string> armas({"Not Equipped", "Banana", "Magnum", "Old Pistol",
                                    "Pew Pew Laser", "Phaser", "Chaindart"});
    return armas;
}
