// Testes do dominio Papel: GESTOR ou DESENVOLVEDOR, em maiusculas.
#include "papel.hpp"
#include "teste.hpp"

int main() {
    TesteDominio<Papel> papel("GESTOR");

    papel.valido("GESTOR");
    papel.valido("DESENVOLVEDOR");
    papel.invalido("");
    papel.invalido("gestor", "minusculas");
    papel.invalido("Gestor", "so a inicial maiuscula");
    papel.invalido("desenvolvedor", "minusculas");
    papel.invalido("GESTORA");
    papel.invalido("GESTO");
    papel.invalido("DESENVOLVEDORA");
    papel.invalido(" GESTOR", "espaco no inicio");
    papel.invalido("GESTOR ", "espaco no fim");
    papel.invalido("DESENVOLVEDOR\n", "quebra de linha no fim");
    papel.invalido("ADMIN");
    papel.invalido("0", "o que a tela usa para cancelar");
    papel.invalido(std::string("GESTOR") + '\0', "byte nulo no fim");

    return resultado();
}
