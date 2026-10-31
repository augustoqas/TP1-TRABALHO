#include "cntrservicocartao.hpp"

#include <cstdio>
#include <ctime>
#include <string>

namespace {

const char* const SIGLAS_MESES[12] = {"JAN", "FEV", "MAR", "ABR", "MAI", "JUN",
                                      "JUL", "AGO", "SET", "OUT", "NOV", "DEZ"};

const std::string A_FAZER = "A FAZER";
const std::string FAZENDO = "FAZENDO";
const std::string FEITO = "FEITO";

bool bissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0;
}

int diasNoMes(int mes, int ano) {
    static const int DIAS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (mes == 2 && bissexto(ano)) ? 29 : DIAS[mes - 1];
}

}  // namespace

// --- Tempo ---

long CntrServicoCartao::converterMinutos(const Timestamp& instante) {
    // O dominio ja garante o formato DIA-MES-ANO-HH:MM (dia com 1 ou 2 digitos).
    std::string valor = instante.getValor();
    if (valor.empty()) {
        return -1;
    }
    std::string::size_type p1 = valor.find('-');
    std::string::size_type p2 = valor.find('-', p1 + 1);
    std::string::size_type p3 = valor.find('-', p2 + 1);

    int dia = std::stoi(valor.substr(0, p1));
    std::string sigla = valor.substr(p1 + 1, p2 - p1 - 1);
    int ano = std::stoi(valor.substr(p2 + 1, p3 - p2 - 1));
    int hora = std::stoi(valor.substr(p3 + 1, 2));
    int minuto = std::stoi(valor.substr(p3 + 4, 2));

    int mes = 1;
    while (mes <= 12 && sigla != SIGLAS_MESES[mes - 1]) {
        mes++;
    }

    long dias = 0;
    for (int a = 2000; a < ano; a++) {
        dias += bissexto(a) ? 366 : 365;
    }
    for (int m = 1; m < mes; m++) {
        dias += diasNoMes(m, ano);
    }
    dias += dia - 1;
    return dias * 24 * 60 + hora * 60 + minuto;
}

Timestamp CntrServicoCartao::agora() const {
    if (this->relogio != nullptr) {
        return this->relogio();
    }
    std::time_t segundos = std::time(nullptr);
    std::tm* local = std::localtime(&segundos);
    char texto[32];
    std::snprintf(texto, sizeof(texto), "%d-%s-%d-%02d:%02d", local->tm_mday, SIGLAS_MESES[local->tm_mon],
                  local->tm_year + 1900, local->tm_hour, local->tm_min);
    Timestamp instante;
    instante.setValor(texto);
    return instante;
}

// --- Auxiliares ---

bool CntrServicoCartao::usuarioCadastrado(const Email& usuario) const {
    if (this->containerPessoa == nullptr) {
        return false;
    }
    Pessoa pessoa;
    return this->containerPessoa->pesquisar(usuario, pessoa);
}

bool CntrServicoCartao::temPapel(const Email& email, const std::string& papel) const {
    if (this->containerPessoa == nullptr) {
        return false;
    }
    Pessoa pessoa;
    if (!this->containerPessoa->pesquisar(email, pessoa)) {
        return false;
    }
    return pessoa.getPapel().getValor() == papel;
}

bool CntrServicoCartao::lerQuadro(const Email& usuario, const Identificador& identificador, Quadro& quadro) {
    if (this->cntrServicoQuadro == nullptr) {
        return false;
    }
    return this->cntrServicoQuadro->ler(usuario, identificador, quadro);
}

int CntrServicoCartao::contarFazendo(const Identificador& quadro) const {
    std::vector<CartaoAtividade> cartoes;
    this->containerCartao->listar(quadro, cartoes);
    int quantidade = 0;
    for (std::vector<CartaoAtividade>::size_type i = 0; i < cartoes.size(); i++) {
        if (cartoes[i].getEstado().getValor() == FAZENDO) {
            quantidade++;
        }
    }
    return quantidade;
}

bool CntrServicoCartao::cartaoCompleto(const CartaoAtividade& cartao) {
    return !cartao.getIdentificador().getValor().empty() && !cartao.getNome().getValor().empty() &&
           !cartao.getDescricao().getValor().empty() && !cartao.getPrioridade().getValor().empty() &&
           !cartao.getTamanho().getValor().empty();
}

// --- Servicos 16 a 20 ---

bool CntrServicoCartao::criar(const Email& usuario, const Identificador& quadro, const Email& desenvolvedor,
                              const CartaoAtividade& cartao) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    if (!temPapel(usuario, "GESTOR")) {
        return false;
    }
    if (!temPapel(desenvolvedor, "DESENVOLVEDOR")) {
        return false;
    }
    if (!cartaoCompleto(cartao)) {
        return false;
    }
    Quadro lido;
    if (!lerQuadro(usuario, quadro, lido)) {
        return false;
    }

    // Estado e instantes vindos em cartao sao ignorados: todo cartao novo entra em A FAZER agora.
    CartaoAtividade novo;
    novo.setIdentificador(cartao.getIdentificador());
    novo.setNome(cartao.getNome());
    novo.setDescricao(cartao.getDescricao());
    novo.setPrioridade(cartao.getPrioridade());
    novo.setTamanho(cartao.getTamanho());
    Estado estado;
    estado.setValor(A_FAZER);
    novo.setEstado(estado);
    novo.setEntrada(agora());

    // O conteiner ja recusa identificador repetido.
    return this->containerCartao->incluir(novo, quadro, desenvolvedor);
}

bool CntrServicoCartao::ler(const Email& usuario, const Identificador& identificador, CartaoAtividade& cartao) {
    if (this->containerCartao == nullptr || !usuarioCadastrado(usuario)) {
        return false;
    }
    return this->containerCartao->pesquisar(identificador, cartao);
}

