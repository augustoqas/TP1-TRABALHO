/**
 * @file main.cpp
 * @brief Programa principal do sistema Kanban: cria e conecta as camadas.
 *
 * Ordem de montagem (injecao de dependencia por setters):
 * 1. Conteineres (armazenamento em memoria, unicos no programa).
 * 2. Controladoras de servico, que recebem os conteineres.
 * 3. Controladoras de apresentacao, que recebem os servicos.
 * 4. Controladora do menu (Controle), que recebe as apresentacoes e executa.
 *
 * Todos os objetos sao locais do main: vivem ate o fim do programa e nenhuma
 * controladora e dona dos objetos que recebe (nao ha new/delete).
 */

#include "cntrapresentacaoautenticacao.hpp"
#include "cntrapresentacaocontrole.hpp"
#include "cntrapresentacaopessoa.hpp"
#include "cntrservicoautenticacao.hpp"
#include "cntrservicopessoa.hpp"
#include "containercartao.hpp"
#include "containerpessoa.hpp"
#include "containerprojeto.hpp"

int main() {
    // 1. Conteineres.
    ContainerPessoa containerPessoa;
    ContainerProjeto containerProjeto;
    ContainerCartao containerCartao;
    // COLEGAS: criar aqui os demais conteineres (ex.: ContainerQuadro).

    // 2. Controladoras de servico.
    CntrServicoAutenticacao cntrServicoAutenticacao;
    cntrServicoAutenticacao.setContainerPessoa(&containerPessoa);

    CntrServicoPessoa cntrServicoPessoa;
    cntrServicoPessoa.setContainerPessoa(&containerPessoa);
    cntrServicoPessoa.setContainerProjeto(&containerProjeto);
    cntrServicoPessoa.setContainerCartao(&containerCartao);

    // COLEGAS: criar aqui CntrServicoProjeto, CntrServicoQuadro e
    // CntrServicoCartao e informar os conteineres com setContainer*(&...).

    // 3. Controladoras de apresentacao.
    CntrApresentacaoAutenticacao cntrApresentacaoAutenticacao;
    cntrApresentacaoAutenticacao.setCntrServicoAutenticacao(&cntrServicoAutenticacao);

    CntrApresentacaoPessoa cntrApresentacaoPessoa;
    cntrApresentacaoPessoa.setCntrServicoPessoa(&cntrServicoPessoa);

    // COLEGAS: criar aqui CntrApresentacaoProjeto, CntrApresentacaoQuadro e
    // CntrApresentacaoCartao e informar os servicos com setCntrServico*(&...).

    // 4. Controle (menus inicial e principal).
    CntrApresentacaoControle cntrApresentacaoControle;
    cntrApresentacaoControle.setCntrApresentacaoAutenticacao(&cntrApresentacaoAutenticacao);
    cntrApresentacaoControle.setCntrApresentacaoPessoa(&cntrApresentacaoPessoa);

    // COLEGAS: plugar aqui as apresentacoes de Projeto, Quadro e Cartao.
    // Enquanto ficarem nulas, o menu mostra "Funcionalidade ainda nao disponivel".
    cntrApresentacaoControle.setCntrApresentacaoProjeto(nullptr);
    cntrApresentacaoControle.setCntrApresentacaoQuadro(nullptr);
    cntrApresentacaoControle.setCntrApresentacaoCartao(nullptr);

    cntrApresentacaoControle.executar();
    return 0;
}
