#ifndef CONTAINERCARTAO_HPP_INCLUDED
#define CONTAINERCARTAO_HPP_INCLUDED

#include <map>
#include <string>

#include "cartaoatividade.hpp"
#include "email.hpp"
#include "identificador.hpp"

/**
 * @brief Conteiner (armazenamento em memoria) de cartoes de atividade.
 *
 * VERSAO MINIMA: contem somente o que o modulo de Pessoa precisa (incluir,
 * remover e existeDesenvolvedor). O responsavel pelo modulo de Cartao deve
 * completa-la (pesquisar, atualizar, listar por quadro etc.) seguindo o mesmo
 * padrao de ContainerPessoa.
 *
 * A entidade CartaoAtividade nao possui atributo desenvolvedor: a associacao
 * Pessoa 1--0..* CartaoAtividade ("desenvolvedor") e guardada aqui, como o
 * email do desenvolvedor de cada cartao. O conteiner nao depende de
 * ContainerPessoa nem de servicos e NAO verifica regras de negocio (ex.: se o
 * desenvolvedor existe e e DESENVOLVEDOR).
 *
 * Copia proibida pelo mesmo motivo de ContainerPessoa (armazenamento unico,
 * compartilhado por ponteiro entre as controladoras).
 */
class ContainerCartao {
    private:
        // ATENCAO: cartoes e desenvolvedores andam juntos. Todo metodo que incluir
        // ou remover um cartao deve incluir ou remover tambem o seu desenvolvedor.
        std::map<std::string, CartaoAtividade> cartoes;  ///< Cartoes indexados pelo valor do identificador.
        std::map<std::string, Email> desenvolvedores;    ///< Email do desenvolvedor de cada cartao, indexado pelo valor do identificador (sempre com as mesmas chaves de cartoes).

    public:
        /**
         * @brief Cria o conteiner vazio.
         */
        ContainerCartao() = default;

        /**
         * @brief Copia proibida (o armazenamento deve ser unico).
         */
        ContainerCartao(const ContainerCartao&) = delete;

        /**
         * @brief Atribuicao proibida (o armazenamento deve ser unico).
         * @return Nao se aplica (metodo removido).
         */
        ContainerCartao& operator=(const ContainerCartao&) = delete;

        /**
         * @brief Inclui um cartao e registra o seu desenvolvedor.
         *        (Minimo exigido pelo modulo de Pessoa.)
         * @param cartao Cartao a incluir.
         * @param desenvolvedor Email da pessoa responsavel pelo cartao.
         * @return true se incluiu; false se o identificador ja esta cadastrado.
         */
        bool incluir(const CartaoAtividade& cartao, const Email& desenvolvedor);

        /**
         * @brief Remove um cartao e o registro do seu desenvolvedor.
         *        (Minimo exigido pelo modulo de Pessoa.)
         * @param identificador Identificador (chave primaria) do cartao.
         * @return true se removeu; false se o identificador nao esta cadastrado.
         */
        bool remover(const Identificador& identificador);

        /**
         * @brief Informa se a pessoa e desenvolvedora de algum cartao.
         *        (Minimo exigido pelo modulo de Pessoa: usado para recusar a
         *        exclusao de um desenvolvedor e a troca do seu papel.)
         * @param desenvolvedor Email da pessoa.
         * @return true se a pessoa e desenvolvedora de pelo menos um cartao;
         *         false caso contrario.
         */
        bool existeDesenvolvedor(const Email& desenvolvedor) const;
};

#endif // CONTAINERCARTAO_HPP_INCLUDED
