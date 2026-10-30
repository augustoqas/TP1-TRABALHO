#ifndef PESSOA_HPP_INCLUDED
#define PESSOA_HPP_INCLUDED

#include "email.hpp"
#include "nome.hpp"
#include "papel.hpp"
#include "senha.hpp"

/**
 * @brief Entidade que representa uma pessoa do sistema Kanban.
 *
 * Atributos: email (chave primaria), nome, senha e papel. Cada atributo e
 * um dominio, que ja valida o proprio valor; por isso a entidade apenas
 * armazena e retorna copias dos dominios recebidos.
 */
class Pessoa {
    private:
        Email email;  ///< Email da pessoa (chave primaria).
        Nome nome;    ///< Nome da pessoa.
        Senha senha;  ///< Senha da pessoa.
        Papel papel;  ///< Papel da pessoa (GESTOR ou DESENVOLVEDOR).

    public:
        /**
         * @brief Armazena o email (chave primaria).
         * @param email Email da pessoa.
         */
        void setEmail(const Email& email);

        /**
         * @brief Retorna o email (chave primaria).
         * @return Email da pessoa.
         */
        Email getEmail() const;

        /**
         * @brief Armazena o nome.
         * @param nome Nome da pessoa.
         */
        void setNome(const Nome& nome);

        /**
         * @brief Retorna o nome.
         * @return Nome da pessoa.
         */
        Nome getNome() const;

        /**
         * @brief Armazena a senha.
         * @param senha Senha da pessoa.
         */
        void setSenha(const Senha& senha);

        /**
         * @brief Retorna a senha.
         * @return Senha da pessoa.
         */
        Senha getSenha() const;

        /**
         * @brief Armazena o papel.
         * @param papel Papel da pessoa.
         */
        void setPapel(const Papel& papel);

        /**
         * @brief Retorna o papel.
         * @return Papel da pessoa.
         */
        Papel getPapel() const;
};

#endif // PESSOA_HPP_INCLUDED
