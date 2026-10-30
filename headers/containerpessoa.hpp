#ifndef CONTAINERPESSOA_HPP_INCLUDED
#define CONTAINERPESSOA_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>

#include "email.hpp"
#include "pessoa.hpp"

/**
 * @brief Guarda as pessoas em memoria, indexadas pelo email.
 *
 * So armazena; quem confere permissao e regra de negocio e o servico.
 * A copia e proibida porque o main cria um unico conteiner e passa o ponteiro para
 * as controladoras; uma copia criaria um segundo armazenamento e os dados divergiriam.
 */
class ContainerPessoa {
    private:
        std::map<std::string, Pessoa> pessoas;

    public:
        /** @brief Cria o conteiner vazio. */
        ContainerPessoa() = default;

        /** @brief Copia proibida (ver a classe). */
        ContainerPessoa(const ContainerPessoa&) = delete;

        /** @brief Atribuicao proibida (ver a classe). */
        ContainerPessoa& operator=(const ContainerPessoa&) = delete;

        /** @brief Guarda a pessoa. Retorna false se o email ja existir. */
        bool incluir(const Pessoa& pessoa);

        /** @brief Busca a pessoa pelo email. Retorna false se nao achar. */
        bool pesquisar(const Email& email, Pessoa& pessoa) const;

        /** @brief Troca os dados da pessoa com o mesmo email. Retorna false se ela nao existir. */
        bool atualizar(const Pessoa& pessoa);

        /** @brief Remove a pessoa. Retorna false se ela nao existir. */
        bool remover(const Email& email);

        /** @brief Substitui o conteudo de lista pelos emails cadastrados. */
        void listar(std::vector<Email>& lista) const;
};

#endif // CONTAINERPESSOA_HPP_INCLUDED
