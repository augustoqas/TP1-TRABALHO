#ifndef INTERFACES_HPP_INCLUDED
#define INTERFACES_HPP_INCLUDED

/**
 * @file interfaces.hpp
 * @brief Interfaces entre a camada de apresentacao e a camada de servico.
 *
 * Convencoes adotadas em TODAS as interfaces de servico:
 *
 * 1. Retorno: todo metodo de servico retorna bool.
 *    true  = operacao realizada com sucesso.
 *    false = operacao recusada (registro inexistente, registro duplicado,
 *            permissao negada, regra de negocio violada ou exclusao que
 *            causaria inconsistencia). Se retornar false, nada e alterado.
 *    O servico NAO lanca excecoes. std::invalid_argument so aparece na
 *    apresentacao, ao montar dominios com setValor.
 *
 * 2. Usuario: todo metodo que exige sessao recebe, como PRIMEIRO parametro,
 *    o Email do usuario autenticado (parametro usuario). O servico usa esse
 *    email para consultar o papel (GESTOR ou DESENVOLVEDOR) e para definir o
 *    gestor de um projeto. Excecoes: IServicoAutenticacao::autenticar e
 *    IServicoPessoa::criar, que funcionam sem sessao (criacao de conta).
 *
 * 3. Entrada: parametros de entrada sao referencias constantes (const &).
 *    Associacoes sao informadas pela chave primaria da outra entidade
 *    (ex.: criar quadro recebe o Identificador do projeto).
 *
 * 4. Saida: parametros de saida sao referencias NAO constantes (&), sempre os
 *    ultimos da lista, e so sao preenchidos quando o metodo retorna true.
 *
 * 5. Listagens: preenchem um std::vector com as chaves primarias (Email ou
 *    Identificador). Retorno true com vetor vazio significa "nao ha itens".
 *
 * 6. Chave primaria: atualizar localiza o registro pela chave primaria contida
 *    na entidade recebida e altera somente os demais atributos.
 *
 * 7. Metricas de tempo: double, em MINUTOS. Retornam false se o cartao nao
 *    esta em FEITO ou se o quadro nao possui cartao em FEITO.
 *
 * 8. Destrutor: cada interface declara destrutor virtual PURO, definido vazio
 *    (inline) no final deste arquivo. Assim a interface contem somente metodos
 *    virtuais puros e o delete via ponteiro de interface e seguro.
 */

#include <vector>

#include "email.hpp"
#include "senha.hpp"
#include "identificador.hpp"
#include "pessoa.hpp"
#include "projeto.hpp"
#include "quadro.hpp"
#include "cartaoatividade.hpp"

// ---------------------------------------------------------------------------
// Interfaces da camada de servico
// ---------------------------------------------------------------------------

/**
 * @brief Interface do servico de autenticacao.
 */
class IServicoAutenticacao {
    public:
        /**
         * @brief Verifica se existe pessoa com o email e a senha informados.
         * @param email Email informado pelo usuario.
         * @param senha Senha informada pelo usuario.
         * @return true se email e senha conferem; false caso contrario.
         */
        virtual bool autenticar(const Email& email, const Senha& senha) = 0;

        /**
         * @brief Destrutor virtual puro.
         */
        virtual ~IServicoAutenticacao() = 0;
};

/**
 * @brief Interface do servico de pessoas (servicos 1 a 5).
 *
 * Todos os metodos sao permitidos para GESTOR e DESENVOLVEDOR.
 */
class IServicoPessoa {
    public:
        /**
         * @brief Cria uma pessoa (servico 1). Nao exige sessao: e o metodo usado
         *        para criar conta antes de autenticar.
         * @param pessoa Pessoa completa (email, nome, senha e papel).
         * @return true se criou; false se o email ja esta cadastrado.
         */
        virtual bool criar(const Pessoa& pessoa) = 0;

        /**
         * @brief Le os dados de uma pessoa (servico 2).
         * @param usuario Email do usuario autenticado.
         * @param email Email (chave primaria) da pessoa a ler.
         * @param pessoa Saida: dados da pessoa.
         * @return true se encontrou; false caso contrario.
         */
        virtual bool ler(const Email& usuario, const Email& email, Pessoa& pessoa) = 0;

