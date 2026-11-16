// Testes do dominio Identificador: 3 letras (a-z, A-Z) seguidas de 3 digitos.
#include "identificador.hpp"
#include "teste.hpp"

int main() {
    TesteDominio<Identificador> identificador("ABC123");

    identificador.valido("PRJ001");
    identificador.valido("CRT001");
    identificador.valido("QDR999");
    identificador.valido("prj001", "minusculas");
    identificador.valido("pRj000", "maiusculas e minusculas");
    identificador.valido("AAA000");
    identificador.valido("zzz999");
    identificador.valido("aZa509");
    identificador.invalido("");
    identificador.invalido("P");
    identificador.invalido("PRJ01", "so 2 digitos");
    identificador.invalido("PR001", "so 2 letras");
    identificador.invalido("PRJ0001", "4 digitos");
    identificador.invalido("PRJA001", "4 letras");
    identificador.invalido("PRJ001 ", "espaco no fim");
    identificador.invalido(" PRJ001", "espaco no inicio");
    identificador.invalido("PRJ 01");
    identificador.invalido("001PRJ", "ordem trocada");
    identificador.invalido("P1J001", "digito entre as letras");
    identificador.invalido("PRJ0A1", "letra entre os digitos");
    identificador.invalido("PRJABC", "sem digitos");
    identificador.invalido("123456", "sem letras");
    identificador.invalido("PRJ-01");
    identificador.invalido("PR_001");
    identificador.invalido("@RJ001", "caractere antes de A");
    identificador.invalido("[RJ001", "caractere depois de Z");
    identificador.invalido("`RJ001", "caractere antes de a");
    identificador.invalido("{RJ001", "caractere depois de z");
    identificador.invalido("PRJ/01", "caractere antes de 0");
    identificador.invalido("PRJ:01", "caractere depois de 9");
    identificador.invalido(std::string("PRJ") + '\0' + "01", "byte nulo no meio");
    identificador.invalido("\xC3\xA9" "RJ001", "acento em UTF-8 (7 bytes)");
    identificador.invalido("\xE9" "RJ001", "acento em Latin-1");
    identificador.invalido("PRJ\xFF" "01", "byte 0xFF");

    return resultado();
}
