#ifndef TELA_HPP_INCLUDED
#define TELA_HPP_INCLUDED

#include <stdexcept>
#include <string>
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
 *     Tela::falha("Nao foi possivel criar o quadro.", {"usuario nao e GESTOR", "projeto nao existe", "identificador ja existe"});
 * }
 * @endcode
 */
class Tela {
    public:
        /** @brief Resultado de lerCampo e lerCampoInteiro. */
        enum class Leitura {
            LIDO,       ///< Valor valido, ja guardado no dominio.
            CANCELADO,  ///< O usuario digitou 0 (o aviso ja foi mostrado); abandone a operacao.
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
         * @brief Mostra a pergunta e le uma linha, sem '\\r' nem espacos no fim.
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
         * @param resposta true para S/s; false para N/n ou 0.
         * @return false se a entrada acabou (EOF).
         */
        static bool confirmar(const std::string& pergunta, bool& resposta);

        /**
         * @brief Le um campo de dominio, pedindo de novo ate o valor ser valido.
         *
         * Serve para qualquer dominio com setValor(std::string) que lanca
         * std::invalid_argument para valor invalido. A dica aparece entre parenteses.
         * Se o usuario digitar 0, ja mostra "Operacao cancelada" e retorna CANCELADO.
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

        // Retorna 0 a 99, ou -1 se o texto nao for um numero de 1 ou 2 digitos.
        static int converterOpcao(const std::string& texto);

        static bool converterInteiro(const std::string& texto, int& numero);
};

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
            dominio.setValor(texto);
            return Leitura::LIDO;
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
            dominio.setValor(numero);
            return Leitura::LIDO;
        } catch (const std::invalid_argument& excecao) {
            erro(std::string(excecao.what()) + " Tente novamente.");
        }
    }
}

#endif // TELA_HPP_INCLUDED
