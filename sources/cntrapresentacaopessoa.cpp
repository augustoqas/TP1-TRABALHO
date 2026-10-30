#include "cntrapresentacaopessoa.hpp"

#include <sstream>
#include <stdexcept>
#include <vector>

#include "excecaosessaoencerrada.hpp"

const std::string CntrApresentacaoPessoa::DICA_EMAIL = "formato nome@dominio, letras minusculas";
const std::string CntrApresentacaoPessoa::DICA_NOME = "1 a 15 letras, espaco simples entre palavras";
const std::string CntrApresentacaoPessoa::DICA_SENHA = "5 letras ou digitos, com ao menos 1 letra e 1 digito";
const std::string CntrApresentacaoPessoa::DICA_PAPEL = "GESTOR ou DESENVOLVEDOR, em maiusculas";

// --- Leitura de campos ---

// PROVISORIO (issue #9): Papel::setValor ainda nao lanca excecao em todas as
// versoes, entao o texto e conferido antes. Quando lancar, usar Tela::lerCampo.
Tela::Leitura CntrApresentacaoPessoa::lerPapel(Papel& papel) {
    std::string texto;
    while (true) {
        if (!Tela::lerLinha("Papel (" + DICA_PAPEL + "):", texto)) {
            return Tela::Leitura::FIM;
        }
        if (texto == Tela::CANCELAR) {
            Tela::cancelado();
            return Tela::Leitura::CANCELADO;
        }
        if (texto != "GESTOR" && texto != "DESENVOLVEDOR") {
            Tela::erro("Papel invalido: digite GESTOR ou DESENVOLVEDOR. Tente novamente.");
            continue;
        }
        try {
            papel.setValor(texto);
            return Tela::Leitura::LIDO;
        } catch (const std::invalid_argument& excecao) {
            Tela::erro(std::string(excecao.what()) + " Tente novamente.");
        }
    }
}

Tela::Leitura CntrApresentacaoPessoa::lerDadosAlteraveis(Pessoa& pessoa) {
    Nome nome;
    Tela::Leitura leitura = Tela::lerCampo("Nome", DICA_NOME, nome);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    Senha senha;
    leitura = Tela::lerCampo("Senha", DICA_SENHA, senha);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    Papel papel;
    leitura = lerPapel(papel);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    pessoa.setNome(nome);
    pessoa.setSenha(senha);
    pessoa.setPapel(papel);
    return Tela::Leitura::LIDO;
}

Tela::Leitura CntrApresentacaoPessoa::lerNovaPessoa(Pessoa& pessoa) {
    Email email;
    Tela::Leitura leitura = Tela::lerCampo("Email", DICA_EMAIL, email);
    if (leitura != Tela::Leitura::LIDO) {
        return leitura;
    }
    pessoa.setEmail(email);
    return lerDadosAlteraveis(pessoa);
}

void CntrApresentacaoPessoa::mostrarPessoa(const Pessoa& pessoa) {
    Papel papel = pessoa.getPapel();  // copia porque getValor de Papel nao e const
    Tela::separador();
    Tela::campo("Email", pessoa.getEmail().getValor());
    Tela::campo("Nome", pessoa.getNome().getValor());
    Tela::campo("Papel", papel.getValor());
    Tela::separador();
}

// --- Criar conta / criar pessoa ---

bool CntrApresentacaoPessoa::formularioCriacao(const std::string& titulo, const std::string& mensagemSucesso,
                                               const std::string& mensagemFalha) {
    Tela::titulo(titulo);
    if (this->cntrServicoPessoa == nullptr) {
        Tela::aviso("Servico de pessoas indisponivel.");
        return false;
    }
    Tela::info("Preencha os campos abaixo. Digite 0 em qualquer campo para cancelar.");
    Tela::separador();

    Pessoa pessoa;
    if (lerNovaPessoa(pessoa) != Tela::Leitura::LIDO) {
        return false;
    }
    if (!this->cntrServicoPessoa->criar(pessoa)) {
        Tela::falha(mensagemFalha, {"email ja cadastrado"});
        return false;
    }
    Tela::sucesso(mensagemSucesso);
    return true;
}

bool CntrApresentacaoPessoa::cadastrar() {
    return formularioCriacao("CRIAR CONTA", "Conta criada com sucesso. Use a opcao Entrar para acessar.",
                             "Falha ao criar conta.");
}

// --- Menu de pessoas ---

