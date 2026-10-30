#ifndef CNTRSERVICOPESSOA_HPP_INCLUDED
#define CNTRSERVICOPESSOA_HPP_INCLUDED

#include <vector>

#include "containercartao.hpp"
#include "containerpessoa.hpp"
#include "containerprojeto.hpp"
#include "interfaces.hpp"

/**
 * @brief Servico de pessoas (servicos 1 a 5): aplica as regras de Pessoa.
 *
 * Usa tres conteineres passados por setter: pessoas, projetos (para saber quem e
 * gestor) e cartoes (para saber quem e desenvolvedor). So guarda os ponteiros; o main mantem
 * os conteineres vivos. Qualquer usuario cadastrado pode ler, atualizar, excluir
 * (ate a si mesmo) e listar qualquer pessoa; criar nao exige sessao.
 * Se faltar um conteiner, o metodo retorna false: criar, ler e listar precisam so
 * do de pessoas; atualizar e excluir precisam dos tres para checar os vinculos.
 */
class CntrServicoPessoa : public IServicoPessoa {
    private:
        ContainerPessoa* containerPessoa = nullptr;
        ContainerProjeto* containerProjeto = nullptr;
        ContainerCartao* containerCartao = nullptr;

        bool usuarioCadastrado(const Email& usuario) const;

        // Confere email, nome e senha preenchidos e papel valido. Dominio nunca
        // preenchido fica com valor vazio.
        static bool pessoaCompleta(const Pessoa& pessoa);

    public:
        /** @brief Define o conteiner de pessoas. */
        void setContainerPessoa(ContainerPessoa* containerPessoa);

        /** @brief Define o conteiner de projetos (para saber quem e gestor). */
        void setContainerProjeto(ContainerProjeto* containerProjeto);

        /** @brief Define o conteiner de cartoes (para saber quem e desenvolvedor). */
        void setContainerCartao(ContainerCartao* containerCartao);

        /**
         * @brief Cria a conta (servico 1); nao exige sessao.
         * Retorna false se o email ja existir ou se faltar email, nome, senha ou papel valido.
         */
        bool criar(const Pessoa& pessoa) override;

        /**
         * @brief Le uma pessoa (servico 2).
         * Retorna false se o usuario nao estiver cadastrado ou a pessoa nao existir.
         */
        bool ler(const Email& usuario, const Email& email, Pessoa& pessoa) override;

        /**
         * @brief Atualiza nome, senha e papel (servico 3).
         * Retorna false se o usuario nao estiver cadastrado, se a pessoa nao existir ou
         * estiver incompleta, ou se ela for gestora de projeto ou desenvolvedora de cartao e o novo
         * papel nao bater.
         */
        bool atualizar(const Email& usuario, const Pessoa& pessoa) override;

        /**
         * @brief Exclui a pessoa (servico 4), sem cascata.
         * Retorna false se o usuario nao estiver cadastrado, se a pessoa nao existir ou
         * se ela for gestora de projeto ou desenvolvedora de cartao.
         */
        bool excluir(const Email& usuario, const Email& email) override;

        /**
         * @brief Lista os emails (servico 5).
         * Retorna false se o usuario nao estiver cadastrado.
         */
        bool listar(const Email& usuario, std::vector<Email>& lista) override;
};

#endif // CNTRSERVICOPESSOA_HPP_INCLUDED
