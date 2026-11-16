// Testes do dominio Texto: ate 30 caracteres (letra, digito, espaco, , ; . : ? !),
// comeca com maiuscula, termina com ponto e pontuacao nao seguida de pontuacao.
#include "texto.hpp"
#include "teste.hpp"

int main() {
    TesteDominio<Texto> texto("Base.");

    texto.valido("A.", "menor texto possivel");
    texto.valido("Sistema de vendas.");
    texto.valido("Sistema de vendas, fase 1.");
    texto.valido("Fase 2: testes; ajustes.");
    texto.valido("Pronto? Sim! Entregue.");
    texto.valido("Versao 10 do app.");
    texto.valido("ABC.");
    texto.valido("A 1.", "digito no meio");
    texto.valido("A  b.", "dois espacos seguidos");
    texto.valido("A b .", "espaco antes do ponto");
    texto.valido("Abcdefghijklmnopqrstuvwxyz123.", "30 caracteres");
    texto.invalido("Abcdefghijklmnopqrstuvwxyz1234.", "31 caracteres");
    texto.invalido("");
    texto.invalido(".");
    texto.invalido("A", "sem ponto final");
    texto.invalido("abc.", "comeca com minuscula");
    texto.invalido("1abc.", "comeca com digito");
    texto.invalido(" Abc.", "comeca com espaco");
    texto.invalido(",Abc.", "comeca com pontuacao");
    texto.invalido("Abc", "termina sem ponto");
    texto.invalido("Abc!", "termina com outra pontuacao");
    texto.invalido("Abc. ", "espaco depois do ponto");
    texto.invalido("Abc..", "ponto seguido de ponto");
    texto.invalido("Abc,. ");
    texto.invalido("Abc,.", "virgula seguida de ponto");
    texto.invalido("Ab?! c.", "interrogacao seguida de exclamacao");
    texto.invalido("Ab:; c.");
    texto.invalido("Ab - c.", "hifen");
    texto.invalido("Ab_c.", "sublinhado");
    texto.invalido("Ab (c).", "parenteses");
    texto.invalido("Ab@c.");
    texto.invalido("Ab\"c.");
    texto.invalido("Ab'c.");
    texto.invalido("Ab\tc.", "tab");
    texto.invalido("Ab\nc.", "quebra de linha");
    texto.invalido(std::string("Ab") + '\0' + "c.", "byte nulo no meio");
    texto.invalido("Ac\xE9" "o.", "acento em Latin-1");
    texto.invalido("A\xC3\xA7" "ao.", "cedilha em UTF-8");
    texto.invalido("\xC3\x89" "ab.", "maiuscula acentuada em UTF-8");

    return resultado();
}
