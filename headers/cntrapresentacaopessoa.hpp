#ifndef CNTRAPRESENTACAOPESSOA_HPP_INCLUDED
#define CNTRAPRESENTACAOPESSOA_HPP_INCLUDED

#include <string>

#include "interfaces.hpp"
#include "tela.hpp"

/**
 * @brief Telas de pessoas: criar conta e o menu de pessoas.
 *
 * A senha nunca aparece na tela.
 */
class CntrApresentacaoPessoa : public IApresentacaoPessoa {
    private:
        static const std::string DICA_EMAIL;
        static const std::string DICA_NOME;
        static const std::string DICA_SENHA;
        static const std::string DICA_PAPEL;

        IServicoPessoa* cntrServicoPessoa = nullptr;

        Tela::Leitura lerPapel(Papel& papel);

        // Le nome, senha e papel; o email da pessoa fica como esta.
        Tela::Leitura lerDadosAlteraveis(Pessoa& pessoa);

        Tela::Leitura lerNovaPessoa(Pessoa& pessoa);

        void mostrarPessoa(const Pessoa& pessoa);

        // Usado por cadastrar (criar conta) e pela opcao Criar do menu.
        bool formularioCriacao(const std::string& titulo, const std::string& mensagemSucesso,
                               const std::string& mensagemFalha);

        void ler(const Email& usuario);

        void atualizar(const Email& usuario);

        // Lanca ExcecaoSessaoEncerrada se a pessoa excluir a propria conta.
        void excluir(const Email& usuario);

        void listar(const Email& usuario);

    public:
        /**
         * @brief Roda o menu de pessoas ate o usuario voltar ou a entrada acabar.
         * @throw ExcecaoSessaoEncerrada Se a pessoa excluir a propria conta; o
         *        Controle captura e volta ao menu inicial.
         */
        void executar(const Email& usuario) override;

        /**
         * @brief Tela "Criar conta" do menu inicial (sem login).
         * @return true se a conta foi criada.
         */
        bool cadastrar() override;

        /** @brief Define o servico de pessoas. */
        void setCntrServicoPessoa(IServicoPessoa* cntr) override;
};

inline void CntrApresentacaoPessoa::setCntrServicoPessoa(IServicoPessoa* cntr) {
    this->cntrServicoPessoa = cntr;
}

#endif // CNTRAPRESENTACAOPESSOA_HPP_INCLUDED
