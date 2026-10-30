#ifndef NOME_HPP_INCLUDED
#define NOME_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio que representa um nome.
 *
 * Usado por Pessoa, Projeto, Quadro e Cartao de atividade.
 *
 * Formato valido: texto com 1 a 15 caracteres. Cada caractere pode ser
 * letra maiuscula (A-Z), letra minuscula (a-z) ou espaco em branco.
 * Espaco em branco e seguido por letra. O primeiro e o ultimo caracteres
 * nao sao espaco em branco.
 *
 * Exemplos validos: "Ana", "Joao Silva", "Ana Maria Souza".
 * Exemplos invalidos: "", " Ana", "Ana ", "Ana  Maria", "Ana1", "Ana-Maria".
 */
class Nome {
    private:
        static const std::string::size_type TAMANHO_MAXIMO = 15;  ///< Tamanho maximo do nome.

        std::string valor;  ///< Nome armazenado.

        /**
         * @brief Informa se o caractere e letra (A-Z ou a-z).
         * @param c Caractere a ser verificado.
         * @return true se for letra; false caso contrario.
         */
        static bool ehLetra(char c);

        /**
         * @brief Verifica se o nome e valido.
         * @param valor Nome a ser verificado.
         * @throw std::invalid_argument Se o nome for invalido.
         */
        void validar(std::string valor);

    public:
        /**
         * @brief Armazena o nome, se o formato for valido.
         * @param valor Nome com 1 a 15 caracteres: letras (A-Z, a-z) e
         * espacos simples entre letras.
         * @throw std::invalid_argument Se o formato for invalido. Nesse caso
         * o valor anterior e mantido.
         */
        void setValor(std::string valor);

        /**
         * @brief Retorna o nome armazenado.
         * @return Nome armazenado.
         */
        std::string getValor() const;
};

inline std::string Nome::getValor() const {
    return valor;
}

#endif // NOME_HPP_INCLUDED
