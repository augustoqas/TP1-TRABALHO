#include "limite.hpp"

bool Limite::validar(int valor) {
    if (valor < MINIMO || valor > MAXIMO){
        return false;
    }
    return true;
}

bool Limite::setValor(int valor) {
    if (!validar(valor)){
        return false;
    }
    this -> valor = valor;
    return true;
}
