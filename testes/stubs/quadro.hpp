#ifndef QUADRO_HPP_INCLUDED
#define QUADRO_HPP_INCLUDED

// STUB de teste: so a chave primaria (identificador).
// Sai de cena sozinho quando headers/quadro.hpp tiver "class Quadro"
// (o rodar avisa); depois disso, apague este arquivo.
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

#endif // QUADRO_HPP_INCLUDED
