#ifndef STUB_QUADRO_HPP_INCLUDED
#define STUB_QUADRO_HPP_INCLUDED

// STUB de teste: so a chave primaria (identificador).
// Sai de cena sozinho quando headers/quadro.hpp tiver qualquer conteudo
// (o rodar avisa); depois disso, apague este arquivo. Guarda propria (STUB_): se o
// stub e a classe real entrarem juntos, o g++ acusa "redefinition" em vez de escolher um.
#include "identificador.hpp"

class Quadro {
    private:
        Identificador identificador;

    public:
        void setIdentificador(const Identificador& identificador) {
            this->identificador = identificador;
        }
        Identificador getIdentificador() const {
            return identificador;
        }
};

#endif // STUB_QUADRO_HPP_INCLUDED
