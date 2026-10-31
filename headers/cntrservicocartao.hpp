#ifndef CNTRSERVICOCARTAO_HPP_INCLUDED
#define CNTRSERVICOCARTAO_HPP_INCLUDED

#include <string>
#include <vector>

#include "containercartao.hpp"
#include "containerpessoa.hpp"
#include "interfaces.hpp"

/**
 * @brief Servico de cartoes (servicos 16 a 26): aplica as regras de cartao e
 *        calcula as metricas Kanban.
 *
 * Usa o conteiner de cartoes e o de pessoas (para conferir papeis), passados por
 * setter, e o servico de quadros (IServicoQuadro), para saber se um quadro existe
 * e qual e o seu limite de WIP. So guarda os ponteiros; o main mantem tudo vivo.
 * Se faltar alguma dessas dependencias, o metodo que precisa dela retorna false.
 *
 * Regras:
 * - Criar, atualizar e excluir: so usuario com papel GESTOR.
 * - Ler, listar, mover e metricas: qualquer usuario cadastrado.
 * - Cartao novo entra em A FAZER com entrada = agora; estado e instantes so mudam por mover.
 * - Mover: A FAZER -> FAZENDO (grava inicio, respeita o limite de WIP) e
 *   FAZENDO -> FEITO (grava termino). Cartao em FEITO nao se move.
 * - Tempos em minutos: ciclo = termino - inicio; lead time = termino - entrada.
 */
class CntrServicoCartao : public IServicoCartao {
    public:
        /** @brief Funcao que informa o instante atual (ver setRelogio). */
        typedef Timestamp (*Relogio)();

    private:
        ContainerCartao* containerCartao = nullptr;
        ContainerPessoa* containerPessoa = nullptr;
        IServicoQuadro* cntrServicoQuadro = nullptr;
        Relogio relogio = nullptr;

        bool usuarioCadastrado(const Email& usuario) const;

        // true se a pessoa existe e tem o papel informado ("GESTOR" ou "DESENVOLVEDOR").
        bool temPapel(const Email& email, const std::string& papel) const;

        // Busca o quadro pelo servico de quadros. false se ele nao existir.
        bool lerQuadro(const Email& usuario, const Identificador& identificador, Quadro& quadro);

        // Quantos cartoes do quadro estao em FAZENDO.
        int contarFazendo(const Identificador& quadro) const;

        // Media de ciclo (true) ou de lead time (false) dos cartoes em FEITO do quadro.
        bool calcularMedia(const Email& usuario, const Identificador& quadro, bool ciclo, double& minutos);

        Timestamp agora() const;

        // Confere identificador, nome, descricao, prioridade e tamanho preenchidos.
        // Dominio nunca preenchido fica com valor vazio.
        static bool cartaoCompleto(const CartaoAtividade& cartao);

    public:
        /** @brief Define o conteiner de cartoes. */
        void setContainerCartao(ContainerCartao* containerCartao);

        /** @brief Define o conteiner de pessoas (para conferir GESTOR e DESENVOLVEDOR). */
        void setContainerPessoa(ContainerPessoa* containerPessoa);

        /** @brief Define o servico de quadros (para conferir o quadro e ler o limite de WIP). */
        void setCntrServicoQuadro(IServicoQuadro* cntr);

        /**
         * @brief Troca o relogio usado para registrar entrada, inicio e termino.
         *
         * Sem relogio definido (padrao), usa a data e a hora do sistema. Serve para
         * os testes simularem a passagem do tempo.
         */
        void setRelogio(Relogio relogio);

        /**
         * @brief Converte um instante valido em minutos desde 01-JAN-2000-00:00.
         * @param instante Instante no formato DIA-MES-ANO-HH:MM.
         * @return Minutos desde 01-JAN-2000-00:00, ou -1 se o instante estiver vazio.
         */
        static long converterMinutos(const Timestamp& instante);

        /** @brief Cria o cartao em A FAZER (servico 16). Ver IServicoCartao::criar. */
        bool criar(const Email& usuario, const Identificador& quadro, const Email& desenvolvedor,
                   const CartaoAtividade& cartao) override;

        /** @brief Le um cartao (servico 17). Ver IServicoCartao::ler. */
        bool ler(const Email& usuario, const Identificador& identificador, CartaoAtividade& cartao) override;

        /** @brief Atualiza nome, descricao, prioridade e tamanho (servico 18). Ver IServicoCartao::atualizar. */
        bool atualizar(const Email& usuario, const CartaoAtividade& cartao) override;

        /** @brief Exclui o cartao (servico 19). Ver IServicoCartao::excluir. */
        bool excluir(const Email& usuario, const Identificador& identificador) override;

        /** @brief Lista os cartoes do quadro (servico 20). Ver IServicoCartao::listar. */
        bool listar(const Email& usuario, const Identificador& quadro, std::vector<Identificador>& lista) override;

        /** @brief Passa o cartao para a proxima coluna (servico 21). Ver IServicoCartao::mover. */
        bool mover(const Email& usuario, const Identificador& identificador) override;

        /** @brief Tempo de ciclo do cartao (servico 22). Ver IServicoCartao::lerTempoCiclo. */
        bool lerTempoCiclo(const Email& usuario, const Identificador& identificador, double& minutos) override;

        /** @brief Tempo de ciclo medio do quadro (servico 23). Ver IServicoCartao::lerTempoCicloMedio. */
        bool lerTempoCicloMedio(const Email& usuario, const Identificador& quadro, double& minutos) override;

        /** @brief Lead time do cartao (servico 24). Ver IServicoCartao::lerLeadTime. */
        bool lerLeadTime(const Email& usuario, const Identificador& identificador, double& minutos) override;

        /** @brief Lead time medio do quadro (servico 25). Ver IServicoCartao::lerLeadTimeMedio. */
        bool lerLeadTimeMedio(const Email& usuario, const Identificador& quadro, double& minutos) override;

        /** @brief Cartoes do quadro em FAZENDO (servico 26). Ver IServicoCartao::lerWIP. */
        bool lerWIP(const Email& usuario, const Identificador& quadro, int& wip) override;
};

inline void CntrServicoCartao::setContainerCartao(ContainerCartao* containerCartao) {
    this->containerCartao = containerCartao;
}

inline void CntrServicoCartao::setContainerPessoa(ContainerPessoa* containerPessoa) {
    this->containerPessoa = containerPessoa;
}

inline void CntrServicoCartao::setCntrServicoQuadro(IServicoQuadro* cntr) {
    this->cntrServicoQuadro = cntr;
}

inline void CntrServicoCartao::setRelogio(Relogio relogio) {
    this->relogio = relogio;
}

#endif // CNTRSERVICOCARTAO_HPP_INCLUDED
