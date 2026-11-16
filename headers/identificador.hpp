#ifndef IDENTIFICADOR_HPP_INCLUDED
#define IDENTIFICADOR_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio de identificador (chave de Projeto, Quadro e Cartao).
 *
 * Formato: exatamente 6 caracteres; os 3 primeiros sao letras (a-z, A-Z) e os
 * 3 ultimos sao digitos (0-9).
 *
 * Ex.: "PRJ001"
 */
class Identificador {
    private:
        static const std::string::size_type TAMANHO = 6;
        static const std::string::size_type LETRAS = 3;

        std::string valor;

        void validar(std::string valor);

    public:
        /**
         * @brief Guarda o identificador.
         * @param valor 3 letras seguidas de 3 digitos.
         * @throw std::invalid_argument Se o formato for invalido (o valor anterior fica).
         */
        void setValor(std::string valor);

        /** @brief Retorna o identificador. */
        std::string getValor() const;
};

inline std::string Identificador::getValor() const {
    return valor;
}

#endif // IDENTIFICADOR_HPP_INCLUDED
