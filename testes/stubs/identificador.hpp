#ifndef STUB_IDENTIFICADOR_HPP_INCLUDED
#define STUB_IDENTIFICADOR_HPP_INCLUDED

// STUB de teste: aceita qualquer valor (o real valida 3 letras e 3 digitos).
// Sai de cena sozinho quando headers/identificador.hpp tiver qualquer conteudo
// (o rodar avisa); depois disso, apague este arquivo. Guarda propria (STUB_): se o
// stub e a classe real entrarem juntos, o g++ acusa "redefinition" em vez de escolher um.
#include <string>

class Identificador {
    private:
        std::string valor;

    public:
        void setValor(std::string valor) {
            this->valor = valor;
        }
        std::string getValor() const {
            return valor;
        }
};

#endif // STUB_IDENTIFICADOR_HPP_INCLUDED