        /**
         * @brief Atualiza nome, senha e papel de uma pessoa (servico 3).
         *        A pessoa e localizada pelo email contido em pessoa; o email
         *        nao e alterado.
         * @param usuario Email do usuario autenticado.
         * @param pessoa Pessoa com o email (chave) e os novos dados.
         * @return true se atualizou; false se nao encontrou ou se a troca de
         *         papel geraria inconsistencia (gestor de projeto deixando de ser
         *         GESTOR ou desenvolvedor de cartao deixando de ser DESENVOLVEDOR).
         */
        virtual bool atualizar(const Email& usuario, const Pessoa& pessoa) = 0;

        /**
         * @brief Exclui uma pessoa (servico 4). Recusa se a pessoa for gestor de
         *        algum projeto ou desenvolvedor de algum cartao.
         * @param usuario Email do usuario autenticado.
         * @param email Email (chave primaria) da pessoa a excluir.
         * @return true se excluiu; false se nao encontrou ou se a exclusao
         *         geraria inconsistencia.
         */
        virtual bool excluir(const Email& usuario, const Email& email) = 0;

        /**
         * @brief Lista os emails de todas as pessoas (servico 5).
         * @param usuario Email do usuario autenticado.
         * @param lista Saida: emails cadastrados.
         * @return true se listou; false se o usuario nao esta cadastrado.
         */
        virtual bool listar(const Email& usuario, std::vector<Email>& lista) = 0;

        /**
         * @brief Destrutor virtual puro.
         */
        virtual ~IServicoPessoa() = 0;
};

/**
 * @brief Interface do servico de projetos (servicos 6 a 10).
 *
 * criar, atualizar e excluir: somente GESTOR. ler e listar: ambos os papeis.
 */
class IServicoProjeto {
    public:
        /**
         * @brief Cria um projeto (servico 6). O gestor do projeto passa a ser o
         *        usuario autenticado.
         * @param usuario Email do usuario autenticado (sera o gestor).
         * @param projeto Projeto completo.
         * @return true se criou; false se o usuario nao e GESTOR ou se o
         *         identificador ja existe.
         */
        virtual bool criar(const Email& usuario, const Projeto& projeto) = 0;

        /**
         * @brief Le os dados de um projeto (servico 7).
         * @param usuario Email do usuario autenticado.
         * @param identificador Identificador (chave primaria) do projeto.
         * @param projeto Saida: dados do projeto.
         * @return true se encontrou; false caso contrario.
         */
        virtual bool ler(const Email& usuario, const Identificador& identificador, Projeto& projeto) = 0;

        /**
         * @brief Atualiza nome, descricao, inicio e termino de um projeto
         *        (servico 8). O projeto e localizado pelo identificador contido
         *        em projeto; o identificador nao e alterado.
         * @param usuario Email do usuario autenticado.
         * @param projeto Projeto com o identificador (chave) e os novos dados.
         * @return true se atualizou; false se nao encontrou ou se o usuario
         *         nao e GESTOR.
         */
        virtual bool atualizar(const Email& usuario, const Projeto& projeto) = 0;

        /**
         * @brief Exclui um projeto (servico 9). Recusa se o projeto possuir
         *        quadros.
         * @param usuario Email do usuario autenticado.
         * @param identificador Identificador (chave primaria) do projeto.
         * @return true se excluiu; false se nao encontrou, se o usuario nao e
         *         GESTOR ou se o projeto possui quadros.
         */
        virtual bool excluir(const Email& usuario, const Identificador& identificador) = 0;

        /**
         * @brief Lista os identificadores de todos os projetos (servico 10).
         * @param usuario Email do usuario autenticado.
         * @param lista Saida: identificadores dos projetos.
         * @return true se listou; false se o usuario nao esta cadastrado.
         */
        virtual bool listar(const Email& usuario, std::vector<Identificador>& lista) = 0;

        /**
         * @brief Destrutor virtual puro.
         */
        virtual ~IServicoProjeto() = 0;
};

/**
 * @brief Interface do servico de quadros (servicos 11 a 15).
 *
 * criar, atualizar e excluir: somente GESTOR. ler e listar: ambos os papeis.
 */
