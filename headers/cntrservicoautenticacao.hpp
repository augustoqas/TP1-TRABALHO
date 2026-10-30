#ifndef CNTRSERVICOAUTENTICACAO_HPP_INCLUDED
#define CNTRSERVICOAUTENTICACAO_HPP_INCLUDED

#include "containerpessoa.hpp"
#include "interfaces.hpp"

/**
 * @brief Controladora do servico de autenticacao.
 *
 * Consulta as pessoas no ContainerPessoa recebido por setContainerPessoa
 * (injecao de dependencia). A controladora NAO e dona do conteiner: apenas
 * guarda o ponteiro; quem cria o conteiner (o main) deve mante-lo vivo
 * enquanto a controladora for usada.
 *
 * Se o conteiner nao foi informado (ponteiro nulo), autenticar retorna false
 * (nao lanca excecao nem acessa memoria invalida).
 */
class CntrServicoAutenticacao : public IServicoAutenticacao {
    private:
        ContainerPessoa* containerPessoa = nullptr;  ///< Conteiner de pessoas (nao e dono; nulo ate ser informado).

    public:
        /**
         * @brief Informa o conteiner de pessoas a ser consultado.
         * @param containerPessoa Ponteiro para o conteiner de pessoas.
         */
        void setContainerPessoa(ContainerPessoa* containerPessoa);

        /**
         * @brief Verifica se existe pessoa com o email e a senha informados.
         * @param email Email informado pelo usuario.
         * @param senha Senha informada pelo usuario.
         * @return true se email e senha conferem; false caso contrario (ou se o
         *         conteiner de pessoas nao foi informado).
         */
        bool autenticar(const Email& email, const Senha& senha) override;
};

#endif // CNTRSERVICOAUTENTICACAO_HPP_INCLUDED
