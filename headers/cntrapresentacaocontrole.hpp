#ifndef CNTRAPRESENTACAOCONTROLE_HPP_INCLUDED
#define CNTRAPRESENTACAOCONTROLE_HPP_INCLUDED

#include "email.hpp"
#include "interfaces.hpp"

/**
 * @brief Menus do programa: o inicial (criar conta, entrar, sair) e o
 *        principal, de quem esta logado.
 *
 * Recebe as outras telas pelos setters e nao e dona delas. Tela nula quer dizer
 * modulo ainda nao feito: a opcao mostra "Funcionalidade ainda nao disponivel".
 * Se a entrada acabar em qualquer ponto, o programa encerra.
 */
class CntrApresentacaoControle {
    private:
        IApresentacaoAutenticacao* cntrApresentacaoAutenticacao = nullptr;
        IApresentacaoPessoa* cntrApresentacaoPessoa = nullptr;
        IApresentacaoProjeto* cntrApresentacaoProjeto = nullptr;
        IApresentacaoQuadro* cntrApresentacaoQuadro = nullptr;
        IApresentacaoCartao* cntrApresentacaoCartao = nullptr;

        void executarMenuPrincipal(const Email& usuario);

        // Retorna false se a sessao acabou (a pessoa excluiu a propria conta).
        bool executarPessoas(const Email& usuario);

        static void mostrarIndisponivel();

        static void mostrarFimEntrada();

    public:
        /** @brief Define a tela de autenticacao (Entrar). */
        void setCntrApresentacaoAutenticacao(IApresentacaoAutenticacao* cntr);

        /** @brief Define a tela de pessoas. */
        void setCntrApresentacaoPessoa(IApresentacaoPessoa* cntr);

        /** @brief Define a tela de projetos (pode ser nula). */
        void setCntrApresentacaoProjeto(IApresentacaoProjeto* cntr);

        /** @brief Define a tela de quadros (pode ser nula). */
        void setCntrApresentacaoQuadro(IApresentacaoQuadro* cntr);

        /** @brief Define a tela de cartoes (pode ser nula). */
        void setCntrApresentacaoCartao(IApresentacaoCartao* cntr);

        /** @brief Roda o menu inicial ate o usuario escolher Sair ou a entrada acabar. */
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
