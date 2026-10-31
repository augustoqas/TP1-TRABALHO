#include "cntrapresentacaocartao.hpp"

#include <sstream>
#include <vector>

const std::string CntrApresentacaoCartao::DICA_IDENTIFICADOR = "3 letras e 3 digitos, ex.: CRT001";
const std::string CntrApresentacaoCartao::DICA_EMAIL = "formato nome@dominio, letras minusculas";
const std::string CntrApresentacaoCartao::DICA_NOME = "1 a 15 letras, espaco simples entre palavras";
const std::string CntrApresentacaoCartao::DICA_TEXTO = "ate 30 caracteres, comeca com maiuscula e termina com ponto";
const std::string CntrApresentacaoCartao::DICA_PRIORIDADE = "ALTA, MEDIA ou BAIXA, em maiusculas";
const std::string CntrApresentacaoCartao::DICA_TAMANHO = "GRANDE, MEDIO ou PEQUENO, em maiusculas";

// --- Auxiliares ---

Tela::Leitura CntrApresentacaoCartao::lerDadosEditaveis(CartaoAtividade& cartao) {
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
    Prioridade prioridade;
    leitura = Tela::lerCampo("Prioridade", DICA_PRIORIDADE, prioridade);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    Tamanho tamanho;
    leitura = Tela::lerCampo("Tamanho", DICA_TAMANHO, tamanho);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    cartao.setNome(nome);
    cartao.setDescricao(descricao);
    cartao.setPrioridade(prioridade);
    cartao.setTamanho(tamanho);
    return Tela::Leitura::LIDO;
}

void CntrApresentacaoCartao::mostrarCartao(const CartaoAtividade& cartao) {
    std::string inicio = cartao.getInicio().getValor();
    std::string termino = cartao.getTermino().getValor();
    Tela::separador();
    Tela::campo("Identificador", cartao.getIdentificador().getValor());
    Tela::campo("Nome", cartao.getNome().getValor());
    Tela::campo("Descricao", cartao.getDescricao().getValor());
    Tela::campo("Prioridade", cartao.getPrioridade().getValor());
    Tela::campo("Estado", cartao.getEstado().getValor());
    Tela::campo("Tamanho", cartao.getTamanho().getValor());
    Tela::campo("Entrada", cartao.getEntrada().getValor());
    Tela::campo("Inicio", inicio.empty() ? "-" : inicio);
    Tela::campo("Termino", termino.empty() ? "-" : termino);
    Tela::separador();
}

std::string CntrApresentacaoCartao::formatarMinutos(double minutos) {
    long total = static_cast<long>(minutos + 0.5);
    std::ostringstream texto;
    texto << minutos << " minutos (" << total / 1440 << " d " << (total % 1440) / 60 << " h " << total % 60 << " min)";
    return texto.str();
}

// --- Menu ---

void CntrApresentacaoCartao::executar(const Email& usuario) {
    const int CRIAR = 1;
    const int LER = 2;
    const int ATUALIZAR = 3;
    const int EXCLUIR = 4;
    const int LISTAR = 5;
    const int MOVER = 6;
    const int TEMPO_CICLO = 7;
    const int TEMPO_CICLO_MEDIO = 8;
    const int LEAD_TIME = 9;
    const int LEAD_TIME_MEDIO = 10;
    const int WIP = 11;
    const int VOLTAR = 12;

    if (this->cntrServicoCartao == nullptr) {
        Tela::aviso("Servico de cartoes indisponivel.");
        return;
    }
    while (true) {
        int opcao = Tela::lerOpcao("MENU DE CARTOES  (usuario: " + usuario.getValor() + ")",
                                   {"Criar cartao", "Ler cartao", "Atualizar cartao", "Excluir cartao",
                                    "Listar cartoes do quadro", "Mover cartao", "Ler tempo de ciclo",
                                    "Ler tempo de ciclo medio", "Ler lead time", "Ler lead time medio",
                                    "Ler WIP", "Voltar"});
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
        } else if (opcao == MOVER) {
            mover(usuario);
        } else if (opcao == TEMPO_CICLO) {
            lerTempoCiclo(usuario);
        } else if (opcao == TEMPO_CICLO_MEDIO) {
            lerTempoCicloMedio(usuario);
        } else if (opcao == LEAD_TIME) {
            lerLeadTime(usuario);
        } else if (opcao == LEAD_TIME_MEDIO) {
            lerLeadTimeMedio(usuario);
        } else if (opcao == WIP) {
            lerWIP(usuario);
        }
        if (Tela::entradaEncerrada()) {
            return;
        }
    }
}

// --- Servicos 16 a 20 ---

