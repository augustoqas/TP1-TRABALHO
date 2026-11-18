#ifndef CNTRSERVICOQUADRO_HPP_INCLUDED
#define CNTRSERVICOQUADRO_HPP_INCLUDED

#include <vector>

#include "containercartao.hpp"
#include "containerpessoa.hpp"
#include "containerprojeto.hpp"
#include "containerquadro.hpp"
#include "interfaces.hpp"

/**
 * @brief Servico de quadros (servicos 11 a 15): aplica as regras de Quadro.
 *
 * Usa quatro conteineres passados por setter: pessoas (cadastro e papel do
 * usuario), projetos (o quadro so e criado em projeto existente), quadros e
 * cartoes (para o WIP e para recusar excluir quadro com cartoes). So guarda os
 * ponteiros; o main mantem os conteineres vivos.
 * Qualquer usuario cadastrado pode ler e listar; criar, atualizar e excluir exigem
 * papel GESTOR. Cada quadro pertence a exatamente um projeto, fixado ao criar.
 * Se faltar um conteiner necessario, o metodo retorna false.
 */
class CntrServicoQuadro : public IServicoQuadro {
    private:
        ContainerPessoa* containerPessoa = nullptr;
        ContainerProjeto* containerProjeto = nullptr;
        ContainerQuadro* containerQuadro = nullptr;
        ContainerCartao* containerCartao = nullptr;

        // Diz se o usuario esta cadastrado e o conteiner de quadros existe.
        bool usuarioCadastrado(const Email& usuario) const;

        // Diz se o usuario esta cadastrado com papel GESTOR.
        bool usuarioGestor(const Email& usuario) const;

        // Quantos cartoes do quadro estao em FAZENDO.
        int contarFazendo(const Identificador& quadro) const;

        // Confere identificador e nome preenchidos e limite definido.
        // Dominio nunca preenchido fica com valor vazio (ou 0, no Limite).
        static bool quadroCompleto(const Quadro& quadro);

    public:
        /** @brief Define o conteiner de pessoas (cadastro e papel do usuario). */
        void setContainerPessoa(ContainerPessoa* containerPessoa);

        /** @brief Define o conteiner de projetos (para conferir se o projeto existe). */
        void setContainerProjeto(ContainerProjeto* containerProjeto);

        /** @brief Define o conteiner de quadros. */
        void setContainerQuadro(ContainerQuadro* containerQuadro);

        /** @brief Define o conteiner de cartoes (WIP e exclusao sem orfaos). */
        void setContainerCartao(ContainerCartao* containerCartao);

        /**
         * @brief Cria o quadro dentro do projeto (servico 11).
         * Retorna false se o usuario nao for GESTOR cadastrado, se o projeto nao
         * existir, se o quadro estiver incompleto ou se o identificador ja existir.
         */
        bool criar(const Email& usuario, const Identificador& projeto, const Quadro& quadro) override;

        /**
         * @brief Le um quadro (servico 12).
         * Retorna false se o usuario nao estiver cadastrado ou o quadro nao existir.
         */
        bool ler(const Email& usuario, const Identificador& identificador, Quadro& quadro) override;

        /**
         * @brief Atualiza nome e limite (servico 13).
         * O quadro e achado pelo identificador, que nunca muda; o projeto tambem nao.
         * Retorna false se o usuario nao for GESTOR cadastrado, se o quadro estiver
         * incompleto ou nao existir, ou se o novo limite for menor que o WIP atual.
         */
        bool atualizar(const Email& usuario, const Quadro& quadro) override;

        /**
         * @brief Exclui o quadro (servico 14), sem cascata.
         * Retorna false se o usuario nao for GESTOR cadastrado, se o quadro nao
         * existir ou se ele tiver cartoes.
         */
        bool excluir(const Email& usuario, const Identificador& identificador) override;

        /**
         * @brief Lista os identificadores dos quadros do projeto (servico 15).
         * Retorna false se o usuario nao estiver cadastrado ou o projeto nao existir.
         */
        bool listar(const Email& usuario, const Identificador& projeto, std::vector<Identificador>& lista) override;
};

#endif // CNTRSERVICOQUADRO_HPP_INCLUDED
