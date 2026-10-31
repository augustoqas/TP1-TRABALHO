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
#include "cntrservicoautenticacao.hpp"
#include "cntrservicocartao.hpp"
#include "cntrservicopessoa.hpp"
#include "containercartao.hpp"
#include "containerpessoa.hpp"
#include "containerprojeto.hpp"

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
    ContainerCartao containerCartao;
    // COLEGAS: criar aqui os outros conteineres (ex.: ContainerQuadro).

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