void CntrApresentacaoPessoa::executar(const Email& usuario) {
    const int CRIAR = 1;
    const int LER = 2;
    const int ATUALIZAR = 3;
    const int EXCLUIR = 4;
    const int LISTAR = 5;
    const int VOLTAR = 6;

    if (this->cntrServicoPessoa == nullptr) {
        Tela::aviso("Servico de pessoas indisponivel.");
        return;
    }
    while (true) {
        int opcao = Tela::lerOpcao("MENU DE PESSOAS  (usuario: " + usuario.getValor() + ")",
                                   {"Criar pessoa", "Ler pessoa", "Atualizar pessoa",
                                    "Excluir pessoa", "Listar pessoas", "Voltar"});
        if (opcao == 0 || opcao == VOLTAR) {
            return;
        }
        if (opcao == CRIAR) {
            formularioCriacao("CRIAR PESSOA", "Pessoa criada com sucesso.", "Falha ao criar pessoa.");
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

void CntrApresentacaoPessoa::ler(const Email& usuario) {
    Tela::titulo("LER PESSOA");
    Tela::info("Digite 0 para cancelar.");
    Tela::separador();

    Email email;
    if (Tela::lerCampo("Email da pessoa", DICA_EMAIL, email) != Tela::Leitura::LIDO) {
        return;
    }
    Pessoa pessoa;
    if (!this->cntrServicoPessoa->ler(usuario, email, pessoa)) {
        Tela::falha("Pessoa nao encontrada.");
        return;
    }
    Tela::info("Dados da pessoa:");
    mostrarPessoa(pessoa);
}

void CntrApresentacaoPessoa::atualizar(const Email& usuario) {
    Tela::titulo("ATUALIZAR PESSOA");
    Tela::info("Digite 0 em qualquer campo para cancelar.");
    Tela::separador();

    Email email;
    if (Tela::lerCampo("Email da pessoa", DICA_EMAIL, email) != Tela::Leitura::LIDO) {
        return;
    }
    Pessoa pessoa;
    if (!this->cntrServicoPessoa->ler(usuario, email, pessoa)) {
        Tela::falha("Pessoa nao encontrada.");
        return;
    }
    Tela::info("Dados atuais:");
    mostrarPessoa(pessoa);
    Tela::info("O email nao pode ser alterado. Informe os novos dados:");

    if (lerDadosAlteraveis(pessoa) != Tela::Leitura::LIDO) {
        return;
    }
    if (!this->cntrServicoPessoa->atualizar(usuario, pessoa)) {
        Tela::falha("Nao foi possivel atualizar.",
                    {"pessoa nao encontrada",
                     "gestor de projeto so pode ter papel GESTOR",
                     "desenvolvedor de cartao so pode ter papel DESENVOLVEDOR"});
        return;
    }
    Tela::sucesso("Pessoa atualizada com sucesso.");
}

void CntrApresentacaoPessoa::excluir(const Email& usuario) {
    Tela::titulo("EXCLUIR PESSOA");
    Tela::info("Digite 0 para cancelar.");
    Tela::separador();

    Email email;
    if (Tela::lerCampo("Email da pessoa", DICA_EMAIL, email) != Tela::Leitura::LIDO) {
        return;
    }
    bool confirmou = false;
    if (!Tela::confirmar("Confirma a exclusao de " + email.getValor() + "? (S/N)", confirmou)) {
        return;
    }
    if (!confirmou) {
        Tela::aviso("Exclusao cancelada.");
        return;
    }
    if (!this->cntrServicoPessoa->excluir(usuario, email)) {
        Tela::falha("Nao foi possivel excluir.",
                    {"pessoa nao encontrada",
                     "pessoa vinculada a projeto ou cartao (gestor de projeto ou desenvolvedor de cartao)"});
        return;
    }
    Tela::sucesso("Pessoa excluida com sucesso.");
    if (email.getValor() == usuario.getValor()) {
        Tela::aviso("Sua conta foi excluida. Sessao encerrada.");
        throw ExcecaoSessaoEncerrada();
    }
}

void CntrApresentacaoPessoa::listar(const Email& usuario) {
    Tela::titulo("LISTAR PESSOAS");
    std::vector<Email> lista;
    if (!this->cntrServicoPessoa->listar(usuario, lista)) {
        Tela::falha("Nao foi possivel listar.", {"usuario da sessao nao esta mais cadastrado"});
        return;
    }
    if (lista.empty()) {
        Tela::info("Nenhuma pessoa cadastrada.");
        return;
    }
    std::ostringstream cabecalho;
    cabecalho << "Pessoas cadastradas (" << lista.size() << "):";
    Tela::info(cabecalho.str());
    Tela::separador();
    for (std::vector<Email>::size_type i = 0; i < lista.size(); i++) {
        Tela::info("  " + lista[i].getValor());
    }
    Tela::separador();
}
