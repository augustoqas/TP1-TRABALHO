// Testes do dominio Tamanho: GRANDE, MEDIO ou PEQUENO, em maiusculas e sem acento.
#include "tamanho.hpp"
#include "teste.hpp"

int main() {
    TesteDominio<Tamanho> tamanho("MEDIO");

    tamanho.valido("GRANDE");
    tamanho.valido("MEDIO");
    tamanho.valido("PEQUENO");
    tamanho.invalido("");
    tamanho.invalido("grande", "minusculas");
    tamanho.invalido("Pequeno", "so a inicial maiuscula");
    tamanho.invalido("M\xC3\x89" "DIO", "com acento em UTF-8");
    tamanho.invalido("M\xC9" "DIO", "com acento em Latin-1");
    tamanho.invalido("MEDIA", "valor de Prioridade");
    tamanho.invalido("ENORME");
    tamanho.invalido(" GRANDE", "espaco no inicio");
    tamanho.invalido("PEQUENO ", "espaco no fim");
    tamanho.invalido("2", "numero da opcao no menu");

    return resultado();
}
