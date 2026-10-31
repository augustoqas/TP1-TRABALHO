#ifndef PRIORIDADE_HPP_INCLUDED
#define PRIORIDADE_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio de prioridade de um cartao de atividade.
 *
 * Formato: ALTA, MEDIA ou BAIXA, em maiusculas e sem acento.
 *
 * Ex.: "MEDIA"
 */
class Prioridade {
    private:
        static const std::string ALTA;
        static const std::string MEDIA;
        static const std::string BAIXA;

        std::string valor;

        void validar(std::string valor);

    public:
        /**
         * @brief Guarda a prioridade.
         * @param valor ALTA, MEDIA ou BAIXA.
         * @throw std::invalid_argument Se o valor nao for uma das prioridades validas (o valor anterior fica).
         */
        void setValor(std::string valor);

        /** @brief Retorna a prioridade. */
        std::string getValor() const;
};

inline std::string Prioridade::getValor() const {
    return valor;
}

#endif // PRIORIDADE_HPP_INCLUDED
