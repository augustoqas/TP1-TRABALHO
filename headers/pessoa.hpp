#ifndef PESSOA_HPP_INCLUDED
#define PESSOA_HPP_INCLUDED

#include "email.hpp"
#include "nome.hpp"
#include "papel.hpp"
#include "senha.hpp"

/**
 * @brief Entidade pessoa: email (chave primaria), nome, senha e papel.
 *
 * Cada atributo e um dominio que ja se valida sozinho, entao a entidade so
 * guarda e devolve copias deles.
 */
class Pessoa {
    private:
        Email email;
        Nome nome;
        Senha senha;
        Papel papel;

    public:
        /** @brief Guarda o email (chave primaria). */
        void setEmail(const Email& email);

        /** @brief Retorna o email. */
        Email getEmail() const;

        /** @brief Guarda o nome. */
        void setNome(const Nome& nome);

        /** @brief Retorna o nome. */
        Nome getNome() const;

        /** @brief Guarda a senha. */
        void setSenha(const Senha& senha);

        /** @brief Retorna a senha. */
        Senha getSenha() const;

        /** @brief Guarda o papel (GESTOR ou DESENVOLVEDOR). */
        void setPapel(const Papel& papel);

        /** @brief Retorna o papel. */
        Papel getPapel() const;
};

#endif // PESSOA_HPP_INCLUDED