class IServicoQuadro {
    public:
        /**
         * @brief Cria um quadro ligado a um projeto (servico 11).
         * @param usuario Email do usuario autenticado.
         * @param projeto Identificador do projeto ao qual o quadro pertence.
         * @param quadro Quadro completo (identificador, nome e limite).
         * @return true se criou; false se o usuario nao e GESTOR, se o projeto
         *         nao existe ou se o identificador do quadro ja existe.
         */
        virtual bool criar(const Email& usuario, const Identificador& projeto, const Quadro& quadro) = 0;

        /**
         * @brief Le os dados de um quadro (servico 12).
         * @param usuario Email do usuario autenticado.
         * @param identificador Identificador (chave primaria) do quadro.
         * @param quadro Saida: dados do quadro.
         * @return true se encontrou; false caso contrario.
         */
        virtual bool ler(const Email& usuario, const Identificador& identificador, Quadro& quadro) = 0;

        /**
         * @brief Atualiza nome e limite de um quadro (servico 13). O quadro e
         *        localizado pelo identificador contido em quadro; o identificador
         *        nao e alterado.
         * @param usuario Email do usuario autenticado.
         * @param quadro Quadro com o identificador (chave) e os novos dados.
         * @return true se atualizou; false se nao encontrou, se o usuario nao e
         *         GESTOR ou se o novo limite e menor que o WIP atual.
         */
        virtual bool atualizar(const Email& usuario, const Quadro& quadro) = 0;

        /**
         * @brief Exclui um quadro (servico 14). Recusa se o quadro possuir
         *        cartoes.
         * @param usuario Email do usuario autenticado.
         * @param identificador Identificador (chave primaria) do quadro.
         * @return true se excluiu; false se nao encontrou, se o usuario nao e
         *         GESTOR ou se o quadro possui cartoes.
         */
        virtual bool excluir(const Email& usuario, const Identificador& identificador) = 0;

        /**
         * @brief Lista os identificadores dos quadros de um projeto (servico 15).
         * @param usuario Email do usuario autenticado.
         * @param projeto Identificador do projeto.
         * @param lista Saida: identificadores dos quadros do projeto.
         * @return true se listou; false se o projeto nao existe.
         */
        virtual bool listar(const Email& usuario, const Identificador& projeto, std::vector<Identificador>& lista) = 0;

        /**
         * @brief Destrutor virtual puro.
         */
        virtual ~IServicoQuadro() = 0;
};

/**
 * @brief Interface do servico de cartoes de atividade (servicos 16 a 26).
 *
 * criar, atualizar e excluir: somente GESTOR. Demais metodos: ambos os
 * papeis. Tempo de ciclo = termino - inicio (de FAZENDO ate FEITO).
 * Lead time = termino - entrada (de A FAZER ate FEITO).
 */
class IServicoCartao {
    public:
        /**
         * @brief Cria um cartao em um quadro, atribuido a um desenvolvedor
         *        (servico 16). O servico define estado = A FAZER e
         *        entrada = momento atual, ignorando estado, entrada, inicio e
         *        termino recebidos em cartao.
         * @param usuario Email do usuario autenticado.
         * @param quadro Identificador do quadro ao qual o cartao pertence.
         * @param desenvolvedor Email da pessoa responsavel pelo cartao.
         * @param cartao Cartao (identificador, nome, descricao, prioridade e
         *        tamanho).
         * @return true se criou; false se o usuario nao e GESTOR, se o quadro nao
         *         existe, se o desenvolvedor nao existe ou nao tem papel
         *         DESENVOLVEDOR, ou se o identificador ja existe.
         */
        virtual bool criar(const Email& usuario, const Identificador& quadro, const Email& desenvolvedor, const CartaoAtividade& cartao) = 0;

        /**
         * @brief Le os dados de um cartao (servico 17).
         * @param usuario Email do usuario autenticado.
         * @param identificador Identificador (chave primaria) do cartao.
         * @param cartao Saida: dados do cartao.
         * @return true se encontrou; false caso contrario.
         */
        virtual bool ler(const Email& usuario, const Identificador& identificador, CartaoAtividade& cartao) = 0;

