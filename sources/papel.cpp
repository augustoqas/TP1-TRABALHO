#include "papel.hpp"

const string Papel::GESTOR = "GESTOR";
const string Papel::DESENVOLVEDOR = "DESENVOLVEDOR";

bool Papel::validar(string valor) {
    if (valor == GESTOR || valor == DESENVOLVEDOR){
        return true;
    }
    return false;
}

bool Papel::setValor(string valor) {
    if (!validar(valor)){
        return false;
    }
    this -> valor = valor;
    return true;
}
