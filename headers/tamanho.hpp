#ifndef TAMANHO_HPP_INCLUDED
#define TAMANHO_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio de tamanho (esforco estimado) de um cartao de atividade.
 *
 * Formato: GRANDE, MEDIO ou PEQUENO, em maiusculas e sem acento.
 *
 * Ex.: "PEQUENO"
 */
class Tamanho {
    private:
        static const std::string GRANDE;
        static const std::string MEDIO;
        static const std::string PEQUENO;

        std::string valor;

        void validar(std::string valor);

    public:
        /**
         * @brief Guarda o tamanho.
         * @param valor GRANDE, MEDIO ou PEQUENO.
         * @throw std::invalid_argument Se o valor nao for um dos tamanhos validos (o valor anterior fica).
         */
        void setValor(std::string valor);

        /** @brief Retorna o tamanho. */
        std::string getValor() const;
};

inline std::string Tamanho::getValor() const {
    return valor;
}

#endif // TAMANHO_HPP_INCLUDED
