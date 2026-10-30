#ifndef CONTAINERPROJETO_HPP_INCLUDED
#define CONTAINERPROJETO_HPP_INCLUDED

#include <map>
#include <string>

#include "email.hpp"
#include "identificador.hpp"
#include "projeto.hpp"

/**
 * @brief Guarda os projetos em memoria e o email do gestor de cada um.
 *
 * Versao minima: so o que o modulo de Pessoa usa. Quem fizer o modulo de Projeto
 * completa (pesquisar, atualizar, listar...) no mesmo padrao de ContainerPessoa.
 * O gestor fica aqui porque Projeto nao tem esse atributo. Nao confere regra de
 * negocio, e a copia e proibida pelo mesmo motivo de ContainerPessoa.
 */
class ContainerProjeto {
    private:
        // Os dois mapas tem sempre as mesmas chaves: incluiu ou removeu um projeto,
        // faz o mesmo com o gestor.
        std::map<std::string, Projeto> projetos;
        std::map<std::string, Email> gestores;

    public:
        /** @brief Cria o conteiner vazio. */
        ContainerProjeto() = default;

        /** @brief Copia proibida (ver ContainerPessoa). */
        ContainerProjeto(const ContainerProjeto&) = delete;

        /** @brief Atribuicao proibida (ver ContainerPessoa). */
        ContainerProjeto& operator=(const ContainerProjeto&) = delete;

        /** @brief Guarda o projeto e o seu gestor. Retorna false se o identificador ja existir. */
        bool incluir(const Projeto& projeto, const Email& gestor);

        /** @brief Remove o projeto e o seu gestor. Retorna false se ele nao existir. */
        bool remover(const Identificador& identificador);

        /** @brief Diz se a pessoa e gestora de algum projeto. */
        bool existeGestor(const Email& gestor) const;
};

#endif // CONTAINERPROJETO_HPP_INCLUDED
