#include "tela.hpp"

#include <iostream>
#include <sstream>

const std::string Tela::CANCELAR = "0";
const std::string Tela::LINHA_DUPLA = "==================================================";
const std::string Tela::LINHA_SIMPLES = "--------------------------------------------------";
const std::string Tela::MARGEM = "  ";

void Tela::titulo(const std::string& titulo) {
    std::cout << "\n" << LINHA_DUPLA << "\n";
    std::cout << MARGEM << titulo << "\n";
    std::cout << LINHA_DUPLA << "\n";
}

void Tela::separador() {
    std::cout << LINHA_SIMPLES << "\n";
}

void Tela::info(const std::string& texto) {
    std::cout << MARGEM << texto << "\n";
}

void Tela::campo(const std::string& rotulo, const std::string& valor) {
    std::cout << MARGEM << rotulo << ": " << valor << "\n";
}

// Mensagens de resultado comecam com linha em branco para chamar atencao; a
// linha em branco depois delas vem do titulo da proxima tela.
void Tela::sucesso(const std::string& texto) {
    std::cout << "\n" << MARGEM << "[OK] " << texto << "\n";
}

void Tela::falha(const std::string& texto, const std::vector<std::string>& motivos) {
    std::cout << "\n" << MARGEM << "[FALHA] " << texto << "\n";
    if (!motivos.empty()) {
        std::cout << MARGEM << "        Motivos possiveis:\n";
        for (std::vector<std::string>::size_type i = 0; i < motivos.size(); i++) {
            std::cout << MARGEM << "        - " << motivos[i] << "\n";
        }
    }
}

void Tela::erro(const std::string& texto) {
    std::cout << MARGEM << "[ERRO] " << texto << "\n";
}

void Tela::aviso(const std::string& texto) {
    std::cout << "\n" << MARGEM << "[AVISO] " << texto << "\n";
}

void Tela::cancelado() {
    aviso("Operacao cancelada. Nada foi alterado.");
}

bool Tela::lerLinha(const std::string& pergunta, std::string& linha) {
    std::cout << MARGEM << "> " << pergunta << std::endl;
    if (!std::getline(std::cin, linha)) {
        return false;
    }
    if (!linha.empty() && linha[linha.size() - 1] == '\r') {
        linha.erase(linha.size() - 1);
    }
    return true;
}

int Tela::converterOpcao(const std::string& texto) {
    if (texto.empty() || texto.size() > 2) {
        return -1;
    }
    int numero = 0;
    for (std::string::size_type i = 0; i < texto.size(); i++) {
        if (texto[i] < '0' || texto[i] > '9') {
            return -1;
        }
        numero = numero * 10 + (texto[i] - '0');
    }
    return numero;
}

bool Tela::converterInteiro(const std::string& texto, int& numero) {
    const std::string::size_type MAXIMO_DIGITOS = 9;  // cabe em int sem estouro
    std::string::size_type inicio = 0;
    bool negativo = false;
    if (!texto.empty() && texto[0] == '-') {
        negativo = true;
        inicio = 1;
    }
    std::string::size_type digitos = texto.size() - inicio;
    if (digitos == 0 || digitos > MAXIMO_DIGITOS) {
        return false;
    }
    int valor = 0;
    for (std::string::size_type i = inicio; i < texto.size(); i++) {
        if (texto[i] < '0' || texto[i] > '9') {
            return false;
        }
        valor = valor * 10 + (texto[i] - '0');
    }
    numero = negativo ? -valor : valor;
    return true;
}

int Tela::lerOpcao(const std::string& titulo, const std::vector<std::string>& opcoes) {
    const int quantidade = static_cast<int>(opcoes.size());
    std::ostringstream pergunta;
    pergunta << "Escolha uma opcao (1 a " << quantidade << "):";
    std::string texto;
    while (true) {
        Tela::titulo(titulo);
        for (int i = 0; i < quantidade; i++) {
            std::cout << MARGEM << "[" << (i + 1) << "] " << opcoes[i] << "\n";
        }
        separador();
        if (!lerLinha(pergunta.str(), texto)) {
            return 0;
        }
        int opcao = converterOpcao(texto);
        if (opcao >= 1 && opcao <= quantidade) {
            return opcao;
        }
        erro("Opcao invalida. Escolha um numero do menu.");
    }
}

bool Tela::confirmar(const std::string& pergunta, bool& resposta) {
    std::string texto;
    while (true) {
        if (!lerLinha(pergunta, texto)) {
            return false;
        }
        if (texto == "S" || texto == "s") {
            resposta = true;
            return true;
        }
        if (texto == "N" || texto == "n") {
            resposta = false;
            return true;
        }
        erro("Resposta invalida. Digite S (sim) ou N (nao).");
    }
}

bool Tela::entradaEncerrada() {
    return !std::cin;
}
