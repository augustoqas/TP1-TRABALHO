#ifndef TELA_HPP_INCLUDED
#define TELA_HPP_INCLUDED

#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

/**
 * @brief Funcoes de tela em modo texto usadas por todas as controladoras de
 *        apresentacao, para que todas as telas tenham a mesma cara.
 *
 * Tudo e estatico. Cada pergunta sai numa linha propria e nada digitado e
 * repetido na tela (a senha nunca aparece). Digitar 0 em um campo cancela;
 * nenhum dominio aceita "0", entao nao ha conflito.
 *
 * Exemplo (tela de quadros):
 * @code
 * int opcao = Tela::lerOpcao("MENU DE QUADROS", {"Criar quadro", "Voltar"});
 * if (opcao == 0) {
 *     return;  // a entrada acabou
 * }
 * Tela::titulo("CRIAR QUADRO");
 * Identificador identificador;
 * if (Tela::lerCampo("Identificador", "3 letras e 3 digitos", identificador) != Tela::Leitura::LIDO) {
 *     return;  // cancelou ou a entrada acabou
 * }
 * Limite limite;  // setValor recebe int
 * if (Tela::lerCampoInteiro("Limite", "1 a 25", limite) != Tela::Leitura::LIDO) {
 *     return;
 * }
 * if (!servico->criar(usuario, projeto, quadro)) {
 *     Tela::falha("Nao foi possivel criar o quadro.", {"usuario nao e GESTOR"});
 * }
 * @endcode
 */
class Tela {
    public:
        /** @brief Resultado de lerCampo e lerCampoInteiro. */
        enum class Leitura {
            LIDO,       ///< Valor valido, ja guardado no dominio.
            CANCELADO,  ///< O usuario digitou 0; abandone a operacao.
            FIM         ///< A entrada acabou (EOF); volte sem fazer mais nada.
        };

        static const std::string CANCELAR;  ///< O que o usuario digita para cancelar ("0").

        /** @brief Mostra um titulo entre linhas duplas. */
        static void titulo(const std::string& texto);

        /** @brief Mostra uma linha separadora. */
        static void separador();

        /** @brief Mostra uma linha de texto comum. */
        static void info(const std::string& texto);

        /** @brief Mostra um dado como "rotulo: valor". */
        static void campo(const std::string& rotulo, const std::string& valor);

        /** @brief Mostra "[OK] texto". */
        static void sucesso(const std::string& texto);

        /** @brief Mostra "[FALHA] texto" e, se houver, a lista de motivos possiveis. */
        static void falha(const std::string& texto, const std::vector<std::string>& motivos = std::vector<std::string>());

        /** @brief Mostra "[ERRO] texto" para um campo digitado errado (sem linha em branco). */
        static void erro(const std::string& texto);

        /** @brief Mostra "[AVISO] texto". */
        static void aviso(const std::string& texto);

        /** @brief Mostra o aviso padrao de operacao cancelada. */
        static void cancelado();

        /**
         * @brief Mostra a pergunta e le uma linha, sem '\r' nem espacos no fim.
         * @return false se a entrada acabou (EOF).
         */
        static bool lerLinha(const std::string& pergunta, std::string& linha);

        /**
         * @brief Mostra um menu numerado a partir de 1 e le a opcao; repete o
         *        menu se a opcao for invalida.
         * @return Opcao escolhida, ou 0 se a entrada acabou.
         */
        static int lerOpcao(const std::string& tituloMenu, const std::vector<std::string>& opcoes);

        /**
         * @brief Pergunta S/N ate ter uma resposta valida.
         * @param resposta true para S; false para N ou 0.
         * @return false se a entrada acabou (EOF).
         */
        static bool confirmar(const std::string& pergunta, bool& resposta);

