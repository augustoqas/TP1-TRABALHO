#ifndef LIMITE_HPP_INCLUDED
#define LIMITE_HPP_INCLUDED

#include <stdexcept>

/**
 * @brief Dominio de limite de WIP de um quadro.
 *
 * O limite e o numero maximo de cartoes que podem estar ao mesmo tempo na
 * coluna FAZENDO.
 *
 * Formato: inteiro de 1 a 25.
 *
 * Ex.: 5
 */
class Limite {
    private:
        static const int MINIMO = 1;
        static const int MAXIMO = 25;

        int valor = 0;  // 0 = ainda nao definido (nenhum valor valido e 0).

        void validar(int valor);

    public:
        /**
         * @brief Guarda o limite.
         * @param valor Inteiro de 1 a 25.
         * @throw std::invalid_argument Se o valor estiver fora do intervalo (o valor anterior fica).
         */
        void setValor(int valor);

        /** @brief Retorna o limite (0 se ainda nao foi definido). */
        int getValor() const;
};

inline int Limite::getValor() const {
    return valor;
}

#endif // LIMITE_HPP_INCLUDED
