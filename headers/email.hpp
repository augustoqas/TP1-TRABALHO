#ifndef EMAIL_HPP_INCLUDED
#define EMAIL_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio de email, no formato parte-local@dominio.
 *
 * Formato:
 * - Exatamente um '@'.
 * - Parte local: 1 a 64 caracteres entre a-z, 0-9, '.' e '-'. Nao comeca nem
 *   termina com '.' ou '-', e todo '.' ou '-' vem seguido de letra ou digito.
 * - Dominio: 1 a 255 caracteres, em partes separadas por '.'. Cada parte nao
 *   e vazia, usa a-z, 0-9 e '-', e nao comeca nem termina com '-'.
 *
 * Ex.: joao.silva@unb.br.
 */
class Email {
    private:
        static const std::string::size_type MAXIMO_PARTE_LOCAL = 64;
        static const std::string::size_type MAXIMO_DOMINIO = 255;

        std::string valor;

        void validar(std::string valor);
        static bool ehLetraOuDigito(char c);
        static bool validarParteLocal(const std::string& parteLocal);
        static bool validarDominio(const std::string& dominio);
        static bool validarParteDominio(const std::string& parte);

    public:
        /**
         * @brief Guarda o email.
         * @throw std::invalid_argument Se o formato for invalido (o valor anterior fica).
         */
        void setValor(std::string valor);

        /** @brief Retorna o email. */
        std::string getValor() const;
};

inline std::string Email::getValor() const {
    return valor;
}

#endif // EMAIL_HPP_INCLUDED
