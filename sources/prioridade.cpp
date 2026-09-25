#include "prioridade.hpp"

const string Prioridade::ALTA = "ALTA";
const string Prioridade::MEDIA = "MEDIA";
const string Prioridade::BAIXA = "BAIXA";

bool Prioridade::validar(string valor) {
    if (valor == ALTA || valor == MEDIA || valor == BAIXA){
        return true;
    }
    return false;
}

bool Prioridade::setValor(string valor) {
    if (!validar(valor)){
        return false;
    }
    this -> valor = valor;
    return true;
}
