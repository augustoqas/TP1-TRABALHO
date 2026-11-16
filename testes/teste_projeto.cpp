// Testes da entidade Projeto: guarda copias dos dominios e funciona via const Projeto&.
#include "projeto.hpp"
#include "teste.hpp"

// Usa a entidade somente por referencia constante (como a camada de servico).
static std::string resumo(const Projeto& projeto) {
    return projeto.getIdentificador().getValor() + "|" + projeto.getNome().getValor() + "|" +
           projeto.getDescricao().getValor() + "|" + projeto.getInicio().getValor() + "|" +
           projeto.getTermino().getValor();
}

// Simula parametro de saida (Projeto&), como em IServicoProjeto::ler.
static void preencher(Projeto& saida, const Projeto& origem) {
    saida = origem;
}

int main() {
    Projeto projeto;

    Identificador identificador;
    identificador.setValor("PRJ001");
    Nome nome;
    nome.setValor("Sistema Kanban");
    Texto descricao;
    descricao.setValor("Trabalho de TP1.");
    Timestamp inicio;
    inicio.setValor("01-MAR-2026-08:00");
    Timestamp termino;
    termino.setValor("30-JUN-2026-18:00");

    projeto.setIdentificador(identificador);
    projeto.setNome(nome);
    projeto.setDescricao(descricao);
    projeto.setInicio(inicio);
    projeto.setTermino(termino);

    verificar(projeto.getIdentificador().getValor() == "PRJ001", "getIdentificador retorna o identificador armazenado");
    verificar(projeto.getNome().getValor() == "Sistema Kanban", "getNome retorna o nome armazenado");
    verificar(projeto.getDescricao().getValor() == "Trabalho de TP1.", "getDescricao retorna a descricao armazenada");
    verificar(projeto.getInicio().getValor() == "01-MAR-2026-08:00", "getInicio retorna o inicio armazenado");
    verificar(projeto.getTermino().getValor() == "30-JUN-2026-18:00", "getTermino retorna o termino armazenado");
    verificar(projeto.getTermino().getMinutos() > projeto.getInicio().getMinutos(), "timestamps guardados mantem os minutos");

    nome.setValor("Outro");
    descricao.setValor("Outra.");
    verificar(projeto.getNome().getValor() == "Sistema Kanban", "entidade guarda copia do nome");
    verificar(projeto.getDescricao().getValor() == "Trabalho de TP1.", "entidade guarda copia da descricao");

    Projeto copia(projeto);
    verificar(resumo(copia) == resumo(projeto), "construtor de copia preserva os atributos");

    Projeto atribuido;
    atribuido = projeto;
    verificar(resumo(atribuido) == resumo(projeto), "atribuicao preserva os atributos");

    Timestamp outroTermino;
    outroTermino.setValor("31-DEZ-2026-23:59");
    copia.setTermino(outroTermino);
    verificar(copia.getTermino().getValor() == "31-DEZ-2026-23:59", "copia pode ser alterada");
    verificar(projeto.getTermino().getValor() == "30-JUN-2026-18:00", "alterar a copia nao altera o original");

    Projeto saida;
    preencher(saida, projeto);
    verificar(resumo(saida) == resumo(projeto), "uso via const Projeto& e Projeto& de saida");

    return resultado();
}
