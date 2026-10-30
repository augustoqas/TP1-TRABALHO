#ifndef CNTRAPRESENTACAOAUTENTICACAO_HPP_INCLUDED
#define CNTRAPRESENTACAOAUTENTICACAO_HPP_INCLUDED

#include <string>

#include "interfaces.hpp"

/**
 * @brief Controladora de apresentacao da autenticacao (tela "Entrar").
 *
 * Pede email e senha (cada campo repete ate o formato ser valido), chama o
 * servico de autenticacao e mostra o resultado. Digitar "0" em um campo
 * cancela a operacao. A senha nunca e mostrada.
 *
 * NAO e dona do servico recebido por setCntrServicoAutenticacao.
 */
class CntrApresentacaoAutenticacao : public IApresentacaoAutenticacao {
    private:
        static const std::string DICA_EMAIL;  ///< Formato do email mostrado ao usuario.
        static const std::string DICA_SENHA;  ///< Formato da senha (sem exemplos).

        IServicoAutenticacao* cntrServicoAutenticacao = nullptr;  ///< Servico de autenticacao (nao e dono).

    public:
        /**
         * @brief Solicita email e senha e tenta autenticar.
         * @param email Saida: email do usuario autenticado (preenchido somente
         *        em caso de sucesso).
         * @return true se autenticou; false se a autenticacao falhou, se o
         *         usuario cancelou ou se a entrada terminou (EOF).
         */
        bool executar(Email& email) override;

        /**
         * @brief Informa a controladora de servico a ser usada.
         * @param cntr Ponteiro para o servico de autenticacao.
         */
        void setCntrServicoAutenticacao(IServicoAutenticacao* cntr) override;
};

inline void CntrApresentacaoAutenticacao::setCntrServicoAutenticacao(IServicoAutenticacao* cntr) {
    this->cntrServicoAutenticacao = cntr;
}

#endif // CNTRAPRESENTACAOAUTENTICACAO_HPP_INCLUDED
