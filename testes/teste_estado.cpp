// Testes do dominio Estado: "A FAZER", "FAZENDO" ou "FEITO".
#include "estado.hpp"
#include "teste.hpp"

int main() {
    TesteDominio<Estado> estado("FEITO");

    estado.valido("A FAZER");
    estado.valido("FAZENDO");
    estado.valido("FEITO");
    estado.invalido("");
    estado.invalido("a fazer", "minusculas");
    estado.invalido("Fazendo", "so a inicial maiuscula");
    estado.invalido("AFAZER", "sem espaco");
    estado.invalido("A  FAZER", "dois espacos");
    estado.invalido("A_FAZER");
    estado.invalido(" FEITO", "espaco no inicio");
    estado.invalido("FEITO ", "espaco no fim");
    estado.invalido("A FAZER\n");
    estado.invalido("FAZER");
    estado.invalido("FEIT");
    estado.invalido("FEITOS");
    estado.invalido("PRONTO");
    estado.invalido("ALTA", "valor de outro dominio");
    estado.invalido(std::string("FEITO") + '\0', "byte nulo no fim");

    verificar(Estado::A_FAZER == "A FAZER" && Estado::FAZENDO == "FAZENDO" && Estado::FEITO == "FEITO",
              "constantes publicas");

    return resultado();
}
