#ifndef PROJETO_HPP_INCLUDED
#define PROJETO_HPP_INCLUDED

#include "identificador.hpp"
#include "nome.hpp"
#include "texto.hpp"
#include "timestamp.hpp"

/**
 * @brief Entidade projeto: identificador (chave primaria), nome, descricao,
 *        inicio e termino.
 *
 * Cada atributo e um dominio que ja se valida sozinho, entao a entidade so
 * guarda e devolve copias deles. O gestor (quem criou) fica no conteiner, nao aqui.
 */
class Projeto {
    private:
        Identificador identificador;
        Nome nome;
        Texto descricao;
        Timestamp inicio;
        Timestamp termino;

    public:
        /** @brief Guarda o identificador (chave primaria). */
        void setIdentificador(const Identificador& identificador);

        /** @brief Retorna o identificador. */
        Identificador getIdentificador() const;

        /** @brief Guarda o nome. */
        void setNome(const Nome& nome);

        /** @brief Retorna o nome. */
        Nome getNome() const;

        /** @brief Guarda a descricao. */
        void setDescricao(const Texto& descricao);

        /** @brief Retorna a descricao. */
        Texto getDescricao() const;

        /** @brief Guarda a data e hora de inicio. */
        void setInicio(const Timestamp& inicio);

        /** @brief Retorna a data e hora de inicio. */
        Timestamp getInicio() const;

        /** @brief Guarda a data e hora de termino. */
        void setTermino(const Timestamp& termino);

        /** @brief Retorna a data e hora de termino. */
        Timestamp getTermino() const;
};

#endif // PROJETO_HPP_INCLUDED
