// Testes do dominio Nome: ate 15 caracteres, letras e espacos simples entre palavras.
#include "nome.hpp"
#include "teste.hpp"

int main() {
    TesteDominio<Nome> nome("Anterior");

    nome.valido("Ana", "nome simples");
    nome.valido("Pedro", "nome simples maiuscula + minusculas");
    nome.valido("A", "um unico caractere maiusculo");
    nome.valido("z", "um unico caractere minusculo");
    nome.valido("mArIa", "maiusculas e minusculas misturadas");
    nome.valido("Joao Silva", "nome com um espaco interno");
    nome.valido("Ana Maria Souza", "15 caracteres com espacos (limite)");
    nome.valido("ABCDEFGHIJKLMNO", "15 letras (limite)");
    nome.valido("a B c D e F g H", "15 caracteres alternando letra/espaco");
    nome.valido("Z z", "letra espaco letra");
    nome.invalido("ABCDEFGHIJKLMNOP", "16 letras (excede limite)");
    nome.invalido("Ana Maria Souzas", "16 caracteres com espacos");
    nome.invalido("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "26 letras");
    nome.invalido("", "string vazia");
    nome.invalido(" ", "somente um espaco");
    nome.invalido("   ", "somente espacos");
    nome.invalido(" Ana", "espaco no inicio");
    nome.invalido("Ana ", "espaco no fim");
    nome.invalido("Ana  Maria", "dois espacos seguidos");
    nome.invalido("Ana1", "digito");
    nome.invalido("1", "somente digito");
    nome.invalido("Ana-Maria", "hifen");
    nome.invalido("D'Avila", "apostrofo");
    nome.invalido("Ana.", "ponto");
    nome.invalido("Ana_Maria", "sublinhado");
    nome.invalido("Ana\tMaria", "tab no meio");
    nome.invalido("\tAna", "tab no inicio");
    nome.invalido("Ana\nMaria", "quebra de linha LF");
    nome.invalido("Ana\r", "CR no fim");
    nome.invalido("Jo\xC3\xA3o", "acentuado em UTF-8 (Joao com til)");
    nome.invalido("Jos\xE9", "acentuado em Latin-1 (byte > 127)");
    nome.invalido(std::string("Ana") + '\0' + "Bia", "byte nulo embutido");
    nome.invalido("Ana[", "'[' (0x5B, entre Z e a)");
    nome.invalido("Ana`", "crase (0x60, entre Z e a)");
    nome.invalido("@na", "'@' (0x40, logo antes de A)");
    nome.invalido("Ana{", "'{' (0x7B, logo depois de z)");

    return resultado();
}
