#ifndef SENHA_HPP_INCLUDED
#define SENHA_HPP_INCLUDED

#include <string>

/**
 * @brief Dominio que representa uma senha.
 *
 * Regras de formato:
 * - Exatamente 5 caracteres.
 * - Cada caractere e letra (a-z ou A-Z) ou digito (0-9).
 * - Existe pelo menos uma letra e pelo menos um digito.
 *
 * Exemplos validos: "Abc12", "1a2b3". Exemplos invalidos: "abcde", "12345", "ab#12", "abc1".
 */
class Senha {
    private:
        static const std::string::size_type TAMANHO = 5;
        std::string valor;
        void validar(std::string);
    public:
        /**
         * @brief Armazena a senha, se for valida.
         * @param valor Texto com 5 letras/digitos, com pelo menos uma letra e um digito.
         * @throw std::invalid_argument Se o valor for invalido. Nesse caso o valor anterior e mantido.
         */
        void setValor(std::string valor);

        /**
         * @brief Retorna a senha armazenada.
         * @return Senha armazenada.
         */
        std::string getValor() const;
};

inline std::string Senha::getValor() const {
    return valor;
}

#endif // SENHA_HPP_INCLUDED
