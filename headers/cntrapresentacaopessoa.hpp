#ifndef CNTRAPRESENTACAOPESSOA_HPP_INCLUDED
#define CNTRAPRESENTACAOPESSOA_HPP_INCLUDED

#include <string>

#include "interfaces.hpp"
#include "tela.hpp"

/**
 * @brief Controladora de apresentacao de pessoas: criar conta (sem sessao) e
 *        menu de pessoas (criar, ler, atualizar, excluir e listar).
 *
 * Cada campo e validado montando o dominio (setValor) e repetido ate ficar
 * valido; digitar "0" em um campo cancela a operacao. A senha nunca e
 * mostrada. As mensagens de falha listam os motivos possiveis, pois o servico
 * so informa sucesso ou falha (bool).
 *
 * Quando o usuario exclui a propria conta, as mensagens sao mostradas e
 * executar lanca ExcecaoSessaoEncerrada, capturada pelo Controle.
 *
 * NAO e dona do servico recebido por setCntrServicoPessoa.
 */
class CntrApresentacaoPessoa : public IApresentacaoPessoa {
    private:
        static const std::string DICA_EMAIL;  ///< Formato do email.
        static const std::string DICA_NOME;   ///< Formato do nome.
        static const std::string DICA_SENHA;  ///< Formato da senha (sem exemplos).
        static const std::string DICA_PAPEL;  ///< Valores aceitos para o papel.

        IServicoPessoa* cntrServicoPessoa = nullptr;  ///< Servico de pessoas (nao e dono).

        /**
         * @brief Le o papel (texto GESTOR ou DESENVOLVEDOR), repetindo ate ser
         *        valido.
         * @param papel Saida: papel preenchido (somente se retornar LIDO).
         * @return LIDO, CANCELADO ou FIM.
         */
        Tela::Leitura lerPapel(Papel& papel);

        /**
         * @brief Le nome, senha e papel e os guarda na pessoa.
         * @param pessoa Pessoa a completar (o email nao e alterado).
         * @return LIDO, CANCELADO ou FIM.
         */
        Tela::Leitura lerDadosAlteraveis(Pessoa& pessoa);

        /**
         * @brief Le email, nome, senha e papel de uma nova pessoa.
         * @param pessoa Saida: pessoa completa (somente se retornar LIDO).
         * @return LIDO, CANCELADO ou FIM.
         */
        Tela::Leitura lerNovaPessoa(Pessoa& pessoa);

        /**
         * @brief Mostra email, nome e papel (nunca a senha).
         * @param pessoa Pessoa a mostrar.
         */
        void mostrarPessoa(const Pessoa& pessoa);

        /**
         * @brief Formulario de criacao usado por cadastrar e por criar.
         * @param titulo Titulo da tela.
         * @param mensagemSucesso Mensagem mostrada se o servico criar.
         * @param mensagemFalha Mensagem mostrada se o servico recusar.
         * @return true se a pessoa foi criada; false caso contrario.
         */
        bool formularioCriacao(const std::string& titulo, const std::string& mensagemSucesso,
                               const std::string& mensagemFalha);

        /**
         * @brief Opcao 2 do menu: le e mostra uma pessoa.
         * @param usuario Email do usuario autenticado.
         */
        void ler(const Email& usuario);

        /**
         * @brief Opcao 3 do menu: atualiza nome, senha e papel de uma pessoa.
         * @param usuario Email do usuario autenticado.
         */
        void atualizar(const Email& usuario);

        /**
         * @brief Opcao 4 do menu: exclui uma pessoa, apos confirmacao.
         * @param usuario Email do usuario autenticado.
         * @throw ExcecaoSessaoEncerrada Se o usuario excluiu a propria conta.
         */
        void excluir(const Email& usuario);

        /**
         * @brief Opcao 5 do menu: lista os emails das pessoas cadastradas.
         * @param usuario Email do usuario autenticado.
         */
        void listar(const Email& usuario);

    public:
        /**
         * @brief Exibe o menu de pessoas ate o usuario escolher Voltar, a
         *        entrada terminar (EOF) ou o usuario excluir a propria conta.
         * @param usuario Email do usuario autenticado.
         * @throw ExcecaoSessaoEncerrada Se o usuario excluiu a propria conta
         *        (a sessao terminou e o Controle deve voltar ao menu inicial).
         */
        void executar(const Email& usuario) override;

        /**
         * @brief Cria conta (servico 1) sem sessao, a partir do menu inicial.
         * @return true se a conta foi criada; false se o servico recusou, se o
         *         usuario cancelou ou se a entrada terminou (EOF).
         */
        bool cadastrar() override;

        /**
         * @brief Informa a controladora de servico a ser usada.
         * @param cntr Ponteiro para o servico de pessoas.
         */
        void setCntrServicoPessoa(IServicoPessoa* cntr) override;
};

inline void CntrApresentacaoPessoa::setCntrServicoPessoa(IServicoPessoa* cntr) {
    this->cntrServicoPessoa = cntr;
}

#endif // CNTRAPRESENTACAOPESSOA_HPP_INCLUDED
