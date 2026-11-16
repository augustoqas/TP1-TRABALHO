#ifndef QUADRO_HPP_INCLUDED
#define QUADRO_HPP_INCLUDED

#include "identificador.hpp"
#include "limite.hpp"
#include "nome.hpp"

/**
 * @brief Entidade quadro: identificador (chave primaria), nome e limite de WIP.
 *
 * Cada atributo e um dominio que ja se valida sozinho, entao a entidade so
 * guarda e devolve copias deles. O projeto do quadro fica no conteiner, nao aqui.
 */
class Quadro {
    private:
        Identificador identificador;
        Nome nome;
        Limite limite;

    public:
        /** @brief Guarda o identificador (chave primaria). */
        void setIdentificador(const Identificador& identificador);

        /** @brief Retorna o identificador. */
        Identificador getIdentificador() const;

        /** @brief Guarda o nome. */
        void setNome(const Nome& nome);

        /** @brief Retorna o nome. */
        Nome getNome() const;

        /** @brief Guarda o limite de cartoes em FAZENDO (WIP). */
        void setLimite(const Limite& limite);

        /** @brief Retorna o limite de cartoes em FAZENDO (WIP). */
        Limite getLimite() const;
};

#endif // QUADRO_HPP_INCLUDED
