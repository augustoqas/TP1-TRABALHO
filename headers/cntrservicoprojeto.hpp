#ifndef CNTRSERVICOPROJETO_HPP_INCLUDED
#define CNTRSERVICOPROJETO_HPP_INCLUDED

#include <vector>

#include "containerpessoa.hpp"
#include "containerprojeto.hpp"
#include "containerquadro.hpp"
#include "interfaces.hpp"

/**
 * @brief Servico de projetos (servicos 6 a 10): aplica as regras de Projeto.
 *
 * Usa tres conteineres passados por setter: pessoas (para saber se o usuario esta
 * cadastrado e qual o papel dele), projetos e quadros (para recusar excluir projeto
 * com quadros). So guarda os ponteiros; o main mantem os conteineres vivos.
 * Qualquer usuario cadastrado pode ler e listar; criar exige papel GESTOR, e quem
 * cria vira o gestor do projeto. Atualizar e excluir: so o gestor do projeto.
 * Se faltar um conteiner necessario, o metodo retorna false: ler e listar precisam
 * dos de pessoas e projetos; excluir precisa tambem do de quadros.
 */
class CntrServicoProjeto : public IServicoProjeto {
    private:
        ContainerPessoa* containerPessoa = nullptr;
        ContainerProjeto* containerProjeto = nullptr;
        ContainerQuadro* containerQuadro = nullptr;

        // Diz se o usuario esta cadastrado e o conteiner de projetos existe.
        bool usuarioCadastrado(const Email& usuario) const;

        // Diz se o usuario esta cadastrado com papel GESTOR.
        bool usuarioGestor(const Email& usuario) const;

        // Diz se o usuario e o gestor (quem criou) do projeto; false se o projeto nao existir.
        bool gestorDoProjeto(const Email& usuario, const Identificador& projeto) const;

        // Confere identificador, nome, descricao, inicio e termino preenchidos.
        // Dominio nunca preenchido fica com valor vazio.
        static bool projetoCompleto(const Projeto& projeto);

    public:
        /** @brief Define o conteiner de pessoas (cadastro e papel do usuario). */
        void setContainerPessoa(ContainerPessoa* containerPessoa);

        /** @brief Define o conteiner de projetos. */
        void setContainerProjeto(ContainerProjeto* containerProjeto);

        /** @brief Define o conteiner de quadros (para recusar excluir projeto com quadros). */
        void setContainerQuadro(ContainerQuadro* containerQuadro);

        /**
         * @brief Cria o projeto; o usuario vira o gestor (servico 6).
         * Retorna false se o usuario nao for GESTOR cadastrado, se o projeto estiver
         * incompleto ou se o identificador ja existir.
         */
        bool criar(const Email& usuario, const Projeto& projeto) override;

        /**
         * @brief Le um projeto (servico 7).
         * Retorna false se o usuario nao estiver cadastrado ou o projeto nao existir.
         */
        bool ler(const Email& usuario, const Identificador& identificador, Projeto& projeto) override;

        /**
         * @brief Atualiza nome, descricao, inicio e termino (servico 8).
         * O projeto e achado pelo identificador, que nunca muda; o gestor tambem nao.
         * Retorna false se o usuario nao for o gestor do projeto, se o projeto estiver
         * incompleto ou nao existir.
         */
        bool atualizar(const Email& usuario, const Projeto& projeto) override;

        /**
         * @brief Exclui o projeto (servico 9), sem cascata.
         * Retorna false se o usuario nao for o gestor do projeto, se o projeto nao
         * existir ou se ele tiver quadros.
         */
        bool excluir(const Email& usuario, const Identificador& identificador) override;

        /**
         * @brief Lista os identificadores dos projetos (servico 10).
         * Retorna false se o usuario nao estiver cadastrado.
         */
        bool listar(const Email& usuario, std::vector<Identificador>& lista) override;
};

#endif // CNTRSERVICOPROJETO_HPP_INCLUDED
