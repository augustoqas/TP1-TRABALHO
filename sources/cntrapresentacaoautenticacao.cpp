#include "cntrapresentacaoautenticacao.hpp"

#include "tela.hpp"

const std::string CntrApresentacaoAutenticacao::DICA_EMAIL = "formato nome@dominio, letras minusculas";
const std::string CntrApresentacaoAutenticacao::DICA_SENHA = "5 letras ou digitos";

bool CntrApresentacaoAutenticacao::executar(Email& email) {
    Tela::titulo("ENTRAR");
    if (this->cntrServicoAutenticacao == nullptr) {
        Tela::aviso("Servico de autenticacao indisponivel.");
        return false;
    }
    Tela::info("Digite 0 em qualquer campo para cancelar.");
    Tela::separador();

    Email emailLido;
    if (Tela::lerCampo("Email", DICA_EMAIL, emailLido) != Tela::Leitura::LIDO) {
        return false;
    }
    Senha senha;
    if (Tela::lerCampo("Senha", DICA_SENHA, senha) != Tela::Leitura::LIDO) {
        return false;
    }

    if (!this->cntrServicoAutenticacao->autenticar(emailLido, senha)) {
        Tela::falha("Email ou senha incorretos.");
        return false;
    }
    email = emailLido;
    Tela::sucesso("Autenticado com sucesso. Bem-vindo(a), " + emailLido.getValor() + "!");
    return true;
}