        /**
         * @brief Le um campo de dominio, pedindo de novo ate o valor ser valido.
         *
         * Serve para qualquer dominio com setValor(std::string), lance ele
         * excecao ou retorne bool. A dica aparece entre parenteses.
         * @param dominio So e preenchido quando o retorno e LIDO.
         */
        template<typename Dominio>
        static Leitura lerCampo(const std::string& rotulo, const std::string& dica, Dominio& dominio);

        /** @brief Igual a lerCampo, para dominios cujo setValor recebe int (ex.: Limite). */
        template<typename Dominio>
        static Leitura lerCampoInteiro(const std::string& rotulo, const std::string& dica, Dominio& dominio);

        /** @brief Diz se a entrada acabou (EOF). */
        static bool entradaEncerrada();

    private:
        static const std::string LINHA_DUPLA;
        static const std::string LINHA_SIMPLES;
        static const std::string MARGEM;

        // Retorna 1 a 99, ou -1 se o texto nao for numero nessa faixa.
        static int converterOpcao(const std::string& texto);

        static bool converterInteiro(const std::string& texto, int& numero);

        // Chama dominio.setValor(valor) e diz se o valor foi aceito, tanto para
        // setValor que retorna bool quanto para o que lanca excecao.
        // PROVISORIO: so existe porque Papel, Prioridade, Tamanho e Limite ainda
        // retornam bool. Quando todos lancarem excecao, remover e chamar setValor direto.
        template<typename Dominio, typename Valor>
        static bool atribuir(Dominio& dominio, const Valor& valor);

        // Versao para setValor que retorna bool.
        template<typename Dominio, typename Valor>
        static bool atribuir(Dominio& dominio, const Valor& valor, std::true_type);

        // Versao para setValor que retorna void (valor invalido lanca excecao).
        template<typename Dominio, typename Valor>
        static bool atribuir(Dominio& dominio, const Valor& valor, std::false_type);
};

template<typename Dominio, typename Valor>
bool Tela::atribuir(Dominio& dominio, const Valor& valor) {
    typedef decltype(dominio.setValor(valor)) Retorno;
    return atribuir(dominio, valor, std::is_same<Retorno, bool>());
}

template<typename Dominio, typename Valor>
bool Tela::atribuir(Dominio& dominio, const Valor& valor, std::true_type) {
    return dominio.setValor(valor);
}

template<typename Dominio, typename Valor>
bool Tela::atribuir(Dominio& dominio, const Valor& valor, std::false_type) {
    dominio.setValor(valor);
    return true;
}

template<typename Dominio>
Tela::Leitura Tela::lerCampo(const std::string& rotulo, const std::string& dica, Dominio& dominio) {
    std::string texto;
    while (true) {
        if (!lerLinha(rotulo + " (" + dica + "):", texto)) {
            return Leitura::FIM;
        }
        if (texto == CANCELAR) {
            cancelado();
            return Leitura::CANCELADO;
        }
        try {
            if (atribuir(dominio, texto)) {
                return Leitura::LIDO;
            }
            erro(rotulo + ": valor invalido. Tente novamente.");
        } catch (const std::invalid_argument& excecao) {
            erro(std::string(excecao.what()) + " Tente novamente.");
        }
    }
}

template<typename Dominio>
Tela::Leitura Tela::lerCampoInteiro(const std::string& rotulo, const std::string& dica, Dominio& dominio) {
    std::string texto;
    int numero = 0;
    while (true) {
        if (!lerLinha(rotulo + " (" + dica + "):", texto)) {
            return Leitura::FIM;
        }
        if (texto == CANCELAR) {
            cancelado();
            return Leitura::CANCELADO;
        }
        if (!converterInteiro(texto, numero)) {
            erro(rotulo + ": digite um numero inteiro. Tente novamente.");
            continue;
        }
        try {
            if (atribuir(dominio, numero)) {
                return Leitura::LIDO;
            }
            erro(rotulo + ": valor invalido. Tente novamente.");
        } catch (const std::invalid_argument& excecao) {
            erro(std::string(excecao.what()) + " Tente novamente.");
        }
    }
}

#endif // TELA_HPP_INCLUDED
