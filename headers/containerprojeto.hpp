#ifndef CONTAINERPROJETO_HPP_INCLUDED
#define CONTAINERPROJETO_HPP_INCLUDED

#include <map>
#include <string>

#include "email.hpp"
#include "identificador.hpp"
#include "projeto.hpp"

/**
 * @brief Conteiner (armazenamento em memoria) de projetos.
 *
 * VERSAO MINIMA: contem somente o que o modulo de Pessoa precisa (incluir,
 * remover e existeGestor). O responsavel pelo modulo de Projeto deve
 * completa-la (pesquisar, atualizar, listar etc.) seguindo o mesmo padrao de
 * ContainerPessoa.
 *
 * A entidade Projeto nao possui atributo gestor: a associacao
 * Pessoa 1--0..* Projeto ("gestor") e guardada aqui, como o email do gestor
 * de cada projeto. O conteiner nao depende de ContainerPessoa nem de servicos
 * e NAO verifica regras de negocio (ex.: se o gestor existe e e GESTOR).
 *
 * Copia proibida pelo mesmo motivo de ContainerPessoa (armazenamento unico,
 * compartilhado por ponteiro entre as controladoras).
 */
class ContainerProjeto {
    private:
        // ATENCAO: projetos e gestores andam juntos. Todo metodo que incluir ou
        // remover um projeto deve incluir ou remover tambem o seu gestor.
        std::map<std::string, Projeto> projetos;  ///< Projetos indexados pelo valor do identificador.
        std::map<std::string, Email> gestores;    ///< Email do gestor de cada projeto, indexado pelo valor do identificador (sempre com as mesmas chaves de projetos).

    public:
        /**
         * @brief Cria o conteiner vazio.
         */
        ContainerProjeto() = default;

        /**
         * @brief Copia proibida (o armazenamento deve ser unico).
         */
        ContainerProjeto(const ContainerProjeto&) = delete;

        /**
         * @brief Atribuicao proibida (o armazenamento deve ser unico).
         * @return Nao se aplica (metodo removido).
         */
        ContainerProjeto& operator=(const ContainerProjeto&) = delete;

        /**
         * @brief Inclui um projeto e registra o seu gestor.
         *        (Minimo exigido pelo modulo de Pessoa.)
         * @param projeto Projeto a incluir.
         * @param gestor Email da pessoa gestora do projeto.
         * @return true se incluiu; false se o identificador ja esta cadastrado.
         */
        bool incluir(const Projeto& projeto, const Email& gestor);

        /**
         * @brief Remove um projeto e o registro do seu gestor.
         *        (Minimo exigido pelo modulo de Pessoa.)
         * @param identificador Identificador (chave primaria) do projeto.
         * @return true se removeu; false se o identificador nao esta cadastrado.
         */
        bool remover(const Identificador& identificador);

        /**
         * @brief Informa se a pessoa e gestora de algum projeto.
         *        (Minimo exigido pelo modulo de Pessoa: usado para recusar a
         *        exclusao de um gestor e a troca do seu papel.)
         * @param gestor Email da pessoa.
         * @return true se a pessoa e gestora de pelo menos um projeto; false
         *         caso contrario.
         */
        bool existeGestor(const Email& gestor) const;
};

#endif // CONTAINERPROJETO_HPP_INCLUDED