        /**
         * @brief Atualiza nome, descricao, prioridade e tamanho de um cartao
         *        (servico 18). O cartao e localizado pelo identificador contido
         *        em cartao. Identificador, estado e timestamps NAO sao alterados
         *        aqui (estado e timestamps mudam somente por mover).
         * @param usuario Email do usuario autenticado.
         * @param cartao Cartao com o identificador (chave) e os novos dados.
         * @return true se atualizou; false se nao encontrou ou se o usuario nao
         *         e GESTOR.
         */
        virtual bool atualizar(const Email& usuario, const CartaoAtividade& cartao) = 0;

        /**
         * @brief Exclui um cartao (servico 19).
         * @param usuario Email do usuario autenticado.
         * @param identificador Identificador (chave primaria) do cartao.
         * @return true se excluiu; false se nao encontrou ou se o usuario nao e
         *         GESTOR.
         */
        virtual bool excluir(const Email& usuario, const Identificador& identificador) = 0;

        /**
         * @brief Lista os identificadores dos cartoes de um quadro (servico 20).
         * @param usuario Email do usuario autenticado.
         * @param quadro Identificador do quadro.
         * @param lista Saida: identificadores dos cartoes do quadro.
         * @return true se listou; false se o quadro nao existe.
         */
        virtual bool listar(const Email& usuario, const Identificador& quadro, std::vector<Identificador>& lista) = 0;

        /**
         * @brief Move o cartao para a coluna seguinte (servico 21):
         *        A FAZER -> FAZENDO (registra inicio = momento atual) ou
         *        FAZENDO -> FEITO (registra termino = momento atual).
         * @param usuario Email do usuario autenticado.
         * @param identificador Identificador do cartao.
         * @return true se moveu; false se o cartao nao existe, se ja esta em
         *         FEITO ou se o limite do quadro seria ultrapassado.
         */
        virtual bool mover(const Email& usuario, const Identificador& identificador) = 0;

        /**
         * @brief Le o tempo de ciclo de um cartao (servico 22).
         * @param usuario Email do usuario autenticado.
         * @param identificador Identificador do cartao.
         * @param minutos Saida: tempo de ciclo em minutos.
         * @return true se calculou; false se o cartao nao existe ou nao esta em
         *         FEITO.
         */
        virtual bool lerTempoCiclo(const Email& usuario, const Identificador& identificador, double& minutos) = 0;

        /**
         * @brief Le o tempo de ciclo medio dos cartoes em FEITO de um quadro
         *        (servico 23).
         * @param usuario Email do usuario autenticado.
         * @param quadro Identificador do quadro.
         * @param minutos Saida: media em minutos.
         * @return true se calculou; false se o quadro nao existe ou nao possui
         *         cartao em FEITO.
         */
        virtual bool lerTempoCicloMedio(const Email& usuario, const Identificador& quadro, double& minutos) = 0;

        /**
         * @brief Le o lead time de um cartao (servico 24).
         * @param usuario Email do usuario autenticado.
         * @param identificador Identificador do cartao.
         * @param minutos Saida: lead time em minutos.
         * @return true se calculou; false se o cartao nao existe ou nao esta em
         *         FEITO.
         */
        virtual bool lerLeadTime(const Email& usuario, const Identificador& identificador, double& minutos) = 0;

        /**
         * @brief Le o lead time medio dos cartoes em FEITO de um quadro
         *        (servico 25).
         * @param usuario Email do usuario autenticado.
         * @param quadro Identificador do quadro.
         * @param minutos Saida: media em minutos.
         * @return true se calculou; false se o quadro nao existe ou nao possui
         *         cartao em FEITO.
         */
        virtual bool lerLeadTimeMedio(const Email& usuario, const Identificador& quadro, double& minutos) = 0;

        /**
         * @brief Le o WIP de um quadro: quantidade de cartoes em FAZENDO
         *        (servico 26).
         * @param usuario Email do usuario autenticado.
         * @param quadro Identificador do quadro.
         * @param wip Saida: quantidade de cartoes em FAZENDO (pode ser 0).
         * @return true se calculou; false se o quadro nao existe.
         */
        virtual bool lerWIP(const Email& usuario, const Identificador& quadro, int& wip) = 0;

        /**
         * @brief Destrutor virtual puro.
         */
        virtual ~IServicoCartao() = 0;
};

