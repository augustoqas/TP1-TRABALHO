#ifndef INTERFACES_HPP_INCLUDED
#define INTERFACES_HPP_INCLUDED

/**
 * @file interfaces.hpp
 * @brief Interfaces entre a apresentacao e o servico.
 *
 * Regras de todas as interfaces de servico:
 * - Retorna true se deu certo e false se foi recusado; nao lanca excecao.
 * - Se retornar false, nada e alterado.
 * - O 1o parametro e o email de quem esta logado (exceto autenticar e criar pessoa).
 * - Associacao e passada pela chave da outra entidade (ex.: Identificador do projeto).
 * - A saida vem no ultimo parametro e so e preenchida se retornar true.
 * - Listar devolve as chaves (Email ou Identificador); vetor vazio = nao ha itens.
 * - Atualizar acha o registro pela chave e nunca muda a chave.
 * - Tempos sao double, em minutos.
 * - Excluir e recusado se deixar algo orfao (nao ha cascata).
 * - Destrutores sao virtuais puros, definidos vazios no fim do arquivo.
 */

#include <vector>

#include "email.hpp"
#include "senha.hpp"
#include "identificador.hpp"
#include "pessoa.hpp"
#include "projeto.hpp"
#include "quadro.hpp"
#include "cartaoatividade.hpp"

// --- Camada de servico ---

/** @brief Servico de autenticacao (login). */
class IServicoAutenticacao {
    public:
        /** @brief Confere email e senha. Retorna false se nao baterem. */
        virtual bool autenticar(const Email& email, const Senha& senha) = 0;

        /** @brief Destrutor virtual puro. */
        virtual ~IServicoAutenticacao() = 0;
};

/** @brief Servico de pessoas (servicos 1 a 5). Gestor e desenvolvedor podem usar tudo. */
class IServicoPessoa {
    public:
        /** @brief Cria a conta (servico 1), sem sessao. Retorna false se o email ja existir. */
        virtual bool criar(const Pessoa& pessoa) = 0;

        /** @brief Le uma pessoa (servico 2). Retorna false se ela nao existir. */
        virtual bool ler(const Email& usuario, const Email& email, Pessoa& pessoa) = 0;

        /**
         * @brief Atualiza nome, senha e papel (servico 3).
         * Retorna false se a pessoa nao existir, se for gestora de projeto e deixar de ser
         * GESTOR, ou se for dev de cartao e deixar de ser DESENVOLVEDOR.
         */
        virtual bool atualizar(const Email& usuario, const Pessoa& pessoa) = 0;

        /**
         * @brief Exclui a pessoa (servico 4).
         * Retorna false se ela nao existir ou for gestora de projeto ou dev de cartao.
         */
        virtual bool excluir(const Email& usuario, const Email& email) = 0;

        /** @brief Lista os emails (servico 5). Retorna false se o usuario nao estiver cadastrado. */
        virtual bool listar(const Email& usuario, std::vector<Email>& lista) = 0;

        /** @brief Destrutor virtual puro. */
        virtual ~IServicoPessoa() = 0;
};

/** @brief Servico de projetos (servicos 6 a 10). Criar, atualizar e excluir: so GESTOR. */
class IServicoProjeto {
    public:
        /**
         * @brief Cria o projeto; quem esta logado vira o gestor (servico 6).
         * Retorna false se o usuario nao for GESTOR ou o identificador ja existir.
         */
        virtual bool criar(const Email& usuario, const Projeto& projeto) = 0;

        /** @brief Le um projeto (servico 7). Retorna false se ele nao existir. */
        virtual bool ler(const Email& usuario, const Identificador& identificador, Projeto& projeto) = 0;

        /**
         * @brief Atualiza nome, descricao, inicio e termino (servico 8).
         * Retorna false se o projeto nao existir ou o usuario nao for GESTOR.
         */
        virtual bool atualizar(const Email& usuario, const Projeto& projeto) = 0;

        /**
         * @brief Exclui o projeto (servico 9).
         * Retorna false se ele nao existir, o usuario nao for GESTOR ou o projeto tiver quadros.
         */
        virtual bool excluir(const Email& usuario, const Identificador& identificador) = 0;

        /**
         * @brief Lista os identificadores dos projetos (servico 10).
         * Retorna false se o usuario nao estiver cadastrado.
         */
        virtual bool listar(const Email& usuario, std::vector<Identificador>& lista) = 0;

        /** @brief Destrutor virtual puro. */
        virtual ~IServicoProjeto() = 0;
};

