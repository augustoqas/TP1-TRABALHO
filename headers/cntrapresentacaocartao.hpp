#ifndef CNTRAPRESENTACAOCARTAO_HPP_INCLUDED
#define CNTRAPRESENTACAOCARTAO_HPP_INCLUDED

#include <string>

#include "interfaces.hpp"
#include "tela.hpp"

/**
 * @brief Telas de cartoes de atividade (servicos 16 a 26): criar, ler, atualizar,
 *        excluir, listar e mover cartoes, e consultar as metricas do quadro.
 *
 * Valida a entrada pelos dominios (pede de novo ate o valor ser valido) e depende
 * so de IServicoCartao. Digitar 0 cancela.
 */
class CntrApresentacaoCartao : public IApresentacaoCartao {
    private:
        static const std::string DICA_IDENTIFICADOR;
        static const std::string DICA_EMAIL;
        static const std::string DICA_NOME;
        static const std::string DICA_TEXTO;
        static const std::string DICA_PRIORIDADE;
        static const std::string DICA_TAMANHO;

        IServicoCartao* cntrServicoCartao = nullptr;

        // Le nome, descricao, prioridade e tamanho; o identificador fica como esta.
        Tela::Leitura lerDadosEditaveis(CartaoAtividade& cartao);

        void mostrarCartao(const CartaoAtividade& cartao);

        // Mostra "N minutos (D d H h M min)".
        static std::string formatarMinutos(double minutos);

        void criar(const Email& usuario);
        void ler(const Email& usuario);
        void atualizar(const Email& usuario);
        void excluir(const Email& usuario);
        void listar(const Email& usuario);
        void mover(const Email& usuario);
        void lerTempoCiclo(const Email& usuario);
        void lerTempoCicloMedio(const Email& usuario);
        void lerLeadTime(const Email& usuario);
        void lerLeadTimeMedio(const Email& usuario);
        void lerWIP(const Email& usuario);

    public:
        /** @brief Roda o menu de cartoes ate o usuario voltar ou a entrada acabar. */
        void executar(const Email& usuario) override;

        /** @brief Define o servico de cartoes. */
        void setCntrServicoCartao(IServicoCartao* cntr) override;
};

inline void CntrApresentacaoCartao::setCntrServicoCartao(IServicoCartao* cntr) {
    this->cntrServicoCartao = cntr;
}

#endif // CNTRAPRESENTACAOCARTAO_HPP_INCLUDED
