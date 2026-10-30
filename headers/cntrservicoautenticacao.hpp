#ifndef CNTRSERVICOAUTENTICACAO_HPP_INCLUDED
#define CNTRSERVICOAUTENTICACAO_HPP_INCLUDED

#include "containerpessoa.hpp"
#include "interfaces.hpp"

/**
 * @brief Servico de autenticacao: confere email e senha no ContainerPessoa.
 *
 * So guarda o ponteiro do conteiner; quem o criou (o main) mantem ele vivo.
 */
class CntrServicoAutenticacao : public IServicoAutenticacao {
    private:
        ContainerPessoa* containerPessoa = nullptr;

    public:
        /** @brief Define o conteiner de pessoas consultado. */
        void setContainerPessoa(ContainerPessoa* containerPessoa);

        /** @brief Confere email e senha. Retorna false se nao baterem ou se faltar o conteiner. */
        bool autenticar(const Email& email, const Senha& senha) override;
};

#endif // CNTRSERVICOAUTENTICACAO_HPP_INCLUDED
