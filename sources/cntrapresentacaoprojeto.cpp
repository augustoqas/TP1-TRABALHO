#include "cntrapresentacaoprojeto.hpp"

#include <sstream>
#include <vector>

const std::string CntrApresentacaoProjeto::DICA_IDENTIFICADOR = "3 letras e 3 digitos, ex.: PRJ001";
const std::string CntrApresentacaoProjeto::DICA_NOME = "1 a 15 letras, espaco simples entre palavras";
const std::string CntrApresentacaoProjeto::DICA_TEXTO = "ate 30 caracteres, comeca com maiuscula e termina com ponto";
const std::string CntrApresentacaoProjeto::DICA_TIMESTAMP = "DD-MES-AAAA-HH:MM, ex.: 15-MAR-2026-14:30";

// --- Auxiliares ---

Tela::Leitura CntrApresentacaoProjeto::lerDadosEditaveis(Projeto& projeto) {
    Nome nome;
    Tela::Leitura leitura = Tela::lerCampo("Nome", DICA_NOME, nome);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    Texto descricao;
    leitura = Tela::lerCampo("Descricao", DICA_TEXTO, descricao);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    Timestamp inicio;
    leitura = Tela::lerCampo("Inicio", DICA_TIMESTAMP, inicio);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    Timestamp termino;
    leitura = Tela::lerCampo("Termino", DICA_TIMESTAMP, termino);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    projeto.setNome(nome);
    projeto.setDescricao(descricao);
    projeto.setInicio(inicio);
    projeto.setTermino(termino);
    return Tela::Leitura::LIDO;
}

void CntrApresentacaoProjeto::mostrarProjeto(const Projeto& projeto) {
    Tela::separador();
    Tela::campo("Identificador", projeto.getIdentificador().getValor());
    Tela::campo("Nome", projeto.getNome().getValor());
    Tela::campo("Descricao", projeto.getDescricao().getValor());
    Tela::campo("Inicio", projeto.getInicio().getValor());
    Tela::campo("Termino", projeto.getTermino().getValor());
    Tela::separador();
}

// --- Menu ---

void CntrApresentacaoProjeto::executar(const Email& usuario) {
    const int CRIAR = 1;
    const int LER = 2;
    const int ATUALIZAR = 3;
    const int EXCLUIR = 4;
    const int LISTAR = 5;
    const int VOLTAR = 6;

    if (this->cntrServicoProjeto == nullptr) {
        Tela::aviso("Servico de projetos indisponivel.");
        return;
    }
    while (true) {
        int opcao = Tela::lerOpcao("MENU DE PROJETOS  (usuario: " + usuario.getValor() + ")",
                                   {"Criar projeto", "Ler projeto", "Atualizar projeto", "Excluir projeto",
                                    "Listar projetos", "Voltar"});
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

// --- Servicos 6 a 10 ---

void CntrApresentacaoProjeto::criar(const Email& usuario) {
    Tela::titulo("CRIAR PROJETO");
    Tela::info("Voce sera o gestor do projeto. Digite 0 em qualquer campo para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do projeto", DICA_IDENTIFICADOR, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    Projeto projeto;
    projeto.setIdentificador(identificador);
    if (lerDadosEditaveis(projeto) != Tela::Leitura::LIDO) {
        return;
    }
    if (!this->cntrServicoProjeto->criar(usuario, projeto)) {
        Tela::falha("Nao foi possivel criar o projeto.", {"usuario nao e GESTOR", "identificador ja existe"});
        return;
    }
    Tela::sucesso("Projeto criado com sucesso.");
}

void CntrApresentacaoProjeto::ler(const Email& usuario) {
    Tela::titulo("LER PROJETO");
    Tela::info("Digite 0 para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do projeto", DICA_IDENTIFICADOR, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    Projeto projeto;
    if (!this->cntrServicoProjeto->ler(usuario, identificador, projeto)) {
        Tela::falha("Projeto nao encontrado.");
        return;
    }
    Tela::info("Dados do projeto:");
    mostrarProjeto(projeto);
}

void CntrApresentacaoProjeto::atualizar(const Email& usuario) {
    Tela::titulo("ATUALIZAR PROJETO");
    Tela::info("Digite 0 em qualquer campo para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do projeto", DICA_IDENTIFICADOR, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    Projeto projeto;
    if (!this->cntrServicoProjeto->ler(usuario, identificador, projeto)) {
        Tela::falha("Projeto nao encontrado.");
        return;
    }
    Tela::info("Dados atuais:");
    mostrarProjeto(projeto);
    Tela::info("O identificador nao pode ser alterado.");
    Tela::info("Informe os novos dados:");

    if (lerDadosEditaveis(projeto) != Tela::Leitura::LIDO) {
        return;
    }
    if (!this->cntrServicoProjeto->atualizar(usuario, projeto)) {
        Tela::falha("Nao foi possivel atualizar.", {"projeto nao encontrado", "usuario nao e GESTOR"});
        return;
    }
    Tela::sucesso("Projeto atualizado com sucesso.");
}

void CntrApresentacaoProjeto::excluir(const Email& usuario) {
    Tela::titulo("EXCLUIR PROJETO");
    Tela::info("Digite 0 para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do projeto", DICA_IDENTIFICADOR, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    bool confirmou = false;
    if (!Tela::confirmar("Confirma a exclusao do projeto " + identificador.getValor() + "? (S/N)", confirmou)) {
        return;
    }
    if (!confirmou) {
        Tela::aviso("Exclusao cancelada.");
        return;
    }
    if (!this->cntrServicoProjeto->excluir(usuario, identificador)) {
        Tela::falha("Nao foi possivel excluir.",
                    {"projeto nao encontrado", "usuario nao e GESTOR", "projeto tem quadros (exclua-os antes)"});
        return;
    }
    Tela::sucesso("Projeto excluido com sucesso.");
}

void CntrApresentacaoProjeto::listar(const Email& usuario) {
    Tela::titulo("LISTAR PROJETOS");

    std::vector<Identificador> lista;
    if (!this->cntrServicoProjeto->listar(usuario, lista)) {
        Tela::falha("Nao foi possivel listar.");
        return;
    }
    if (lista.empty()) {
        Tela::info("Nao ha projetos cadastrados.");
        return;
    }
    std::ostringstream cabecalho;
    cabecalho << "Projetos cadastrados (" << lista.size() << "):";
    Tela::info(cabecalho.str());
    Tela::separador();
    for (std::vector<Identificador>::size_type i = 0; i < lista.size(); i++) {
        Tela::info("  " + lista[i].getValor());
    }
    Tela::separador();
}
