// Testes da entidade CartaoAtividade: cada set guarda o dominio e o get devolve o mesmo valor.
#include "cartaoatividade.hpp"
#include "teste.hpp"

int main() {
    CartaoAtividade cartao;

    // Cartao novo: todos os dominios vazios (nada preenchido).
    verificar(cartao.getIdentificador().getValor().empty(), "cartao novo: identificador vazio");
    verificar(cartao.getInicio().getValor().empty(), "cartao novo: inicio vazio");
    verificar(cartao.getTermino().getValor().empty(), "cartao novo: termino vazio");

    Identificador identificador;
    identificador.setValor("CRT001");
    cartao.setIdentificador(identificador);
    verificar(cartao.getIdentificador().getValor() == "CRT001", "identificador");

    Nome nome;
    nome.setValor("Tela de login");
    cartao.setNome(nome);
    verificar(cartao.getNome().getValor() == "Tela de login", "nome");

    Texto descricao;
    descricao.setValor("Implementar autenticacao.");
    cartao.setDescricao(descricao);
    verificar(cartao.getDescricao().getValor() == "Implementar autenticacao.", "descricao");

    Prioridade prioridade;
    prioridade.setValor("ALTA");
    cartao.setPrioridade(prioridade);
    verificar(cartao.getPrioridade().getValor() == "ALTA", "prioridade");

    Estado estado;
    estado.setValor("A FAZER");
    cartao.setEstado(estado);
    verificar(cartao.getEstado().getValor() == "A FAZER", "estado");

    Tamanho tamanho;
    tamanho.setValor("MEDIO");
    cartao.setTamanho(tamanho);
    verificar(cartao.getTamanho().getValor() == "MEDIO", "tamanho");

    Timestamp entrada;
    entrada.setValor("20-OUT-2026-09:00");
    cartao.setEntrada(entrada);
    verificar(cartao.getEntrada().getValor() == "20-OUT-2026-09:00", "entrada");

    Timestamp inicio;
    inicio.setValor("21-OUT-2026-10:30");
    cartao.setInicio(inicio);
    verificar(cartao.getInicio().getValor() == "21-OUT-2026-10:30", "inicio");

    Timestamp termino;
    termino.setValor("22-OUT-2026-17:45");
    cartao.setTermino(termino);
    verificar(cartao.getTermino().getValor() == "22-OUT-2026-17:45", "termino");

    // A copia e independente do original (o conteiner guarda copias).
    CartaoAtividade copia = cartao;
    estado.setValor("FAZENDO");
    cartao.setEstado(estado);
    verificar(copia.getEstado().getValor() == "A FAZER", "copia nao muda quando o original muda");

    return resultado();
}
