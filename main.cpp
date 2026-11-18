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
#include "cntrservicoautenticacao.hpp"
#include "cntrservicocartao.hpp"
#include "cntrservicopessoa.hpp"
#include "containercartao.hpp"
#include "containerpessoa.hpp"
#include "containerprojeto.hpp"

int main() {
    // 1. Conteineres.
    ContainerPessoa containerPessoa;
    ContainerProjeto containerProjeto;
    ContainerCartao containerCartao;
    // COLEGAS: criar aqui os outros conteineres (ex.: ContainerQuadro).

    // 2. Controladoras de servico.
    CntrServicoAutenticacao cntrServicoAutenticacao;
    cntrServicoAutenticacao.setContainerPessoa(&containerPessoa);

    CntrServicoPessoa cntrServicoPessoa;
    cntrServicoPessoa.setContainerPessoa(&containerPessoa);
    cntrServicoPessoa.setContainerProjeto(&containerProjeto);
    cntrServicoPessoa.setContainerCartao(&containerCartao);

    CntrServicoCartao cntrServicoCartao;
    cntrServicoCartao.setContainerCartao(&containerCartao);
    cntrServicoCartao.setContainerPessoa(&containerPessoa);
    // COLEGAS: trocar nullptr por &cntrServicoQuadro quando o servico de Quadro existir.
    // Enquanto for nullptr, criar, listar, mover para FAZENDO, medias e WIP retornam false.
    cntrServicoCartao.setCntrServicoQuadro(nullptr);

    // COLEGAS: criar aqui os servicos de Projeto e Quadro e passar os
    // conteineres com setContainer*(&...).

    // 3. Controladoras de apresentacao.
    CntrApresentacaoAutenticacao cntrApresentacaoAutenticacao;
    cntrApresentacaoAutenticacao.setCntrServicoAutenticacao(&cntrServicoAutenticacao);

    CntrApresentacaoPessoa cntrApresentacaoPessoa;
    cntrApresentacaoPessoa.setCntrServicoPessoa(&cntrServicoPessoa);

    CntrApresentacaoCartao cntrApresentacaoCartao;
    cntrApresentacaoCartao.setCntrServicoCartao(&cntrServicoCartao);

    // COLEGAS: criar aqui as telas de Projeto e Quadro e passar os
    // servicos com setCntrServico*(&...).

    // 4. Controle (menus inicial e principal).
    CntrApresentacaoControle cntrApresentacaoControle;
    cntrApresentacaoControle.setCntrApresentacaoAutenticacao(&cntrApresentacaoAutenticacao);
    cntrApresentacaoControle.setCntrApresentacaoPessoa(&cntrApresentacaoPessoa);

    // COLEGAS: trocar nullptr pelas telas de Projeto e Quadro.
    // Enquanto for nullptr, o menu mostra "Funcionalidade ainda nao disponivel".
    cntrApresentacaoControle.setCntrApresentacaoProjeto(nullptr);
    cntrApresentacaoControle.setCntrApresentacaoQuadro(nullptr);
    cntrApresentacaoControle.setCntrApresentacaoCartao(&cntrApresentacaoCartao);

    cntrApresentacaoControle.executar();
    return 0;
}
