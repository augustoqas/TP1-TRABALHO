/**
 * @file main_teste.cpp
 * @brief Igual ao main.cpp, mas ja comeca com um projeto e um cartao para
 *        testar os vinculos de Pessoa.
 *
 * Projeto PRJ001 com gestor gestor@unb.br e cartao CRT001 com desenvolvedor dev@unb.br.
 * As contas dessas pessoas sao criadas pelo menu, no proprio roteiro.
 *
 * Fora dos blocos "DADOS DE TESTE" este arquivo tem que ser igual ao main.cpp:
 * o rodar compara os dois e avisa se divergirem. Roda tambem os roteiros
 * @apenas_teste (ver testes/README.md).
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

// INICIO DADOS DE TESTE
#include "cartaoatividade.hpp"
#include "email.hpp"
#include "identificador.hpp"
#include "projeto.hpp"
// FIM DADOS DE TESTE

int main() {
    // 1. Conteineres.
    ContainerPessoa containerPessoa;
    ContainerProjeto containerProjeto;
    ContainerQuadro containerQuadro;
    ContainerCartao containerCartao;

    // INICIO DADOS DE TESTE
    Identificador identificadorProjeto;
    identificadorProjeto.setValor("PRJ001");
    Projeto projeto;
    projeto.setIdentificador(identificadorProjeto);
    Email gestor;
    gestor.setValor("gestor@unb.br");
    containerProjeto.incluir(projeto, gestor);

    Identificador identificadorCartao;
    identificadorCartao.setValor("CRT001");
    CartaoAtividade cartao;
    cartao.setIdentificador(identificadorCartao);
    Identificador identificadorQuadro;
    identificadorQuadro.setValor("QDR001");
    Email desenvolvedor;
    desenvolvedor.setValor("dev@unb.br");
    containerCartao.incluir(cartao, identificadorQuadro, desenvolvedor);
    // FIM DADOS DE TESTE

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
