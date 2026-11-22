// Testes da entidade Quadro: guarda copias dos dominios e funciona via const Quadro&.
#include <sstream>

#include "quadro.hpp"
#include "teste.hpp"

// Usa a entidade somente por referencia constante (como a camada de servico).
static std::string resumo(const Quadro& quadro) {
    std::ostringstream texto;
    texto << quadro.getIdentificador().getValor() << "|" << quadro.getNome().getValor() << "|"
          << quadro.getLimite().getValor();
    return texto.str();
}

// Simula parametro de saida (Quadro&), como em IServicoQuadro::ler.
static void preencher(Quadro& saida, const Quadro& origem) {
    saida = origem;
}

int main() {
    Quadro quadro;

    Identificador identificador;
    identificador.setValor("QDR001");
    Nome nome;
    nome.setValor("Sprint um");
    Limite limite;
    limite.setValor(5);

    quadro.setIdentificador(identificador);
    quadro.setNome(nome);
    quadro.setLimite(limite);

    verificar(quadro.getIdentificador().getValor() == "QDR001", "getIdentificador retorna o identificador armazenado");
    verificar(quadro.getNome().getValor() == "Sprint um", "getNome retorna o nome armazenado");
    verificar(quadro.getLimite().getValor() == 5, "getLimite retorna o limite armazenado");

    limite.setValor(10);
    verificar(quadro.getLimite().getValor() == 5, "entidade guarda copia do limite");

    Quadro copia(quadro);
    verificar(resumo(copia) == resumo(quadro), "construtor de copia preserva os atributos");

    Quadro atribuido;
    atribuido = quadro;
    verificar(resumo(atribuido) == resumo(quadro), "atribuicao preserva os atributos");

    copia.setLimite(limite);
    verificar(copia.getLimite().getValor() == 10, "copia pode ser alterada");
    verificar(quadro.getLimite().getValor() == 5, "alterar a copia nao altera o original");

    Quadro saida;
    preencher(saida, quadro);
    verificar(resumo(saida) == "QDR001|Sprint um|5", "uso via const Quadro& e Quadro& de saida");

    return resultado();
}
