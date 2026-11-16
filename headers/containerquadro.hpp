#ifndef CONTAINERQUADRO_HPP_INCLUDED
#define CONTAINERQUADRO_HPP_INCLUDED

#include <map>
#include <string>

#include "identificador.hpp"
#include "quadro.hpp"

/**
 * @brief Guarda os quadros em memoria e o identificador do projeto de cada um.
 *
 * Versao minima: so o que o modulo de Projeto usa. O modulo de Quadro completa
 * (pesquisar, atualizar, listar por projeto...) no mesmo padrao de ContainerProjeto.
 * O projeto fica aqui porque Quadro nao tem esse atributo. Nao confere regra de
 * negocio, e a copia e proibida pelo mesmo motivo de ContainerPessoa.
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

        /** @brief Remove o quadro e o seu projeto. Retorna false se ele nao existir. */
        bool remover(const Identificador& identificador);

        /** @brief Diz se o projeto tem algum quadro. */
        bool existeQuadroNoProjeto(const Identificador& projeto) const;
};

#endif // CONTAINERQUADRO_HPP_INCLUDED
