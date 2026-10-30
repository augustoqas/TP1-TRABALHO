#ifndef CNTRSERVICOPESSOA_HPP_INCLUDED
#define CNTRSERVICOPESSOA_HPP_INCLUDED

#include <vector>

#include "containercartao.hpp"
#include "containerpessoa.hpp"
#include "containerprojeto.hpp"
#include "interfaces.hpp"

/**
 * @brief Controladora do servico de pessoas (servicos 1 a 5).
 *
 * Aplica as regras de negocio de Pessoa e usa tres conteineres recebidos por
 * setters (injecao de dependencia, no mesmo estilo de setCntrServicoPessoa da
 * camada de apresentacao):
 * - ContainerPessoa: armazenamento das pessoas;
 * - ContainerProjeto: consulta se a pessoa e gestora de algum projeto;
 * - ContainerCartao: consulta se a pessoa e desenvolvedora de algum cartao.
 *
 * A controladora NAO e dona dos conteineres: apenas guarda ponteiros; quem os
 * cria (o main) deve mante-los vivos enquanto a controladora for usada.
 *
 * Permissoes: qualquer usuario cadastrado (GESTOR ou DESENVOLVEDOR) pode ler,
 * atualizar, excluir (inclusive a si mesmo) e listar qualquer pessoa. criar
 * nao exige sessao.
 *
 * Dependencia nao informada (ponteiro nulo): o metodo que precisa dela retorna
 * false sem alterar nada (nunca lanca excecao). criar, ler e listar precisam
 * somente do ContainerPessoa; atualizar e excluir precisam dos tres, pois sem
 * os conteineres de projeto e de cartao nao ha como verificar os vinculos.
 */
class CntrServicoPessoa : public IServicoPessoa {
    private:
        ContainerPessoa* containerPessoa = nullptr;    ///< Conteiner de pessoas (nao e dono; nulo ate ser informado).
        ContainerProjeto* containerProjeto = nullptr;  ///< Conteiner de projetos (nao e dono; nulo ate ser informado).
        ContainerCartao* containerCartao = nullptr;    ///< Conteiner de cartoes (nao e dono; nulo ate ser informado).

        /**
         * @brief Verifica se o usuario da sessao esta cadastrado.
         * @param usuario Email do usuario autenticado.
         * @return true se o conteiner de pessoas foi informado e contem o
         *         usuario; false caso contrario.
         */
        bool usuarioCadastrado(const Email& usuario) const;

    public:
        /**
         * @brief Informa o conteiner de pessoas.
         * @param containerPessoa Ponteiro para o conteiner de pessoas.
         */
        void setContainerPessoa(ContainerPessoa* containerPessoa);

        /**
         * @brief Informa o conteiner de projetos (consulta de gestores).
         * @param containerProjeto Ponteiro para o conteiner de projetos.
         */
        void setContainerProjeto(ContainerProjeto* containerProjeto);

        /**
         * @brief Informa o conteiner de cartoes (consulta de desenvolvedores).
         * @param containerCartao Ponteiro para o conteiner de cartoes.
         */
        void setContainerCartao(ContainerCartao* containerCartao);

        /**
         * @brief Cria uma pessoa (servico 1). Nao exige sessao.
         * @param pessoa Pessoa completa (email, nome, senha e papel).
         * @return true se criou; false se o email ja esta cadastrado.
         */
        bool criar(const Pessoa& pessoa) override;

        /**
         * @brief Le os dados de uma pessoa (servico 2).
         * @param usuario Email do usuario autenticado.
         * @param email Email (chave primaria) da pessoa a ler.
         * @param pessoa Saida: dados da pessoa (preenchida somente se retornar
         *        true).
         * @return true se encontrou; false se o usuario nao esta cadastrado ou
         *         se a pessoa nao existe.
         */
        bool ler(const Email& usuario, const Email& email, Pessoa& pessoa) override;

        /**
         * @brief Atualiza nome, senha e papel de uma pessoa (servico 3). A
         *        pessoa e localizada pelo email contido em pessoa; o email nao e
         *        alterado.
         * @param usuario Email do usuario autenticado.
         * @param pessoa Pessoa com o email (chave) e os novos dados.
         * @return true se atualizou; false se o usuario nao esta cadastrado, se
         *         a pessoa nao existe, se ela e gestora de algum projeto e o
         *         novo papel nao e GESTOR, ou se ela e desenvolvedora de algum
         *         cartao e o novo papel nao e DESENVOLVEDOR.
         */
        bool atualizar(const Email& usuario, const Pessoa& pessoa) override;

        /**
         * @brief Exclui uma pessoa (servico 4). Nao faz exclusao em cascata.
         * @param usuario Email do usuario autenticado.
         * @param email Email (chave primaria) da pessoa a excluir.
         * @return true se excluiu; false se o usuario nao esta cadastrado, se a
         *         pessoa nao existe, ou se ela e gestora de algum projeto ou
         *         desenvolvedora de algum cartao.
         */
        bool excluir(const Email& usuario, const Email& email) override;

        /**
         * @brief Lista os emails de todas as pessoas (servico 5).
         * @param usuario Email do usuario autenticado.
         * @param lista Saida: o conteudo anterior e substituido pelos emails
         *        cadastrados (somente se retornar true).
         * @return true se listou; false se o usuario nao esta cadastrado.
         */
        bool listar(const Email& usuario, std::vector<Email>& lista) override;
};

#endif // CNTRSERVICOPESSOA_HPP_INCLUDED
