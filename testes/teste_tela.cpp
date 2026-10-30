// Testes de Tela::lerCampo / Tela::lerCampoInteiro com dominios REAIS: Nome (lanca
// excecao), Papel/Prioridade (bool setValor(string)) e Limite (bool setValor(int)).
// A entrada vem de uma string; o que a tela escreve e descartado.
#include <iostream>
#include <sstream>
#include <string>

#include "limite.hpp"
#include "nome.hpp"
#include "papel.hpp"
#include "prioridade.hpp"
#include "tela.hpp"
#include "teste.hpp"

static std::streambuf* saidaReal = std::cout.rdbuf();
static std::ostringstream descarte;

// A partir daqui std::cin le "fonte" e std::cout escreve no descarte.
static void entrada(std::istringstream& fonte) {
    std::cin.clear();
    std::cin.rdbuf(fonte.rdbuf());
    std::cout.rdbuf(descarte.rdbuf());
}

static void conferir(bool condicao, const std::string& descricao) {
    std::cout.rdbuf(saidaReal);
    verificar(condicao, descricao);
}

int main() {
    std::streambuf* entradaReal = std::cin.rdbuf();
    {
        std::istringstream fonte("Ana1\nAna Maria\n");
        entrada(fonte);
        Nome nome;
        Tela::Leitura r = Tela::lerCampo("Nome", "dica", nome);
        conferir(r == Tela::Leitura::LIDO && nome.getValor() == "Ana Maria", "Nome: excecao -> pede de novo");
    }
    {
        std::istringstream fonte("URGENTE\nALTA\n");
        entrada(fonte);
        Prioridade prioridade;
        Tela::Leitura r = Tela::lerCampo("Prioridade", "dica", prioridade);
        conferir(r == Tela::Leitura::LIDO && prioridade.getValor() == "ALTA",
                 "Prioridade (bool setValor): false -> pede de novo");
    }
    {
        std::istringstream fonte("gestor\nGESTOR\n");
        entrada(fonte);
        Papel papel;
        Tela::Leitura r = Tela::lerCampo("Papel", "dica", papel);
        conferir(r == Tela::Leitura::LIDO && papel.getValor() == "GESTOR",
                 "Papel (bool setValor): false -> pede de novo");
    }
    {
        std::istringstream fonte("abc\n30\n5\n");
        entrada(fonte);
        Limite limite;
        Tela::Leitura r = Tela::lerCampoInteiro("Limite", "dica", limite);
        conferir(r == Tela::Leitura::LIDO && limite.getValor() == 5,
                 "Limite (bool setValor(int)): texto e fora da faixa -> pede de novo");
    }
    {
        std::istringstream fonte("0\n");
        entrada(fonte);
        Limite limite;
        conferir(Tela::lerCampoInteiro("Limite", "dica", limite) == Tela::Leitura::CANCELADO,
                 "lerCampoInteiro: 0 cancela");
    }
    {
        std::istringstream fonte("0\n");
        entrada(fonte);
        Nome nome;
        conferir(Tela::lerCampo("Nome", "dica", nome) == Tela::Leitura::CANCELADO, "lerCampo: 0 cancela");
    }
    {
        std::istringstream fonte("Ana1\n");
        entrada(fonte);
        Nome nome;
        conferir(Tela::lerCampo("Nome", "dica", nome) == Tela::Leitura::FIM, "lerCampo: EOF -> FIM");
    }
    std::cin.rdbuf(entradaReal);
    return resultado();
}
