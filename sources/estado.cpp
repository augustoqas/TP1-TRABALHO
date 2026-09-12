#include "Estado.h"

const string Estado::A_FAZER = "A FAZER"
const string Estado::FAZENDO = "FAZENDO"
const string Estado::FEITO = "FEITO"

bool Estado::validar(string valor) {
    if (valor == A_FAZER || valor == FAZENDO || valor == FEITO){
        return true;
    }
    return false;
    
}

bool Estado::setValor(string valor) {
    if (!validar){
        return false;
    }
    this -> valor = valor;
    return true;
}
