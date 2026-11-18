#ifndef CONTAINERCARTAO_HPP_INCLUDED
#define CONTAINERCARTAO_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>

#include "cartaoatividade.hpp"
#include "email.hpp"
#include "identificador.hpp"

/**
 * @brief Guarda os cartoes em memoria, indexados pelo identificador, junto com
 *        o quadro e o email do desenvolvedor de cada um.
 *
 * O quadro e o desenvolvedor ficam aqui porque CartaoAtividade nao tem esses
 * atributos (sao associacoes do diagrama de classes). So armazena; quem confere
 * permissao e regra de negocio e o servico. A copia e proibida pelo mesmo motivo
 * de ContainerPessoa.
 */
class ContainerCartao {
    private:
        // Os tres mapas tem sempre as mesmas chaves: incluiu ou removeu um cartao,
        // faz o mesmo com o quadro e o desenvolvedor.
        std::map<std::string, CartaoAtividade> cartoes;
        std::map<std::string, Identificador> quadros;
        std::map<std::string, Email> desenvolvedores;

    public:
        /** @brief Cria o conteiner vazio. */
        ContainerCartao() = default;

        /** @brief Copia proibida (ver ContainerPessoa). */
        ContainerCartao(const ContainerCartao&) = delete;

        /** @brief Atribuicao proibida (ver ContainerPessoa). */
        ContainerCartao& operator=(const ContainerCartao&) = delete;

        /**
         * @brief Guarda o cartao, o seu quadro e o seu desenvolvedor.
         * @return false se o identificador ja existir (nada e alterado).
         */
        bool incluir(const CartaoAtividade& cartao, const Identificador& quadro, const Email& desenvolvedor);

        /** @brief Remove o cartao, o seu quadro e o seu desenvolvedor. Retorna false se ele nao existir. */
        bool remover(const Identificador& identificador);

        /** @brief Busca o cartao pelo identificador. Retorna false se nao achar. */
        bool pesquisar(const Identificador& identificador, CartaoAtividade& cartao) const;

        /** @brief Troca os dados do cartao com o mesmo identificador. Retorna false se ele nao existir. */
        bool atualizar(const CartaoAtividade& cartao);

        /** @brief Informa o quadro do cartao. Retorna false se o cartao nao existir. */
        bool getQuadro(const Identificador& identificador, Identificador& quadro) const;

        /** @brief Informa o desenvolvedor do cartao. Retorna false se o cartao nao existir. */
        bool getDesenvolvedor(const Identificador& identificador, Email& desenvolvedor) const;

        /** @brief Diz se a pessoa e desenvolvedora de algum cartao. */
        bool existeDesenvolvedor(const Email& desenvolvedor) const;

        /** @brief Diz se o quadro tem algum cartao (usado para recusar a exclusao do quadro). */
        bool existeQuadro(const Identificador& quadro) const;

        /** @brief Substitui o conteudo de lista pelos cartoes do quadro, em ordem de identificador. */
        void listar(const Identificador& quadro, std::vector<CartaoAtividade>& lista) const;
};

#endif // CONTAINERCARTAO_HPP_INCLUDED
