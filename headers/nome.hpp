#ifndef NOME_HPP_INCLUDED
#define NOME_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio de nome, usado em Pessoa, Projeto, Quadro e Cartao.
 *
 * Formato: 1 a 15 caracteres, so letras (A-Z, a-z) e espaco. Nao comeca
 * nem termina com espaco, e todo espaco vem seguido de letra.
 *
 * Ex.: Ana Maria.
 */
class Nome {
    private:
        static const std::string::size_type TAMANHO_MAXIMO = 15;

        std::string valor;

        static bool ehLetra(char c);
        void validar(std::string valor);

    public:
        /**
         * @brief Guarda o nome.
         * @throw std::invalid_argument Se o formato for invalido (o valor anterior fica).
         */
        void setValor(std::string valor);

        /** @brief Retorna o nome. */
        std::string getValor() const;
};

inline std::string Nome::getValor() const {
    return valor;
}

#endif // NOME_HPP_INCLUDED