/** @brief Servico de quadros (servicos 11 a 15). Criar, atualizar e excluir: so GESTOR. */
class IServicoQuadro {
    public:
        /**
         * @brief Cria um quadro dentro do projeto (servico 11).
         * Retorna false se o usuario nao for GESTOR, o projeto nao existir ou o
         * identificador do quadro ja existir.
         */
        virtual bool criar(const Email& usuario, const Identificador& projeto, const Quadro& quadro) = 0;

        /** @brief Le um quadro (servico 12). Retorna false se ele nao existir. */
        virtual bool ler(const Email& usuario, const Identificador& identificador, Quadro& quadro) = 0;

        /**
         * @brief Atualiza nome e limite (servico 13).
         * Retorna false se o quadro nao existir, o usuario nao for GESTOR ou o novo
         * limite for menor que o WIP atual.
         */
        virtual bool atualizar(const Email& usuario, const Quadro& quadro) = 0;

        /**
         * @brief Exclui o quadro (servico 14).
         * Retorna false se ele nao existir, o usuario nao for GESTOR ou o quadro tiver cartoes.
         */
        virtual bool excluir(const Email& usuario, const Identificador& identificador) = 0;

        /** @brief Lista os quadros do projeto (servico 15). Retorna false se ele nao existir. */
        virtual bool listar(const Email& usuario, const Identificador& projeto, std::vector<Identificador>& lista) = 0;

        /** @brief Destrutor virtual puro. */
        virtual ~IServicoQuadro() = 0;
};

/**
 * @brief Servico de cartoes (servicos 16 a 26). Criar, atualizar e excluir: so GESTOR.
 *
 * Tempo de ciclo = termino - inicio (FAZENDO ate FEITO).
 * Lead time = termino - entrada (A FAZER ate FEITO).
 */
class IServicoCartao {
    public:
        /**
         * @brief Cria o cartao no quadro, ja em A FAZER e com entrada = agora (servico 16).
         * Estado e timestamps vindos em cartao sao ignorados. Retorna false se o usuario
         * nao for GESTOR, o quadro nao existir, o desenvolvedor nao existir ou nao for
         * DESENVOLVEDOR, ou o identificador ja existir.
         */
        virtual bool criar(const Email& usuario, const Identificador& quadro, const Email& desenvolvedor, const CartaoAtividade& cartao) = 0;

        /** @brief Le um cartao (servico 17). Retorna false se ele nao existir. */
        virtual bool ler(const Email& usuario, const Identificador& identificador, CartaoAtividade& cartao) = 0;

        /**
         * @brief Atualiza nome, descricao, prioridade e tamanho (servico 18).
         * Estado e timestamps so mudam por mover. Retorna false se o cartao nao existir
         * ou o usuario nao for GESTOR.
         */
        virtual bool atualizar(const Email& usuario, const CartaoAtividade& cartao) = 0;

        /**
         * @brief Exclui o cartao (servico 19).
         * Retorna false se ele nao existir ou o usuario nao for GESTOR.
         */
        virtual bool excluir(const Email& usuario, const Identificador& identificador) = 0;

        /** @brief Lista os cartoes do quadro (servico 20). Retorna false se o quadro nao existir. */
        virtual bool listar(const Email& usuario, const Identificador& quadro, std::vector<Identificador>& lista) = 0;

        /**
         * @brief Passa o cartao para a proxima coluna (servico 21).
         * A FAZER -> FAZENDO grava o inicio; FAZENDO -> FEITO grava o termino.
         * Retorna false se o cartao nao existir, ja estiver em FEITO ou o limite do
         * quadro for estourar.
         */
        virtual bool mover(const Email& usuario, const Identificador& identificador) = 0;

        /**
         * @brief Tempo de ciclo do cartao (servico 22).
         * Retorna false se o cartao nao existir ou nao estiver em FEITO.
         */
        virtual bool lerTempoCiclo(const Email& usuario, const Identificador& identificador, double& minutos) = 0;

        /**
         * @brief Tempo de ciclo medio dos cartoes em FEITO do quadro (servico 23).
         * Retorna false se o quadro nao existir ou nao tiver cartao em FEITO.
         */
        virtual bool lerTempoCicloMedio(const Email& usuario, const Identificador& quadro, double& minutos) = 0;

        /**
         * @brief Lead time do cartao (servico 24).
         * Retorna false se o cartao nao existir ou nao estiver em FEITO.
         */
        virtual bool lerLeadTime(const Email& usuario, const Identificador& identificador, double& minutos) = 0;