void CntrApresentacaoCartao::criar(const Email& usuario) {
    Tela::titulo("CRIAR CARTAO");
    Tela::info("O cartao entra na coluna A FAZER. Digite 0 em qualquer campo para cancelar.");
    Tela::separador();

    Identificador quadro;
    if (Tela::lerCampo("Identificador do quadro", DICA_IDENTIFICADOR, quadro) != Tela::Leitura::LIDO) {
        return;
    }
    Identificador identificador;
    if (Tela::lerCampo("Identificador do cartao", DICA_IDENTIFICADOR, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    CartaoAtividade cartao;
    cartao.setIdentificador(identificador);
    if (lerDadosEditaveis(cartao) != Tela::Leitura::LIDO) {
        return;
    }
    Email desenvolvedor;
    if (Tela::lerCampo("Email do desenvolvedor", DICA_EMAIL, desenvolvedor) != Tela::Leitura::LIDO) {
        return;
    }
    if (!this->cntrServicoCartao->criar(usuario, quadro, desenvolvedor, cartao)) {
        Tela::falha("Nao foi possivel criar o cartao.",
                    {"usuario nao e GESTOR", "quadro nao existe",
                     "desenvolvedor nao existe ou nao tem papel DESENVOLVEDOR", "identificador ja existe"});
        return;
    }
    Tela::sucesso("Cartao criado com sucesso na coluna A FAZER.");
}

void CntrApresentacaoCartao::ler(const Email& usuario) {
    Tela::titulo("LER CARTAO");
    Tela::info("Digite 0 para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do cartao", DICA_IDENTIFICADOR, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    CartaoAtividade cartao;
    if (!this->cntrServicoCartao->ler(usuario, identificador, cartao)) {
        Tela::falha("Cartao nao encontrado.");
        return;
    }
    Tela::info("Dados do cartao:");
    mostrarCartao(cartao);
}

void CntrApresentacaoCartao::atualizar(const Email& usuario) {
    Tela::titulo("ATUALIZAR CARTAO");
    Tela::info("Digite 0 em qualquer campo para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do cartao", DICA_IDENTIFICADOR, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    CartaoAtividade cartao;
    if (!this->cntrServicoCartao->ler(usuario, identificador, cartao)) {
        Tela::falha("Cartao nao encontrado.");
        return;
    }
    Tela::info("Dados atuais:");
    mostrarCartao(cartao);
    Tela::info("O identificador nao pode ser alterado; estado e instantes so mudam por Mover.");
    Tela::info("Informe os novos dados:");

    if (lerDadosEditaveis(cartao) != Tela::Leitura::LIDO) {
        return;
    }
    if (!this->cntrServicoCartao->atualizar(usuario, cartao)) {
        Tela::falha("Nao foi possivel atualizar.", {"cartao nao encontrado", "usuario nao e GESTOR"});
        return;
    }
    Tela::sucesso("Cartao atualizado com sucesso.");
}

void CntrApresentacaoCartao::excluir(const Email& usuario) {
    Tela::titulo("EXCLUIR CARTAO");
    Tela::info("Digite 0 para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do cartao", DICA_IDENTIFICADOR, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    bool confirmou = false;
    if (!Tela::confirmar("Confirma a exclusao do cartao " + identificador.getValor() + "? (S/N)", confirmou)) {
        return;
    }
    if (!confirmou) {
        Tela::aviso("Exclusao cancelada.");
        return;
    }
    if (!this->cntrServicoCartao->excluir(usuario, identificador)) {
        Tela::falha("Nao foi possivel excluir.", {"cartao nao encontrado", "usuario nao e GESTOR"});
        return;
    }
    Tela::sucesso("Cartao excluido com sucesso.");
}

void CntrApresentacaoCartao::listar(const Email& usuario) {
    Tela::titulo("LISTAR CARTOES DO QUADRO");
    Tela::info("Digite 0 para cancelar.");
    Tela::separador();

    Identificador quadro;
    if (Tela::lerCampo("Identificador do quadro", DICA_IDENTIFICADOR, quadro) != Tela::Leitura::LIDO) {
        return;
    }
    std::vector<Identificador> lista;
    if (!this->cntrServicoCartao->listar(usuario, quadro, lista)) {
        Tela::falha("Nao foi possivel listar.", {"quadro nao existe"});
        return;
    }
    if (lista.empty()) {
        Tela::info("O quadro nao tem cartoes.");
        return;
    }
    std::ostringstream cabecalho;
    cabecalho << "Cartoes do quadro " << quadro.getValor() << " (" << lista.size() << "):";
    Tela::info(cabecalho.str());
    Tela::separador();
    for (std::vector<Identificador>::size_type i = 0; i < lista.size(); i++) {
        Tela::info("  " + lista[i].getValor());
    }
    Tela::separador();
}

// --- Servicos 21 a 26 ---

void CntrApresentacaoCartao::mover(const Email& usuario) {
    Tela::titulo("MOVER CARTAO");
    Tela::info("Movimentos possiveis: A FAZER -> FAZENDO e FAZENDO -> FEITO. Digite 0 para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do cartao", DICA_IDENTIFICADOR, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    if (!this->cntrServicoCartao->mover(usuario, identificador)) {
        Tela::falha("Nao foi possivel mover o cartao.",
                    {"cartao nao encontrado", "cartao ja esta em FEITO", "limite de WIP do quadro atingido"});
        return;
    }
    CartaoAtividade cartao;
    this->cntrServicoCartao->ler(usuario, identificador, cartao);
    Tela::sucesso("Cartao movido para " + cartao.getEstado().getValor() + ".");
}

void CntrApresentacaoCartao::lerTempoCiclo(const Email& usuario) {
    Tela::titulo("TEMPO DE CICLO DO CARTAO");
    Tela::info("De FAZENDO ate FEITO. Digite 0 para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do cartao", DICA_IDENTIFICADOR, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    double minutos = 0;
    if (!this->cntrServicoCartao->lerTempoCiclo(usuario, identificador, minutos)) {
        Tela::falha("Nao foi possivel calcular.", {"cartao nao encontrado", "cartao ainda nao esta em FEITO"});
        return;
    }
    Tela::campo("Tempo de ciclo", formatarMinutos(minutos));
}

void CntrApresentacaoCartao::lerTempoCicloMedio(const Email& usuario) {
    Tela::titulo("TEMPO DE CICLO MEDIO DO QUADRO");
    Tela::info("Media dos cartoes em FEITO. Digite 0 para cancelar.");
    Tela::separador();

    Identificador quadro;
    if (Tela::lerCampo("Identificador do quadro", DICA_IDENTIFICADOR, quadro) != Tela::Leitura::LIDO) {
        return;
    }
    double minutos = 0;
    if (!this->cntrServicoCartao->lerTempoCicloMedio(usuario, quadro, minutos)) {
        Tela::falha("Nao foi possivel calcular.", {"quadro nao existe", "quadro sem cartao em FEITO"});
        return;
    }
    Tela::campo("Tempo de ciclo medio", formatarMinutos(minutos));
}

void CntrApresentacaoCartao::lerLeadTime(const Email& usuario) {
    Tela::titulo("LEAD TIME DO CARTAO");
    Tela::info("De A FAZER ate FEITO. Digite 0 para cancelar.");
    Tela::separador();

    Identificador identificador;
    if (Tela::lerCampo("Identificador do cartao", DICA_IDENTIFICADOR, identificador) != Tela::Leitura::LIDO) {
        return;
    }
    double minutos = 0;
    if (!this->cntrServicoCartao->lerLeadTime(usuario, identificador, minutos)) {
        Tela::falha("Nao foi possivel calcular.", {"cartao nao encontrado", "cartao ainda nao esta em FEITO"});
        return;
    }
    Tela::campo("Lead time", formatarMinutos(minutos));
}

void CntrApresentacaoCartao::lerLeadTimeMedio(const Email& usuario) {
    Tela::titulo("LEAD TIME MEDIO DO QUADRO");
    Tela::info("Media dos cartoes em FEITO. Digite 0 para cancelar.");
    Tela::separador();

    Identificador quadro;
    if (Tela::lerCampo("Identificador do quadro", DICA_IDENTIFICADOR, quadro) != Tela::Leitura::LIDO) {
        return;
    }
    double minutos = 0;
    if (!this->cntrServicoCartao->lerLeadTimeMedio(usuario, quadro, minutos)) {
        Tela::falha("Nao foi possivel calcular.", {"quadro nao existe", "quadro sem cartao em FEITO"});
        return;
    }
    Tela::campo("Lead time medio", formatarMinutos(minutos));
}

void CntrApresentacaoCartao::lerWIP(const Email& usuario) {
    Tela::titulo("WIP DO QUADRO");
    Tela::info("Cartoes na coluna FAZENDO. Digite 0 para cancelar.");
    Tela::separador();

    Identificador quadro;
    if (Tela::lerCampo("Identificador do quadro", DICA_IDENTIFICADOR, quadro) != Tela::Leitura::LIDO) {
        return;
    }
    int wip = 0;
    if (!this->cntrServicoCartao->lerWIP(usuario, quadro, wip)) {
        Tela::falha("Nao foi possivel ler o WIP.", {"quadro nao existe"});
        return;
    }
    std::ostringstream texto;
    texto << wip;
    Tela::campo("WIP", texto.str());
}
