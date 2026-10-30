#ifndef EMAIL_HPP_INCLUDED
#define EMAIL_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio que representa um endereco de email no formato parte-local@dominio.
 *
 * Regras de formato:
 * - Exatamente um caractere '@' separando parte local e dominio.
 * - Parte local: letras minusculas (a-z), digitos (0-9), ponto (.) ou hifen (-);
 *   nao pode iniciar ou terminar com ponto ou hifen; todo ponto ou hifen deve
 *   ser seguido por letra ou digito; de 1 a 64 caracteres.
 * - Dominio: uma ou mais partes separadas por ponto (.); cada parte nao vazia,
 *   contendo letras minusculas (a-z), digitos (0-9) ou hifen (-), sem iniciar
 *   ou terminar com hifen; de 1 a 255 caracteres.
 */
class Email {
    private:
        static const unsigned int MAXIMO_PARTE_LOCAL = 64;  ///< Tamanho maximo da parte local.
        static const unsigned int MAXIMO_DOMINIO = 255;     ///< Tamanho maximo do dominio.

        std::string valor;  ///< Email armazenado.

        /**
         * @brief Verifica se o email e valido.
         * @param valor Email a ser verificado.
         * @throw std::invalid_argument Se o email for invalido.
         */
        void validar(std::string valor);

        /**
         * @brief Informa se o caractere e letra minuscula (a-z) ou digito (0-9).
         * @param c Caractere a ser verificado.
         * @return true se for letra minuscula ou digito; false caso contrario.
         */
        static bool ehLetraOuDigito(char c);

        /**
         * @brief Verifica a parte local (texto antes do '@').
         * @param parteLocal Texto a ser verificado.
         * @return true se a parte local for valida; false caso contrario.
         */
        static bool validarParteLocal(const std::string& parteLocal);

        /**
         * @brief Verifica o dominio (texto depois do '@').
         * @param dominio Texto a ser verificado.
         * @return true se o dominio for valido; false caso contrario.
         */
        static bool validarDominio(const std::string& dominio);

        /**
         * @brief Verifica uma parte do dominio (texto entre pontos).
         * @param parte Texto a ser verificado.
         * @return true se a parte for valida; false caso contrario.
         */
        static bool validarParteDominio(const std::string& parte);

    public:
        /**
         * @brief Armazena o email.
         * @param valor Email no formato parte-local@dominio.
         * @throw std::invalid_argument Se o email for invalido. Nesse caso o valor anterior e mantido.
         */
        void setValor(std::string valor);

        /**
         * @brief Retorna o email armazenado.
         * @return Email armazenado.
         */
        std::string getValor() const;
};

inline std::string Email::getValor() const {
    return valor;
}

#endif // EMAIL_HPP_INCLUDED
