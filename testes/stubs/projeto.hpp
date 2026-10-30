#ifndef PROJETO_HPP_INCLUDED
#define PROJETO_HPP_INCLUDED

// STUB de teste: so a chave primaria (identificador).
// Sai de cena sozinho quando headers/projeto.hpp tiver "class Projeto"
// (o rodar avisa); depois disso, apague este arquivo.
#include "identificador.hpp"

class Projeto {
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

#endif // PROJETO_HPP_INCLUDED
