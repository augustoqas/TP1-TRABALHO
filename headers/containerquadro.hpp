#ifndef CONTAINERQUADRO_HPP_INCLUDED
#define CONTAINERQUADRO_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>

#include "identificador.hpp"
#include "quadro.hpp"

/**
 * @brief Guarda os quadros em memoria e o identificador do projeto de cada um.
 *
 * O projeto fica aqui porque Quadro nao tem esse atributo; cada quadro pertence a
 * exatamente um projeto, fixado na inclusao. Nao confere regra de negocio, e a
 * copia e proibida pelo mesmo motivo de ContainerPessoa.
 */
class ContainerQuadro {
    private:
        // Os dois mapas tem sempre as mesmas chaves: incluiu ou removeu um quadro,
        // faz o mesmo com o projeto dele.
        std::map<std::string, Quadro> quadros;
        std::map<std::string, Identificador> projetos;

    public:
        /** @brief Cria o conteiner vazio. */
        ContainerQuadro() = default;

        /** @brief Copia proibida (ver ContainerPessoa). */
        ContainerQuadro(const ContainerQuadro&) = delete;

        /** @brief Atribuicao proibida (ver ContainerPessoa). */
        ContainerQuadro& operator=(const ContainerQuadro&) = delete;

        /** @brief Guarda o quadro e o seu projeto. Retorna false se o identificador ja existir. */
        bool incluir(const Quadro& quadro, const Identificador& projeto);

        /** @brief Busca o quadro pelo identificador. Retorna false se nao achar. */
        bool pesquisar(const Identificador& identificador, Quadro& quadro) const;

        /**
         * @brief Troca os dados do quadro com o mesmo identificador; o projeto nao muda.
         * Retorna false se ele nao existir.
         */
        bool atualizar(const Quadro& quadro);

        /** @brief Remove o quadro e o seu projeto. Retorna false se ele nao existir. */
        bool remover(const Identificador& identificador);

        /** @brief Substitui o conteudo de lista pelos identificadores dos quadros do projeto. */
        void listar(const Identificador& projeto, std::vector<Identificador>& lista) const;

        /** @brief Diz se o projeto tem algum quadro. */
        bool existeQuadroNoProjeto(const Identificador& projeto) const;
};

#endif // CONTAINERQUADRO_HPP_INCLUDED
