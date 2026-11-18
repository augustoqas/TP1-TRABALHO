#ifndef CNTRAPRESENTACAOPROJETO_HPP_INCLUDED
#define CNTRAPRESENTACAOPROJETO_HPP_INCLUDED

#include <string>

#include "interfaces.hpp"
#include "tela.hpp"

/**
 * @brief Telas de projetos (servicos 6 a 10): criar, ler, atualizar, excluir e
 *        listar projetos.
 *
 * Valida a entrada pelos dominios (pede de novo ate o valor ser valido) e depende
 * so de IServicoProjeto. Digitar 0 cancela.
 */
class CntrApresentacaoProjeto : public IApresentacaoProjeto {
    private:
        static const std::string DICA_IDENTIFICADOR;
        static const std::string DICA_NOME;
        static const std::string DICA_TEXTO;
        static const std::string DICA_TIMESTAMP;

        IServicoProjeto* cntrServicoProjeto = nullptr;

        // Le nome, descricao, inicio e termino; o identificador fica como esta.
        Tela::Leitura lerDadosEditaveis(Projeto& projeto);

        void mostrarProjeto(const Projeto& projeto);

        void criar(const Email& usuario);
        void ler(const Email& usuario);
        void atualizar(const Email& usuario);
        void excluir(const Email& usuario);
        void listar(const Email& usuario);

    public:
        /** @brief Roda o menu de projetos ate o usuario voltar ou a entrada acabar. */
        void executar(const Email& usuario) override;

        /** @brief Define o servico de projetos. */
        void setCntrServicoProjeto(IServicoProjeto* cntr) override;
};

inline void CntrApresentacaoProjeto::setCntrServicoProjeto(IServicoProjeto* cntr) {
    this->cntrServicoProjeto = cntr;
}

#endif // CNTRAPRESENTACAOPROJETO_HPP_INCLUDED
