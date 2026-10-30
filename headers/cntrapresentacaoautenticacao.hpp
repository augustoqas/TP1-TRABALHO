#ifndef CNTRAPRESENTACAOAUTENTICACAO_HPP_INCLUDED
#define CNTRAPRESENTACAOAUTENTICACAO_HPP_INCLUDED

#include <string>

#include "interfaces.hpp"

/**
 * @brief Tela "Entrar": pede email e senha e autentica.
 *
 * Digitar 0 cancela. A senha nunca aparece na tela.
 */
class CntrApresentacaoAutenticacao : public IApresentacaoAutenticacao {
    private:
        static const std::string DICA_EMAIL;
        static const std::string DICA_SENHA;

        IServicoAutenticacao* cntrServicoAutenticacao = nullptr;

    public:
        /**
         * @brief Pede email e senha e tenta autenticar.
         * @param email Recebe o email de quem entrou (so se der certo).
         * @return false se email ou senha estiverem errados, se o usuario
         *         cancelar ou se a entrada acabar.
         */
        bool executar(Email& email) override;

        /** @brief Define o servico de autenticacao. */
        void setCntrServicoAutenticacao(IServicoAutenticacao* cntr) override;
};

inline void CntrApresentacaoAutenticacao::setCntrServicoAutenticacao(IServicoAutenticacao* cntr) {
    this->cntrServicoAutenticacao = cntr;
}

#endif // CNTRAPRESENTACAOAUTENTICACAO_HPP_INCLUDED
