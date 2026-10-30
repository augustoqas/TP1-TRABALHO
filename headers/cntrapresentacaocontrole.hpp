#ifndef CNTRAPRESENTACAOCONTROLE_HPP_INCLUDED
#define CNTRAPRESENTACAOCONTROLE_HPP_INCLUDED

#include "email.hpp"
#include "interfaces.hpp"

/**
 * @brief Controladora de apresentacao que conduz o programa: menu inicial
 *        (criar conta, entrar, sair) e menu principal do usuario autenticado
 *        (pessoas, projetos, quadros, cartoes, sair).
 *
 * Depende somente das interfaces de apresentacao, recebidas por setters
 * (injecao de dependencia). NAO e dona dos objetos recebidos (nao da delete).
 * Ponteiro nulo significa modulo ainda nao implementado: a opcao mostra
 * "Funcionalidade ainda nao disponivel".
 *
 * Fim da entrada (EOF) em qualquer ponto encerra o programa: depois de cada
 * chamada a outra controladora o estado de std::cin e verificado.
 */
class CntrApresentacaoControle {
    private:
        IApresentacaoAutenticacao* cntrApresentacaoAutenticacao = nullptr;  ///< Apresentacao de autenticacao (nao e dono).
        IApresentacaoPessoa* cntrApresentacaoPessoa = nullptr;              ///< Apresentacao de pessoas (nao e dono).
        IApresentacaoProjeto* cntrApresentacaoProjeto = nullptr;            ///< Apresentacao de projetos (nao e dono).
        IApresentacaoQuadro* cntrApresentacaoQuadro = nullptr;              ///< Apresentacao de quadros (nao e dono).
        IApresentacaoCartao* cntrApresentacaoCartao = nullptr;              ///< Apresentacao de cartoes (nao e dono).

        /**
         * @brief Mostra o menu principal ate o usuario sair, a sessao terminar
         *        (conta excluida) ou a entrada terminar.
         * @param usuario Email do usuario autenticado.
         */
        void executarMenuPrincipal(const Email& usuario);

        /**
         * @brief Executa a opcao Pessoas, tratando o fim de sessao.
         * @param usuario Email do usuario autenticado.
         * @return true se a sessao continua; false se terminou (conta excluida).
         */
        bool executarPessoas(const Email& usuario);

        /**
         * @brief Mostra a mensagem de modulo ainda nao implementado.
         */
        static void mostrarIndisponivel();

        /**
         * @brief Mostra a mensagem de encerramento por fim da entrada.
         */
        static void mostrarFimEntrada();

    public:
        /**
         * @brief Informa a apresentacao de autenticacao.
         * @param cntr Ponteiro para a apresentacao de autenticacao.
         */
        void setCntrApresentacaoAutenticacao(IApresentacaoAutenticacao* cntr);

        /**
         * @brief Informa a apresentacao de pessoas.
         * @param cntr Ponteiro para a apresentacao de pessoas.
         */
        void setCntrApresentacaoPessoa(IApresentacaoPessoa* cntr);

        /**
         * @brief Informa a apresentacao de projetos (pode ser nula).
         * @param cntr Ponteiro para a apresentacao de projetos.
         */
        void setCntrApresentacaoProjeto(IApresentacaoProjeto* cntr);

        /**
         * @brief Informa a apresentacao de quadros (pode ser nula).
         * @param cntr Ponteiro para a apresentacao de quadros.
         */
        void setCntrApresentacaoQuadro(IApresentacaoQuadro* cntr);

        /**
         * @brief Informa a apresentacao de cartoes (pode ser nula).
         * @param cntr Ponteiro para a apresentacao de cartoes.
         */
        void setCntrApresentacaoCartao(IApresentacaoCartao* cntr);

        /**
         * @brief Mostra o menu inicial ate o usuario escolher Sair ou a
         *        entrada terminar (EOF).
         */
        void executar();
};

inline void CntrApresentacaoControle::setCntrApresentacaoAutenticacao(IApresentacaoAutenticacao* cntr) {
    this->cntrApresentacaoAutenticacao = cntr;
}

inline void CntrApresentacaoControle::setCntrApresentacaoPessoa(IApresentacaoPessoa* cntr) {
    this->cntrApresentacaoPessoa = cntr;
}

inline void CntrApresentacaoControle::setCntrApresentacaoProjeto(IApresentacaoProjeto* cntr) {
    this->cntrApresentacaoProjeto = cntr;
}

inline void CntrApresentacaoControle::setCntrApresentacaoQuadro(IApresentacaoQuadro* cntr) {
    this->cntrApresentacaoQuadro = cntr;
}

inline void CntrApresentacaoControle::setCntrApresentacaoCartao(IApresentacaoCartao* cntr) {
    this->cntrApresentacaoCartao = cntr;
}

#endif // CNTRAPRESENTACAOCONTROLE_HPP_INCLUDED
