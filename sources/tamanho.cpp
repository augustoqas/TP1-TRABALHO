#include "tamanho.hpp"

const string Tamanho::GRANDE = "GRANDE";
const string Tamanho::MEDIO = "MEDIO";
const string Tamanho::PEQUENO = "PEQUENO";

bool Tamanho::validar(string valor) {
    if (valor == GRANDE || valor == MEDIO || valor == PEQUENO){
        return true;
    }
    return false;
}

bool Tamanho::setValor(string valor) {
    if (!validar(valor)){
        return false;
    }
    this -> valor = valor;
    return true;
}
