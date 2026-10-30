#ifndef IDENTIFICADOR_HPP_INCLUDED
#define IDENTIFICADOR_HPP_INCLUDED

// STUB de teste: aceita qualquer valor (o real valida 3 letras e 3 digitos).
// Sai de cena sozinho quando headers/identificador.hpp tiver "class Identificador"
// (o rodar avisa); depois disso, apague este arquivo.
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

#endif // IDENTIFICADOR_HPP_INCLUDED
