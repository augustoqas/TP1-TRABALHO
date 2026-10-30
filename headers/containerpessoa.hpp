#ifndef CONTAINERPESSOA_HPP_INCLUDED
#define CONTAINERPESSOA_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>

#include "email.hpp"
#include "pessoa.hpp"

/**
 * @brief Conteiner (armazenamento em memoria) de pessoas.
 *
 * Guarda as pessoas em um std::map indexado pelo valor do email (chave
 * primaria). O conteiner apenas armazena: NAO verifica permissoes nem regras
 * de negocio (isso e papel das controladoras de servico). Os metodos retornam
 * bool para indicar registro encontrado ou duplicado.
 *
 * O objeto e criado uma unica vez (no main) e entregue as controladoras por
 * ponteiro (injecao de dependencia). Por isso a copia e proibida: uma copia
 * acidental criaria um segundo armazenamento e as controladoras deixariam de
 * enxergar os mesmos dados.
 */
class ContainerPessoa {
    private:
        std::map<std::string, Pessoa> pessoas;  ///< Pessoas indexadas pelo valor do email.

    public:
        /**
         * @brief Cria o conteiner vazio.
         */
        ContainerPessoa() = default;

        /**
         * @brief Copia proibida (o armazenamento deve ser unico).
         */
        ContainerPessoa(const ContainerPessoa&) = delete;

        /**
         * @brief Atribuicao proibida (o armazenamento deve ser unico).
         * @return Nao se aplica (metodo removido).
         */
        ContainerPessoa& operator=(const ContainerPessoa&) = delete;

        /**
         * @brief Inclui uma pessoa.
         * @param pessoa Pessoa a incluir.
         * @return true se incluiu; false se o email ja esta cadastrado.
         */
        bool incluir(const Pessoa& pessoa);

        /**
         * @brief Pesquisa uma pessoa pelo email.
         * @param email Email (chave primaria) da pessoa.
         * @param pessoa Saida: dados da pessoa (preenchida somente se encontrou).
         * @return true se encontrou; false caso contrario.
         */
        bool pesquisar(const Email& email, Pessoa& pessoa) const;

        /**
         * @brief Substitui os dados de uma pessoa ja cadastrada. A pessoa e
         *        localizada pelo email contido em pessoa.
         * @param pessoa Pessoa com o email (chave) e os novos dados.
         * @return true se atualizou; false se o email nao esta cadastrado.
         */
        bool atualizar(const Pessoa& pessoa);

        /**
         * @brief Remove uma pessoa.
         * @param email Email (chave primaria) da pessoa.
         * @return true se removeu; false se o email nao esta cadastrado.
         */
        bool remover(const Email& email);

        /**
         * @brief Lista os emails de todas as pessoas cadastradas.
         * @param lista Saida: o conteudo anterior e descartado e o vetor passa a
         *        conter os emails cadastrados (vazio se nao ha pessoas).
         */
        void listar(std::vector<Email>& lista) const;
};

#endif // CONTAINERPESSOA_HPP_INCLUDED
