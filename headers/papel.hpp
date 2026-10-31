#ifndef PAPEL_HPP_INCLUDED
#define PAPEL_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio de papel de um membro do projeto.
 *
 * Formato: GESTOR ou DESENVOLVEDOR, em maiusculas.
 *
 * Ex.: "GESTOR"
 */
class Papel {
    private:
        static const std::string GESTOR;
        static const std::string DESENVOLVEDOR;

        std::string valor;

        void validar(std::string valor);

    public:
        /**
         * @brief Guarda o papel.
         * @param valor GESTOR ou DESENVOLVEDOR.
         * @throw std::invalid_argument Se o valor nao for um dos papeis validos (o valor anterior fica).
         */
        void setValor(std::string valor);

        /** @brief Retorna o papel. */
        std::string getValor() const;
};

inline std::string Papel::getValor() const {
    return valor;
}

#endif // PAPEL_HPP_INCLUDED
