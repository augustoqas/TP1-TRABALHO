#include "cntrapresentacaoquadro.hpp"

#include <sstream>
#include <vector>

const std::string CntrApresentacaoQuadro::DICA_IDENTIFICADOR_PROJETO = "3 letras e 3 digitos, ex.: PRJ001";
const std::string CntrApresentacaoQuadro::DICA_IDENTIFICADOR_QUADRO = "3 letras e 3 digitos, ex.: QDR001";
const std::string CntrApresentacaoQuadro::DICA_NOME = "1 a 15 letras, espaco simples entre palavras";
const std::string CntrApresentacaoQuadro::DICA_LIMITE = "maximo de cartoes em FAZENDO, de 1 a 25";

// --- Auxiliares ---

Tela::Leitura CntrApresentacaoQuadro::lerDadosEditaveis(Quadro& quadro) {
    Nome nome;
    Tela::Leitura leitura = Tela::lerCampo("Nome", DICA_NOME, nome);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    Limite limite;
    leitura = Tela::lerCampoInteiro("Limite de WIP", DICA_LIMITE, limite);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    quadro.setNome(nome);
    quadro.setLimite(limite);
    return Tela::Leitura::LIDO;
}

void CntrApresentacaoQuadro::mostrarQuadro(const Quadro& quadro) {
    std::ostringstream limite;
    limite << quadro.getLimite().getValor();
    Tela::separador();
    Tela::campo("Identificador", quadro.getIdentificador().getValor());
    Tela::campo("Nome", quadro.getNome().getValor());
    Tela::campo("Limite de WIP", limite.str());
    Tela::separador();
}

// --- Menu ---

void CntrApresentacaoQuadro::executar(const Email& usuario) {
    const int CRIAR = 1;
    const int LER = 2;
    const int ATUALIZAR = 3;
    const int EXCLUIR = 4;
    const int LISTAR = 5;
    const int VOLTAR = 6;

    if (this->cntrServicoQuadro == nullptr) {
        Tela::aviso("Servico de quadros indisponivel.");
        return;
    }
    while (true) {
        int opcao = Tela::lerOpcao("MENU DE QUADROS  (usuario: " + usuario.getValor() + ")",
                                   {"Criar quadro", "Ler quadro", "Atualizar quadro", "Excluir quadro",
                                    "Listar quadros do projeto", "Voltar"});
        if (opcao == 0 || opcao == VOLTAR) {
            return;
        }
        if (opcao == CRIAR) {
            criar(usuario);
        } else if (opcao == LER) {
            ler(usuario);
        } else if (opcao == ATUALIZAR) {
            atualizar(usuario);
        } else if (opcao == EXCLUIR) {
            excluir(usuario);
        } else if (opcao == LISTAR) {
            listar(usuario);
        }
        if (Tela::entradaEncerrada()) {
            return;
        }
    }
}

// --- Servicos 11 a 15 ---

void CntrApresentacaoQuadro::criar(const Email& usuario) {
    Tela::titulo("CRIAR QUADRO");
    Tela::info("Digite 0 em qualquer campo para cancelar.");
    Tela::separador();

    Identificador projeto;
    if (Tela::lerCampo("Identificador do projeto", DICA_IDENTIFICADOR_PROJETO, projeto) != Tela::Leitura::LIDO) {
        return;
    }
    Identificador identificador;
    if (Tela::lerCampo("Identificador do quadro", DICA_IDENTIFICADOR_QUADRO, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    Quadro quadro;
    quadro.setIdentificador(identificador);
    if (lerDadosEditaveis(quadro) != Tela::Leitura::LIDO) {
        return;
    }
    if (!this->cntrServicoQuadro->criar(usuario, projeto, quadro)) {
        Tela::falha("Nao foi possivel criar o quadro.",
                    {"voce nao e o gestor do projeto", "projeto nao existe", "identificador ja existe"});
        return;
    }
    Tela::sucesso("Quadro criado com sucesso.");
}

void CntrApresentacaoQuadro::ler(const Email& usuario) {
    Tela::titulo("LER QUADRO");
    Tela::info("Digite 0 para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do quadro", DICA_IDENTIFICADOR_QUADRO, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    Quadro quadro;
    if (!this->cntrServicoQuadro->ler(usuario, identificador, quadro)) {
        Tela::falha("Quadro nao encontrado.");
        return;
    }
    Tela::info("Dados do quadro:");
    mostrarQuadro(quadro);
}

void CntrApresentacaoQuadro::atualizar(const Email& usuario) {
    Tela::titulo("ATUALIZAR QUADRO");
    Tela::info("Digite 0 em qualquer campo para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do quadro", DICA_IDENTIFICADOR_QUADRO, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    Quadro quadro;
    if (!this->cntrServicoQuadro->ler(usuario, identificador, quadro)) {
        Tela::falha("Quadro nao encontrado.");
        return;
    }
    Tela::info("Dados atuais:");
    mostrarQuadro(quadro);
    Tela::info("O identificador e o projeto do quadro nao podem ser alterados.");
    Tela::info("Informe os novos dados:");

    if (lerDadosEditaveis(quadro) != Tela::Leitura::LIDO) {
        return;
    }
    if (!this->cntrServicoQuadro->atualizar(usuario, quadro)) {
        Tela::falha("Nao foi possivel atualizar.",
                    {"quadro nao encontrado", "voce nao e o gestor do projeto", "novo limite menor que o WIP atual"});
        return;
    }
    Tela::sucesso("Quadro atualizado com sucesso.");
}

void CntrApresentacaoQuadro::excluir(const Email& usuario) {
    Tela::titulo("EXCLUIR QUADRO");
    Tela::info("Digite 0 para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do quadro", DICA_IDENTIFICADOR_QUADRO, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    bool confirmou = false;
    if (!Tela::confirmar("Confirma a exclusao do quadro " + identificador.getValor() + "? (S/N)", confirmou)) {
        return;
    }
    if (!confirmou) {
        Tela::aviso("Exclusao cancelada.");
        return;
    }
    if (!this->cntrServicoQuadro->excluir(usuario, identificador)) {
        Tela::falha("Nao foi possivel excluir.",
                    {"quadro nao encontrado", "voce nao e o gestor do projeto", "quadro tem cartoes (exclua-os antes)"});
        return;
    }
    Tela::sucesso("Quadro excluido com sucesso.");
}

void CntrApresentacaoQuadro::listar(const Email& usuario) {
    Tela::titulo("LISTAR QUADROS DO PROJETO");
    Tela::info("Digite 0 para cancelar.");
    Tela::separador();

    Identificador projeto;
    if (Tela::lerCampo("Identificador do projeto", DICA_IDENTIFICADOR_PROJETO, projeto) != Tela::Leitura::LIDO) {
        return;
    }
    std::vector<Identificador> lista;
    if (!this->cntrServicoQuadro->listar(usuario, projeto, lista)) {
        Tela::falha("Nao foi possivel listar.", {"projeto nao existe"});
        return;
    }
    if (lista.empty()) {
        Tela::info("O projeto nao tem quadros.");
        return;
    }
    std::ostringstream cabecalho;
    cabecalho << "Quadros do projeto " << projeto.getValor() << " (" << lista.size() << "):";
    Tela::info(cabecalho.str());
    Tela::separador();
    for (std::vector<Identificador>::size_type i = 0; i < lista.size(); i++) {
        Tela::info("  " + lista[i].getValor());
    }
    Tela::separador();
}