        /**
         * @brief Lead time medio dos cartoes em FEITO do quadro (servico 25).
         * Retorna false se o quadro nao existir ou nao tiver cartao em FEITO.
         */
        virtual bool lerLeadTimeMedio(const Email& usuario, const Identificador& quadro, double& minutos) = 0;

        /**
         * @brief Quantos cartoes do quadro estao em FAZENDO (servico 26); pode ser 0.
         * Retorna false se o quadro nao existir.
         */
        virtual bool lerWIP(const Email& usuario, const Identificador& quadro, int& wip) = 0;

        /** @brief Destrutor virtual puro. */
        virtual ~IServicoCartao() = 0;
};

// --- Camada de apresentacao ---

/** @brief Tela de login. */
class IApresentacaoAutenticacao {
    public:
        /** @brief Pede email e senha. Se autenticar, retorna true e preenche email. */
        virtual bool executar(Email& email) = 0;

        /** @brief Define o servico de autenticacao usado pela tela. */
        virtual void setCntrServicoAutenticacao(IServicoAutenticacao* cntr) = 0;

        /** @brief Destrutor virtual puro. */
        virtual ~IApresentacaoAutenticacao() = 0;
};

/** @brief Telas de pessoas (servicos 1 a 5). */
class IApresentacaoPessoa {
    public:
        /**
         * @brief Mostra o menu de pessoas para quem esta logado.
         * @throw ExcecaoSessaoEncerrada Se o usuario excluir a propria conta. O menu
         *        principal captura e encerra a sessao.
         */
        virtual void executar(const Email& usuario) = 0;

        /** @brief Cria uma conta pelo menu inicial, antes do login. Retorna true se criou. */
        virtual bool cadastrar() = 0;

        /** @brief Define o servico de pessoas usado pela tela. */
        virtual void setCntrServicoPessoa(IServicoPessoa* cntr) = 0;

        /** @brief Destrutor virtual puro. */
        virtual ~IApresentacaoPessoa() = 0;
};

/** @brief Telas de projetos (servicos 6 a 10). */
class IApresentacaoProjeto {
    public:
        /** @brief Mostra o menu de projetos para quem esta logado. */
        virtual void executar(const Email& usuario) = 0;

        /** @brief Define o servico de projetos usado pela tela. */
        virtual void setCntrServicoProjeto(IServicoProjeto* cntr) = 0;

        /** @brief Destrutor virtual puro. */
        virtual ~IApresentacaoProjeto() = 0;
};

/** @brief Telas de quadros (servicos 11 a 15). */
class IApresentacaoQuadro {
    public:
        /** @brief Mostra o menu de quadros para quem esta logado. */
        virtual void executar(const Email& usuario) = 0;

        /** @brief Define o servico de quadros usado pela tela. */
        virtual void setCntrServicoQuadro(IServicoQuadro* cntr) = 0;

        /** @brief Destrutor virtual puro. */
        virtual ~IApresentacaoQuadro() = 0;
};

/** @brief Telas de cartoes (servicos 16 a 26). */
class IApresentacaoCartao {
    public:
        /** @brief Mostra o menu de cartoes para quem esta logado. */
        virtual void executar(const Email& usuario) = 0;

        /** @brief Define o servico de cartoes usado pela tela. */
        virtual void setCntrServicoCartao(IServicoCartao* cntr) = 0;

        /** @brief Destrutor virtual puro. */
        virtual ~IApresentacaoCartao() = 0;
};

// Mesmo sendo puros, os destrutores precisam de corpo: o da derivada chama o da base.
// inline evita definicao multipla quando o header entra em varios .cpp.

inline IServicoAutenticacao::~IServicoAutenticacao() {}
inline IServicoPessoa::~IServicoPessoa() {}
inline IServicoProjeto::~IServicoProjeto() {}
inline IServicoQuadro::~IServicoQuadro() {}
inline IServicoCartao::~IServicoCartao() {}
inline IApresentacaoAutenticacao::~IApresentacaoAutenticacao() {}
inline IApresentacaoPessoa::~IApresentacaoPessoa() {}
inline IApresentacaoProjeto::~IApresentacaoProjeto() {}
inline IApresentacaoQuadro::~IApresentacaoQuadro() {}
inline IApresentacaoCartao::~IApresentacaoCartao() {}

#endif // INTERFACES_HPP_INCLUDED
