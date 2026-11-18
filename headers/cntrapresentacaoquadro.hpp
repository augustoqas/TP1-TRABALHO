#ifndef CNTRAPRESENTACAOQUADRO_HPP_INCLUDED
#define CNTRAPRESENTACAOQUADRO_HPP_INCLUDED

#include <string>

#include "interfaces.hpp"
#include "tela.hpp"

/**
 * @brief Telas de quadros (servicos 11 a 15): criar, ler, atualizar, excluir e
 *        listar os quadros de um projeto.
 *
 * Valida a entrada pelos dominios (pede de novo ate o valor ser valido) e depende
 * so de IServicoQuadro. Digitar 0 cancela.
 */
class CntrApresentacaoQuadro : public IApresentacaoQuadro {
    private:
        static const std::string DICA_IDENTIFICADOR_PROJETO;
        static const std::string DICA_IDENTIFICADOR_QUADRO;
        static const std::string DICA_NOME;
        static const std::string DICA_LIMITE;

        IServicoQuadro* cntrServicoQuadro = nullptr;

        // Le nome e limite; o identificador fica como esta.
        Tela::Leitura lerDadosEditaveis(Quadro& quadro);

        void mostrarQuadro(const Quadro& quadro);

        void criar(const Email& usuario);
        void ler(const Email& usuario);
        void atualizar(const Email& usuario);
        void excluir(const Email& usuario);
        void listar(const Email& usuario);

    public:
        /** @brief Roda o menu de quadros ate o usuario voltar ou a entrada acabar. */
        void executar(const Email& usuario) override;

        /** @brief Define o servico de quadros. */
        void setCntrServicoQuadro(IServicoQuadro* cntr) override;
};

inline void CntrApresentacaoQuadro::setCntrServicoQuadro(IServicoQuadro* cntr) {
    this->cntrServicoQuadro = cntr;
}

#endif // CNTRAPRESENTACAOQUADRO_HPP_INCLUDED
