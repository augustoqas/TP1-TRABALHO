#ifndef ESTADO_HPP_INCLUDED
#define ESTADO_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio de estado (coluna) de um cartao de atividade.
 *
 * Formato: "A FAZER", "FAZENDO" ou "FEITO", exatamente assim.
 *
 * Ex.: "FAZENDO"
 */
class Estado {
    private:
        std::string valor;

        void validar(std::string valor);

    public:
        static const std::string A_FAZER;  ///< "A FAZER"
        static const std::string FAZENDO;  ///< "FAZENDO"
        static const std::string FEITO;    ///< "FEITO"

        /**
         * @brief Guarda o estado.
         * @param valor "A FAZER", "FAZENDO" ou "FEITO".
         * @throw std::invalid_argument Se o valor for invalido (o valor anterior fica).
         */
        void setValor(std::string valor);

        /** @brief Retorna o estado. */
        std::string getValor() const;
};

inline std::string Estado::getValor() const {
    return valor;
}

#endif // ESTADO_HPP_INCLUDED