// ---------------------------------------------------------------------------
// Interfaces da camada de apresentacao
// ---------------------------------------------------------------------------

/**
 * @brief Interface da apresentacao de autenticacao.
 */
class IApresentacaoAutenticacao {
    public:
        /**
         * @brief Solicita email e senha e tenta autenticar.
         * @param email Saida: email do usuario autenticado (preenchido somente
         *        em caso de sucesso).
         * @return true se autenticou; false caso contrario.
         */
        virtual bool executar(Email& email) = 0;

        /**
         * @brief Informa a controladora de servico a ser usada.
         * @param cntr Ponteiro para o servico de autenticacao.
         */
        virtual void setCntrServicoAutenticacao(IServicoAutenticacao* cntr) = 0;

        /**
         * @brief Destrutor virtual puro.
         */
        virtual ~IApresentacaoAutenticacao() = 0;
};

/**
 * @brief Interface da apresentacao de pessoas.
 */
class IApresentacaoPessoa {
    public:
        /**
         * @brief Exibe o menu de pessoas (servicos 1 a 5) para o usuario
         *        autenticado.
         * @param usuario Email do usuario autenticado.
         * @throw ExcecaoSessaoEncerrada Se o usuario excluir a propria conta.
         *        Quem chama (o menu principal) deve capturar e encerrar a
         *        sessao.
         */
        virtual void executar(const Email& usuario) = 0;

        /**
         * @brief Cria conta (servico 1) sem sessao, chamado a partir do menu
         *        inicial antes da autenticacao.
         * @return true se a conta foi criada; false caso contrario.
         */
        virtual bool cadastrar() = 0;

        /**
         * @brief Informa a controladora de servico a ser usada.
         * @param cntr Ponteiro para o servico de pessoas.
         */
        virtual void setCntrServicoPessoa(IServicoPessoa* cntr) = 0;

        /**
         * @brief Destrutor virtual puro.
         */
        virtual ~IApresentacaoPessoa() = 0;
};

/**
 * @brief Interface da apresentacao de projetos.
 */
class IApresentacaoProjeto {
    public:
        /**
         * @brief Exibe o menu de projetos (servicos 6 a 10).
         * @param usuario Email do usuario autenticado.
         */
        virtual void executar(const Email& usuario) = 0;

        /**
         * @brief Informa a controladora de servico a ser usada.
         * @param cntr Ponteiro para o servico de projetos.
         */
        virtual void setCntrServicoProjeto(IServicoProjeto* cntr) = 0;

        /**
         * @brief Destrutor virtual puro.
         */
        virtual ~IApresentacaoProjeto() = 0;
};

/**
 * @brief Interface da apresentacao de quadros.
 */
class IApresentacaoQuadro {
    public:
        /**
         * @brief Exibe o menu de quadros (servicos 11 a 15).
         * @param usuario Email do usuario autenticado.
         */
        virtual void executar(const Email& usuario) = 0;

        /**
         * @brief Informa a controladora de servico a ser usada.
         * @param cntr Ponteiro para o servico de quadros.
         */
        virtual void setCntrServicoQuadro(IServicoQuadro* cntr) = 0;

        /**
         * @brief Destrutor virtual puro.
         */
        virtual ~IApresentacaoQuadro() = 0;
};

/**
 * @brief Interface da apresentacao de cartoes de atividade.
 */
class IApresentacaoCartao {
    public:
        /**
         * @brief Exibe o menu de cartoes (servicos 16 a 26).
         * @param usuario Email do usuario autenticado.
         */
        virtual void executar(const Email& usuario) = 0;

        /**
         * @brief Informa a controladora de servico a ser usada.
         * @param cntr Ponteiro para o servico de cartoes.
         */
        virtual void setCntrServicoCartao(IServicoCartao* cntr) = 0;

        /**
         * @brief Destrutor virtual puro.
         */
        virtual ~IApresentacaoCartao() = 0;
};

// ---------------------------------------------------------------------------
// Definicoes (vazias) dos destrutores virtuais puros.
// Obrigatorias: o destrutor da classe derivada sempre chama o da base.
// inline evita erro de definicao multipla ao incluir em varios .cpp.
// ---------------------------------------------------------------------------

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
