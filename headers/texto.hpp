#ifndef TEXTO_HPP_INCLUDED
#define TEXTO_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio de texto (descricao de Projeto e de Cartao).
 *
 * Formato: ate 30 caracteres, cada um letra (A-Z, a-z), digito (0-9), espaco
 * ou pontuacao (, ; . : ? !). Comeca com letra maiuscula, termina com ponto e
 * nenhuma pontuacao vem seguida de outra pontuacao.
 *
 * Ex.: "Sistema de vendas, fase 1."
 */
class Texto {
    private:
        static const std::string::size_type TAMANHO_MAXIMO = 30;

        std::string valor;

        static bool ehPontuacao(char c);
        void validar(std::string valor);

    public:
        /**
         * @brief Guarda o texto.
         * @param valor Texto no formato descrito na classe.
         * @throw std::invalid_argument Se o formato for invalido (o valor anterior fica).
         */
        void setValor(std::string valor);

        /** @brief Retorna o texto. */
        std::string getValor() const;
};

inline std::string Texto::getValor() const {
    return valor;
}

#endif // TEXTO_HPP_INCLUDED
