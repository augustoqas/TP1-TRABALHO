// Testes de CntrApresentacaoCartao::formatarMinutos, que mostra as metricas do quadro.
// Regra: arredonda uma vez para 1 casa decimal; o numero e a forma "d h min" saem desse
// mesmo valor, entao nunca discordam. "minuto" so quando o valor arredondado e 1.0.
#include <iostream>
#include <string>

#include "cntrapresentacaocartao.hpp"
#include "teste.hpp"

static void conferir(double minutos, const std::string& esperado, const std::string& descricao) {
    std::string obtido = CntrApresentacaoCartao::formatarMinutos(minutos);
    verificar(obtido == esperado, descricao + ": " + esperado);
    if (obtido != esperado) {
        std::cout << "      obtido: " << visivel(obtido) << std::endl;
    }
}

int main() {
    conferir(0, "0.0 minutos (0 d 0 h 0.0 min)", "zero");
    conferir(1, "1.0 minuto (0 d 0 h 1.0 min)", "singular");
    conferir(2, "2.0 minutos (0 d 0 h 2.0 min)", "plural");
    conferir(0.5, "0.5 minutos (0 d 0 h 0.5 min)", "meio minuto nao vira 1 min");
    conferir(2.0 / 3.0, "0.7 minutos (0 d 0 h 0.7 min)", "media com dizima: 1 casa decimal");
    conferir(0.04, "0.0 minutos (0 d 0 h 0.0 min)", "abaixo de 0.05 arredonda para 0.0");
    conferir(0.96, "1.0 minuto (0 d 0 h 1.0 min)", "arredonda para 1.0: singular");
    conferir(59.96, "60.0 minutos (0 d 1 h 0.0 min)", "arredondamento sobe para a hora");
    conferir(1530.5, "1530.5 minutos (1 d 1 h 30.5 min)", "dias, horas e minutos");
    conferir(1440, "1440.0 minutos (1 d 0 h 0.0 min)", "um dia exato");
    conferir(1000000, "1000000.0 minutos (694 d 10 h 40.0 min)", "valor grande sem notacao cientifica");

    return resultado();
}
