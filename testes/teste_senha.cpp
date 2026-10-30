// Testes do dominio Senha: 5 caracteres, letras e digitos, ao menos uma letra e um digito.
#include "senha.hpp"
#include "teste.hpp"

int main() {
    TesteDominio<Senha> senha("Base1");

    senha.valido("Abc12");
    senha.valido("abc12");
    senha.valido("ABC12");
    senha.valido("1a2b3");
    senha.valido("a1111");
    senha.valido("1aaaa");
    senha.valido("0000a");
    senha.valido("Z9z9Z");
    senha.valido("aA0z9");
    senha.valido("zZ90A");
    senha.invalido("");
    senha.invalido("a");
    senha.invalido("a1");
    senha.invalido("abc1");
    senha.invalido("abc123");
    senha.invalido("a1b2c3d4e5");
    senha.invalido("abcde");
    senha.invalido("ABCDE");
    senha.invalido("aBcDe");
    senha.invalido("12345");
    senha.invalido("00000");
    senha.invalido("ab 12");
    senha.invalido(" ab12");
    senha.invalido("ab12 ");
    senha.invalido("     ");
    senha.invalido("ab\t12");
    senha.invalido("ab\n12");
    senha.invalido(std::string("ab") + '\0' + "12", "byte nulo no meio");
    senha.invalido("ab!12");
    senha.invalido("ab_12");
    senha.invalido("ab-12");
    senha.invalido("ab.12");
    senha.invalido("a/123");
    senha.invalido("a:123");
    senha.invalido("@a123");
    senha.invalido("[a123");
    senha.invalido("`a123");
    senha.invalido("{a123");
    senha.invalido("\xE9" "b123", "acento em Latin-1");
    senha.invalido("\xC3\xA9" "b12", "acento em UTF-8");
    senha.invalido("\xC3\xA7" "a12", "cedilha em UTF-8");
    senha.invalido("a\xFF\xFF\xFF" "1", "bytes 0xFF");
    senha.invalido("\xC9\xC7\xD5\xC3" "1", "maiusculas acentuadas em Latin-1");

    return resultado();
}
