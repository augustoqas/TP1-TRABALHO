#include "cntrapresentacaocontrole.hpp"

#include "excecaosessaoencerrada.hpp"
#include "tela.hpp"

void CntrApresentacaoControle::mostrarIndisponivel() {
    Tela::aviso("Funcionalidade ainda nao disponivel.");
}

void CntrApresentacaoControle::mostrarFimEntrada() {
    Tela::aviso("Fim da entrada de dados. Programa encerrado.");
}

void CntrApresentacaoControle::executar() {
    const int CRIAR_CONTA = 1;
    const int ENTRAR = 2;
    const int SAIR = 3;

    Tela::titulo("SISTEMA KANBAN - bem-vindo!");
    while (true) {
        int opcao = Tela::lerOpcao("MENU INICIAL", {"Criar conta", "Entrar", "Sair"});
        if (opcao == 0) {
            mostrarFimEntrada();
            return;
        }
        if (opcao == SAIR) {
            Tela::titulo("Sistema encerrado. Ate logo!");
            return;
        }
        if (opcao == CRIAR_CONTA) {
            if (this->cntrApresentacaoPessoa == nullptr) {
                mostrarIndisponivel();
            } else {
                this->cntrApresentacaoPessoa->cadastrar();
            }
        } else if (opcao == ENTRAR) {
            if (this->cntrApresentacaoAutenticacao == nullptr) {
                mostrarIndisponivel();
            } else {
                Email usuario;
                if (this->cntrApresentacaoAutenticacao->executar(usuario)) {
                    executarMenuPrincipal(usuario);
                }
            }
        }
        // Se a entrada acabou dentro de outra tela, ela so retorna; quem encerra e aqui.
        if (Tela::entradaEncerrada()) {
            mostrarFimEntrada();
            return;
        }
    }
}

void CntrApresentacaoControle::executarMenuPrincipal(const Email& usuario) {
    const int PESSOAS = 1;
    const int PROJETOS = 2;
    const int QUADROS = 3;
    const int CARTOES = 4;
    const int SAIR = 5;

    const std::string titulo = "MENU PRINCIPAL  (usuario: " + usuario.getValor() + ")";
    while (true) {
        int opcao = Tela::lerOpcao(titulo, {"Pessoas", "Projetos", "Quadros", "Cartoes", "Sair (encerrar sessao)"});
        if (opcao == 0) {
            return;
        }
        if (opcao == SAIR) {
            Tela::sucesso("Sessao encerrada.");
            return;
        }
        if (opcao == PESSOAS) {
            if (!executarPessoas(usuario)) {
                return;
            }
        } else if (opcao == PROJETOS) {
            if (this->cntrApresentacaoProjeto == nullptr) {
                mostrarIndisponivel();
            } else {
                this->cntrApresentacaoProjeto->executar(usuario);
            }
        } else if (opcao == QUADROS) {
            if (this->cntrApresentacaoQuadro == nullptr) {
                mostrarIndisponivel();
            } else {
                this->cntrApresentacaoQuadro->executar(usuario);
            }
        } else if (opcao == CARTOES) {
            if (this->cntrApresentacaoCartao == nullptr) {
                mostrarIndisponivel();
            } else {
                this->cntrApresentacaoCartao->executar(usuario);
            }
        }
        if (Tela::entradaEncerrada()) {
            return;
        }
    }
}

bool CntrApresentacaoControle::executarPessoas(const Email& usuario) {
    if (this->cntrApresentacaoPessoa == nullptr) {
        mostrarIndisponivel();
        return true;
    }
    try {
        this->cntrApresentacaoPessoa->executar(usuario);
    } catch (const ExcecaoSessaoEncerrada&) {
        // A tela de pessoas ja avisou o usuario.
        return false;
    }
    return true;
}
