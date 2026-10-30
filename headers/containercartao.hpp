#ifndef CONTAINERCARTAO_HPP_INCLUDED
#define CONTAINERCARTAO_HPP_INCLUDED

#include <map>
#include <string>

#include "cartaoatividade.hpp"
#include "email.hpp"
#include "identificador.hpp"

/**
 * @brief Guarda os cartoes em memoria e o email do desenvolvedor de cada um.
 *
 * Versao minima: so o que o modulo de Pessoa usa. Quem fizer o modulo de Cartao
 * completa (pesquisar, atualizar, listar por quadro...) no mesmo padrao de
 * ContainerPessoa. O desenvolvedor fica aqui porque CartaoAtividade nao tem esse
 * atributo. Nao confere regra de negocio, e a copia e proibida pelo mesmo motivo
 * de ContainerPessoa.
 */
class ContainerCartao {
    private:
        // Os dois mapas tem sempre as mesmas chaves: incluiu ou removeu um cartao,
        // faz o mesmo com o desenvolvedor.
        std::map<std::string, CartaoAtividade> cartoes;
        std::map<std::string, Email> desenvolvedores;

    public:
        /** @brief Cria o conteiner vazio. */
        ContainerCartao() = default;

        /** @brief Copia proibida (ver ContainerPessoa). */
        ContainerCartao(const ContainerCartao&) = delete;

        /** @brief Atribuicao proibida (ver ContainerPessoa). */
        ContainerCartao& operator=(const ContainerCartao&) = delete;

        /** @brief Guarda o cartao e o seu desenvolvedor. Retorna false se o identificador ja existir. */
        bool incluir(const CartaoAtividade& cartao, const Email& desenvolvedor);

        /** @brief Remove o cartao e o seu desenvolvedor. Retorna false se ele nao existir. */
        bool remover(const Identificador& identificador);

        /** @brief Diz se a pessoa e desenvolvedora de algum cartao. */
        bool existeDesenvolvedor(const Email& desenvolvedor) const;
};

#endif // CONTAINERCARTAO_HPP_INCLUDED
