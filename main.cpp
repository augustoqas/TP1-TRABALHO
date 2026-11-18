/**
 * @file main.cpp
 * @brief Monta o sistema Kanban e roda o menu.
 *
 * Ordem: conteineres -> servicos -> telas -> Controle, ligados por setters.
 * Tudo vive no main ate o fim do programa; nenhuma controladora e dona do que
 * recebe (nao ha new/delete).
 */

#include "cntrapresentacaoautenticacao.hpp"
#include "cntrapresentacaocartao.hpp"
#include "cntrapresentacaocontrole.hpp"
#include "cntrapresentacaopessoa.hpp"
#include "cntrapresentacaoprojeto.hpp"
#include "cntrapresentacaoquadro.hpp"
#include "cntrservicoautenticacao.hpp"
#include "cntrservicocartao.hpp"
#include "cntrservicopessoa.hpp"
#include "cntrservicoprojeto.hpp"
#include "cntrservicoquadro.hpp"
#include "containercartao.hpp"
#include "containerpessoa.hpp"
#include "containerprojeto.hpp"
#include "containerquadro.hpp"

int main() {
    // 1. Conteineres.
    ContainerPessoa containerPessoa;
    ContainerProjeto containerProjeto;
    ContainerQuadro containerQuadro;
    ContainerCartao containerCartao;

    // 2. Controladoras de servico.
    CntrServicoAutenticacao cntrServicoAutenticacao;
    cntrServicoAutenticacao.setContainerPessoa(&containerPessoa);

    CntrServicoPessoa cntrServicoPessoa;
    cntrServicoPessoa.setContainerPessoa(&containerPessoa);
    cntrServicoPessoa.setContainerProjeto(&containerProjeto);
    cntrServicoPessoa.setContainerCartao(&containerCartao);

    CntrServicoProjeto cntrServicoProjeto;
    cntrServicoProjeto.setContainerPessoa(&containerPessoa);
    cntrServicoProjeto.setContainerProjeto(&containerProjeto);
    cntrServicoProjeto.setContainerQuadro(&containerQuadro);

    CntrServicoQuadro cntrServicoQuadro;
    cntrServicoQuadro.setContainerPessoa(&containerPessoa);
    cntrServicoQuadro.setContainerProjeto(&containerProjeto);
    cntrServicoQuadro.setContainerQuadro(&containerQuadro);
    cntrServicoQuadro.setContainerCartao(&containerCartao);

    CntrServicoCartao cntrServicoCartao;
    cntrServicoCartao.setContainerCartao(&containerCartao);
    cntrServicoCartao.setContainerPessoa(&containerPessoa);
    cntrServicoCartao.setContainerQuadro(&containerQuadro);
    cntrServicoCartao.setContainerProjeto(&containerProjeto);
    cntrServicoCartao.setCntrServicoQuadro(&cntrServicoQuadro);

    // 3. Controladoras de apresentacao.
    CntrApresentacaoAutenticacao cntrApresentacaoAutenticacao;
    cntrApresentacaoAutenticacao.setCntrServicoAutenticacao(&cntrServicoAutenticacao);

    CntrApresentacaoPessoa cntrApresentacaoPessoa;
    cntrApresentacaoPessoa.setCntrServicoPessoa(&cntrServicoPessoa);

    CntrApresentacaoProjeto cntrApresentacaoProjeto;
    cntrApresentacaoProjeto.setCntrServicoProjeto(&cntrServicoProjeto);

    CntrApresentacaoQuadro cntrApresentacaoQuadro;
    cntrApresentacaoQuadro.setCntrServicoQuadro(&cntrServicoQuadro);

    CntrApresentacaoCartao cntrApresentacaoCartao;
    cntrApresentacaoCartao.setCntrServicoCartao(&cntrServicoCartao);

    // 4. Controle (menus inicial e principal).
    CntrApresentacaoControle cntrApresentacaoControle;
    cntrApresentacaoControle.setCntrApresentacaoAutenticacao(&cntrApresentacaoAutenticacao);
    cntrApresentacaoControle.setCntrApresentacaoPessoa(&cntrApresentacaoPessoa);
    cntrApresentacaoControle.setCntrApresentacaoProjeto(&cntrApresentacaoProjeto);
    cntrApresentacaoControle.setCntrApresentacaoQuadro(&cntrApresentacaoQuadro);
    cntrApresentacaoControle.setCntrApresentacaoCartao(&cntrApresentacaoCartao);

    cntrApresentacaoControle.executar();
    return 0;
}
