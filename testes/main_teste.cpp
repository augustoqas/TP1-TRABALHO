/**
 * @file main_teste.cpp
 * @brief Programa de TESTE: igual ao main.cpp, mas pre-carrega um projeto e um
 *        cartao nos conteineres para testar os vinculos de Pessoa.
 *
 * Dados pre-carregados (incluidos direto nos conteineres, sem servico):
 * - projeto "PRJ001" com gestor gestor@unb.br;
 * - cartao "CRT001" com desenvolvedor dev@unb.br.
 * As pessoas gestor@unb.br e dev@unb.br NAO sao criadas aqui: o roteiro de
 * teste cria as contas pelo menu.
 *
 * Fora dos blocos "INICIO/FIM DADOS DE TESTE", este arquivo deve ser IGUAL ao
 * main.cpp: o rodar compara os dois e avisa se divergirem (ao mudar o main.cpp,
 * copie a mudanca para ca). Roda todos os roteiros, inclusive os marcados com
 * @apenas_teste (ver testes/README.md).
 */

#include "cntrapresentacaoautenticacao.hpp"
#include "cntrapresentacaocontrole.hpp"
#include "cntrapresentacaopessoa.hpp"
#include "cntrservicoautenticacao.hpp"
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
    // COLEGAS: criar aqui os demais conteineres (ex.: ContainerQuadro).

    // INICIO DADOS DE TESTE
    // Projeto PRJ001 (gestor gestor@unb.br).
    Identificador identificadorProjeto;
    identificadorProjeto.setValor("PRJ001");
    Projeto projeto;
    projeto.setIdentificador(identificadorProjeto);
    Email gestor;
    gestor.setValor("gestor@unb.br");
    containerProjeto.incluir(projeto, gestor);

    // Cartao CRT001 (desenvolvedor dev@unb.br).
    Identificador identificadorCartao;
    identificadorCartao.setValor("CRT001");
    CartaoAtividade cartao;
    cartao.setIdentificador(identificadorCartao);
    Email desenvolvedor;
    desenvolvedor.setValor("dev@unb.br");
    containerCartao.incluir(cartao, desenvolvedor);
    // FIM DADOS DE TESTE

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
