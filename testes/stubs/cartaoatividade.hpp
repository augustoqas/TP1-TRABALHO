#ifndef CARTAOATIVIDADE_HPP_INCLUDED
#define CARTAOATIVIDADE_HPP_INCLUDED

// STUB de teste: so a chave primaria (identificador).
// Sai de cena sozinho quando headers/cartaoatividade.hpp tiver "class CartaoAtividade"
// (o rodar avisa); depois disso, apague este arquivo.
#include "identificador.hpp"

class CartaoAtividade {
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

#endif // CARTAOATIVIDADE_HPP_INCLUDED
