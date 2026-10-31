// Testes do dominio Limite: inteiro de 1 a 25.
#include "limite.hpp"
#include "teste.hpp"

int main() {
    TesteDominio<Limite, int> limite(10);

    limite.valido(1, "minimo");
    limite.valido(2);
    limite.valido(5);
    limite.valido(13);
    limite.valido(24);
    limite.valido(25, "maximo");
    limite.invalido(0, "abaixo do minimo");
    limite.invalido(-1, "negativo");
    limite.invalido(-25, "negativo simetrico ao maximo");
    limite.invalido(26, "acima do maximo");
    limite.invalido(100);
    limite.invalido(2147483647, "maior int");
    limite.invalido(-2147483647 - 1, "menor int");

    // Antes de qualquer setValor, getValor devolve 0 (nao definido).
    Limite novo;
    verificar(novo.getValor() == 0, "limite novo comeca em 0 (nao definido)");

    return resultado();
}