bool CntrServicoCartao::atualizar(const Email& usuario, const CartaoAtividade& cartao) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    if (!temPapel(usuario, "GESTOR")) {
        return false;
    }
    if (!cartaoCompleto(cartao)) {
        return false;
    }
    CartaoAtividade atual;
    if (!this->containerCartao->pesquisar(cartao.getIdentificador(), atual)) {
        return false;
    }
    // Chave, estado e instantes ficam como estao; so mudam os dados editaveis.
    atual.setNome(cartao.getNome());
    atual.setDescricao(cartao.getDescricao());
    atual.setPrioridade(cartao.getPrioridade());
    atual.setTamanho(cartao.getTamanho());
    return this->containerCartao->atualizar(atual);
}

bool CntrServicoCartao::excluir(const Email& usuario, const Identificador& identificador) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    if (!temPapel(usuario, "GESTOR")) {
        return false;
    }
    // O conteiner ja recusa cartao inexistente.
    return this->containerCartao->remover(identificador);
}

bool CntrServicoCartao::listar(const Email& usuario, const Identificador& quadro, std::vector<Identificador>& lista) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    Quadro lido;
    if (!lerQuadro(usuario, quadro, lido)) {
        return false;
    }
    std::vector<CartaoAtividade> cartoes;
    this->containerCartao->listar(quadro, cartoes);
    lista.clear();
    for (std::vector<CartaoAtividade>::size_type i = 0; i < cartoes.size(); i++) {
        lista.push_back(cartoes[i].getIdentificador());
    }
    return true;
}

// --- Servicos 21 a 26 ---

bool CntrServicoCartao::mover(const Email& usuario, const Identificador& identificador) {
    if (this->containerCartao == nullptr || !usuarioCadastrado(usuario)) {
        return false;
    }
    CartaoAtividade cartao;
    if (!this->containerCartao->pesquisar(identificador, cartao)) {
        return false;
    }

    std::string estadoAtual = cartao.getEstado().getValor();
    Estado novoEstado;
    if (estadoAtual == A_FAZER) {
        // Entrar em FAZENDO nao pode estourar o limite de WIP do quadro.
        Identificador idQuadro;
        Quadro quadro;
        if (!this->containerCartao->getQuadro(identificador, idQuadro) || !lerQuadro(usuario, idQuadro, quadro)) {
            return false;
        }
        if (contarFazendo(idQuadro) >= quadro.getLimite().getValor()) {
            return false;
        }
        novoEstado.setValor(FAZENDO);
        cartao.setInicio(agora());
    } else if (estadoAtual == FAZENDO) {
        novoEstado.setValor(FEITO);
        cartao.setTermino(agora());
    } else {
        return false;  // Ja esta em FEITO.
    }
    cartao.setEstado(novoEstado);
    return this->containerCartao->atualizar(cartao);
}

bool CntrServicoCartao::lerTempoCiclo(const Email& usuario, const Identificador& identificador, double& minutos) {
    if (this->containerCartao == nullptr || !usuarioCadastrado(usuario)) {
        return false;
    }
    CartaoAtividade cartao;
    if (!this->containerCartao->pesquisar(identificador, cartao) || cartao.getEstado().getValor() != FEITO) {
        return false;
    }
    minutos = static_cast<double>(converterMinutos(cartao.getTermino()) - converterMinutos(cartao.getInicio()));
    return true;
}

bool CntrServicoCartao::lerLeadTime(const Email& usuario, const Identificador& identificador, double& minutos) {
    if (this->containerCartao == nullptr || !usuarioCadastrado(usuario)) {
        return false;
    }
    CartaoAtividade cartao;
    if (!this->containerCartao->pesquisar(identificador, cartao) || cartao.getEstado().getValor() != FEITO) {
        return false;
    }
    minutos = static_cast<double>(converterMinutos(cartao.getTermino()) - converterMinutos(cartao.getEntrada()));
    return true;
}

bool CntrServicoCartao::calcularMedia(const Email& usuario, const Identificador& quadro, bool ciclo, double& minutos) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    Quadro lido;
    if (!lerQuadro(usuario, quadro, lido)) {
        return false;
    }
    std::vector<CartaoAtividade> cartoes;
    this->containerCartao->listar(quadro, cartoes);

    long soma = 0;
    int concluidos = 0;
    for (std::vector<CartaoAtividade>::size_type i = 0; i < cartoes.size(); i++) {
        if (cartoes[i].getEstado().getValor() != FEITO) {
            continue;
        }
        long comeco = converterMinutos(ciclo ? cartoes[i].getInicio() : cartoes[i].getEntrada());
        soma += converterMinutos(cartoes[i].getTermino()) - comeco;
        concluidos++;
    }
    if (concluidos == 0) {
        return false;
    }
    minutos = static_cast<double>(soma) / concluidos;
    return true;
}

bool CntrServicoCartao::lerTempoCicloMedio(const Email& usuario, const Identificador& quadro, double& minutos) {
    return calcularMedia(usuario, quadro, true, minutos);
}

bool CntrServicoCartao::lerLeadTimeMedio(const Email& usuario, const Identificador& quadro, double& minutos) {
    return calcularMedia(usuario, quadro, false, minutos);
}

bool CntrServicoCartao::lerWIP(const Email& usuario, const Identificador& quadro, int& wip) {
    if (this->containerCartao == nullptr) {
        return false;
    }
    Quadro lido;
    if (!lerQuadro(usuario, quadro, lido)) {
        return false;
    }
    wip = contarFazendo(quadro);
    return true;
}
