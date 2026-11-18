#ifndef CARTAOATIVIDADE_HPP_INCLUDED
#define CARTAOATIVIDADE_HPP_INCLUDED

#include "estado.hpp"
#include "identificador.hpp"
#include "nome.hpp"
#include "prioridade.hpp"
#include "tamanho.hpp"
#include "texto.hpp"
#include "timestamp.hpp"

/**
 * @brief Entidade cartao de atividade: identificador (chave primaria), nome,
 *        descricao, prioridade, estado, tamanho e os instantes de entrada,
 *        inicio e termino.
 *
 * Os instantes registram a passagem pelas colunas do quadro:
 * - entrada: quando o cartao entrou em A FAZER;
 * - inicio: quando entrou em FAZENDO (vazio enquanto nao entrar);
 * - termino: quando chegou em FEITO (vazio enquanto nao chegar).
 *
 * Cada atributo e um dominio que ja se valida sozinho, entao a entidade so
 * guarda e devolve copias deles. O quadro e o desenvolvedor do cartao ficam no
 * ContainerCartao, porque nao sao atributos do cartao no diagrama de classes.
 */
class CartaoAtividade {
    private:
        Identificador identificador;
        Nome nome;
        Texto descricao;
        Prioridade prioridade;
        Estado estado;
        Tamanho tamanho;
        Timestamp entrada;
        Timestamp inicio;
        Timestamp termino;

    public:
        /** @brief Guarda o identificador (chave primaria). */
        void setIdentificador(const Identificador& identificador);

        /** @brief Retorna o identificador. */
        Identificador getIdentificador() const;

        /** @brief Guarda o nome da atividade. */
        void setNome(const Nome& nome);

        /** @brief Retorna o nome da atividade. */
        Nome getNome() const;

        /** @brief Guarda a descricao da atividade. */
        void setDescricao(const Texto& descricao);

        /** @brief Retorna a descricao da atividade. */
        Texto getDescricao() const;

        /** @brief Guarda a prioridade (ALTA, MEDIA ou BAIXA). */
        void setPrioridade(const Prioridade& prioridade);

        /** @brief Retorna a prioridade. */
        Prioridade getPrioridade() const;

        /** @brief Guarda o estado, ou seja, a coluna atual (A FAZER, FAZENDO ou FEITO). */
        void setEstado(const Estado& estado);

        /** @brief Retorna o estado (coluna atual). */
        Estado getEstado() const;

        /** @brief Guarda o tamanho (GRANDE, MEDIO ou PEQUENO). */
        void setTamanho(const Tamanho& tamanho);

        /** @brief Retorna o tamanho. */
        Tamanho getTamanho() const;

        /** @brief Guarda o instante em que o cartao entrou em A FAZER. */
        void setEntrada(const Timestamp& entrada);

        /** @brief Retorna o instante de entrada em A FAZER. */
        Timestamp getEntrada() const;

        /** @brief Guarda o instante em que o cartao entrou em FAZENDO. */
        void setInicio(const Timestamp& inicio);

        /** @brief Retorna o instante de entrada em FAZENDO (vazio se ainda nao entrou). */
        Timestamp getInicio() const;

        /** @brief Guarda o instante em que o cartao chegou em FEITO. */
        void setTermino(const Timestamp& termino);

        /** @brief Retorna o instante de chegada em FEITO (vazio se ainda nao chegou). */
        Timestamp getTermino() const;
};

#endif // CARTAOATIVIDADE_HPP_INCLUDED
