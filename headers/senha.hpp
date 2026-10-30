#ifndef SENHA_HPP_INCLUDED
#define SENHA_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio de senha.
 *
 * Formato: exatamente 5 letras (a-z, A-Z) ou digitos (0-9), com ao menos
 * 1 letra e 1 digito.
 *
 * Ex.: "Abc12"
 */
class Senha {
    private:
        static const std::string::size_type TAMANHO = 5;

        std::string valor;

        void validar(std::string valor);

    public:
        /**
         * @brief Guarda a senha.
         * @throw std::invalid_argument Se o formato for invalido (o valor anterior fica).
         */
        void setValor(std::string valor);

        /** @brief Retorna a senha. */
        std::string getValor() const;
};

inline std::string Senha::getValor() const {
    return valor;
}

#endif // SENHA_HPP_INCLUDED
