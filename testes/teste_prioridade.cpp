// Testes do dominio Prioridade: ALTA, MEDIA ou BAIXA, em maiusculas e sem acento.
#include "prioridade.hpp"
#include "teste.hpp"

int main() {
    TesteDominio<Prioridade> prioridade("MEDIA");

    prioridade.valido("ALTA");
    prioridade.valido("MEDIA");
    prioridade.valido("BAIXA");
    prioridade.invalido("");
    prioridade.invalido("alta", "minusculas");
    prioridade.invalido("Media", "so a inicial maiuscula");
    prioridade.invalido("M\xC3\x89" "DIA", "com acento em UTF-8");
    prioridade.invalido("M\xC9" "DIA", "com acento em Latin-1");
    prioridade.invalido("URGENTE");
    prioridade.invalido("ALT");
    prioridade.invalido(" ALTA", "espaco no inicio");
    prioridade.invalido("BAIXA ", "espaco no fim");
    prioridade.invalido("1", "numero da opcao no menu");

    return resultado();
}
