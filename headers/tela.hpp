#ifndef TELA_HPP_INCLUDED
#define TELA_HPP_INCLUDED

#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

/**
 * @brief Utilitarios de tela (entrada e saida em modo texto) compartilhados
 *        pelas controladoras de apresentacao.
 *
 * Centraliza o visual (titulos, separadores, menus numerados, mensagens de
 * sucesso/erro/aviso) e a leitura linha a linha com std::getline, para que
 * todas as telas do sistema tenham a mesma aparencia e o mesmo tratamento de
 * fim de entrada (EOF).
 *
 * Convencoes:
 * - Toda leitura usa std::getline(std::cin, ...) e remove um '\\r' final.
 * - Cada pergunta e impressa em uma linha propria (terminada em '\\n'). Assim
 *   a saida continua legivel quando a entrada vem de arquivo (o texto
 *   digitado nao aparece na tela).
 * - Nada digitado e repetido na saida (nenhuma senha aparece).
 * - Em campos de dominio, digitar CANCELAR ("0") cancela a operacao. "0" e
 *   invalido em todos os dominios do sistema, entao nao ha conflito.
 * - lerCampo e lerCampoInteiro aceitam dominios cujo setValor lanca
 *   std::invalid_argument (padrao do projeto) e tambem os que ainda retornam
 *   bool (false = valor recusado): nunca aceitam valor invalido em silencio.
 *
 * Todos os membros sao estaticos: a classe nao guarda estado proprio (o
 * estado de fim de entrada e o do proprio std::cin).
 *
 * Exemplo de uso (ex.: em CntrApresentacaoQuadro):
 * @code
 * int opcao = Tela::lerOpcao("MENU DE QUADROS", {"Criar quadro", "Voltar"});
 * if (opcao == 0) {
 *     return;                                      // fim da entrada
 * }
 * Tela::titulo("CRIAR QUADRO");
 * Identificador identificador;
 * if (Tela::lerCampo("Identificador", "3 letras e 3 digitos", identificador) != Tela::Leitura::LIDO) {
 *     return;                                      // cancelado ou fim da entrada
 * }
 * Limite limite;                                   // setValor recebe int
 * if (Tela::lerCampoInteiro("Limite", "1 a 25", limite) != Tela::Leitura::LIDO) {
 *     return;
 * }
 * if (!servico->criar(usuario, projeto, quadro)) {
 *     Tela::falha("Nao foi possivel criar o quadro.", {"usuario nao e GESTOR", "projeto nao existe"});
 * }
 * @endcode
 */
class Tela {
    public:
        /**
         * @brief Resultado da leitura de um campo de formulario.
         */
        enum class Leitura {
            LIDO,       ///< Valor valido lido e armazenado no dominio.
            CANCELADO,  ///< Usuario digitou CANCELAR; operacao deve ser abandonada.
            FIM         ///< Fim da entrada (EOF); o programa deve encerrar.
        };

        static const std::string CANCELAR;  ///< Texto que cancela um formulario ("0").

        /**
         * @brief Mostra um titulo de tela entre linhas duplas.
         * @param titulo Texto do titulo.
         */
        static void titulo(const std::string& titulo);

        /**
         * @brief Mostra uma linha separadora simples.
         */
        static void separador();

        /**
         * @brief Mostra uma linha de texto comum, indentada.
         * @param texto Texto a mostrar.
         */
        static void info(const std::string& texto);

        /**
         * @brief Mostra um dado no formato "rotulo: valor".
         * @param rotulo Nome do dado (ex.: "Email").
         * @param valor Valor do dado.
         */
        static void campo(const std::string& rotulo, const std::string& valor);

        /**
         * @brief Mostra uma mensagem de sucesso bem visivel ("[OK] ...").
         * @param texto Mensagem.
         */
        static void sucesso(const std::string& texto);

        /**
         * @brief Mostra o resultado de uma operacao recusada, bem visivel
         *        ("[FALHA] ..." apos uma linha em branco), seguido dos motivos.
         * @param texto Mensagem principal.
         * @param motivos Motivos possiveis, um por linha (pode ser vazio).
         */
        static void falha(const std::string& texto, const std::vector<std::string>& motivos = std::vector<std::string>());

        /**
         * @brief Mostra um erro de digitacao em um campo ("[ERRO] ..."), sem
         *        linhas em branco, pois o campo e pedido de novo em seguida.
         * @param texto Mensagem.
         */
        static void erro(const std::string& texto);

        /**
         * @brief Mostra uma mensagem de aviso bem visivel ("[AVISO] ...").
         * @param texto Mensagem.
         */
        static void aviso(const std::string& texto);

        /**
         * @brief Mostra a mensagem padrao de operacao cancelada.
         */
        static void cancelado();

        /**
         * @brief Mostra uma pergunta e le uma linha da entrada padrao.
         * @param pergunta Texto da pergunta (impresso em linha propria).
         * @param linha Saida: linha lida, sem '\\r' final.
         * @return true se leu; false se a entrada terminou (EOF).
         */
        static bool lerLinha(const std::string& pergunta, std::string& linha);

        /**
         * @brief Mostra um menu numerado (a partir de 1) e le a opcao.
         *
         * Se a opcao nao for numero ou estiver fora da faixa, mostra
         * "Opcao invalida" e o mesmo menu de novo.
         * @param titulo Titulo do menu.
         * @param opcoes Textos das opcoes, na ordem em que sao numeradas.
         * @return Opcao escolhida (1 a opcoes.size()); 0 se a entrada terminou.
         */
        static int lerOpcao(const std::string& titulo, const std::vector<std::string>& opcoes);

        /**
         * @brief Faz uma pergunta de sim/nao ate obter S/s ou N/n.
         * @param pergunta Texto da pergunta.
         * @param resposta Saida: true para S/s; false para N/n.
         * @return true se obteve resposta; false se a entrada terminou (EOF).
         */
        static bool confirmar(const std::string& pergunta, bool& resposta);

        /**
         * @brief Le um campo de dominio, repetindo ate o valor ser valido.
         *
         * Funciona com qualquer dominio que tenha "setValor(std::string)":
         * se setValor lancar std::invalid_argument, a mensagem da excecao e
         * mostrada; se setValor retornar false (dominios que ainda retornam
         * bool), mostra "<rotulo>: valor invalido". Em ambos os casos o campo
         * e pedido de novo. Digitar CANCELAR mostra "Operacao cancelada".
         * @param rotulo Nome do campo (ex.: "Email").
         * @param dica Formato esperado, mostrado entre parenteses.
         * @param dominio Saida: dominio preenchido (somente se retornar LIDO).
         * @return LIDO, CANCELADO ou FIM.
         */
        template<typename Dominio>
        static Leitura lerCampo(const std::string& rotulo, const std::string& dica, Dominio& dominio);

        /**
         * @brief Igual a lerCampo, para dominios cujo setValor recebe int
         *        (ex.: Limite).
         *
         * Se o texto nao for um numero inteiro, mostra mensagem e pede de novo.
         * @param rotulo Nome do campo (ex.: "Limite").
         * @param dica Formato esperado, mostrado entre parenteses.
         * @param dominio Saida: dominio preenchido (somente se retornar LIDO).
         * @return LIDO, CANCELADO ou FIM.
         */
        template<typename Dominio>
        static Leitura lerCampoInteiro(const std::string& rotulo, const std::string& dica, Dominio& dominio);

        /**
         * @brief Informa se a entrada padrao terminou (std::getline falhou).
         * @return true se std::cin esta em estado de falha; false caso contrario.
         */
        static bool entradaEncerrada();

    private:
        static const std::string LINHA_DUPLA;   ///< Linha usada nos titulos.
        static const std::string LINHA_SIMPLES; ///< Linha usada nos separadores.
        static const std::string MARGEM;        ///< Recuo do texto da tela.

        /**
         * @brief Converte texto em numero inteiro positivo pequeno.
         * @param texto Texto lido.
         * @return Numero (1 a 99) ou -1 se o texto nao for numero nessa faixa.
         */
        static int converterOpcao(const std::string& texto);

        /**
         * @brief Converte texto em int (somente digitos, '-' opcional no inicio).
         * @param texto Texto lido.
         * @param numero Saida: numero convertido (somente se retornar true).
         * @return true se o texto e um inteiro de ate 9 digitos; false caso contrario.
         */
        static bool converterInteiro(const std::string& texto, int& numero);

        /**
         * @brief Chama dominio.setValor(valor) e informa se o valor foi aceito.
         *
         * Escolhe, em tempo de compilacao, uma das duas versoes abaixo conforme
         * o tipo de retorno de setValor (bool ou void).
         * @param dominio Dominio que recebe o valor.
         * @param valor Valor a armazenar.
         * @return true se aceito; false se setValor retornou false.
         * @throw std::invalid_argument Repassada de setValor.
         */
        template<typename Dominio, typename Valor>
        static bool atribuir(Dominio& dominio, const Valor& valor);

        /**
         * @brief Versao de atribuir para setValor que retorna bool.
         * @param dominio Dominio que recebe o valor.
         * @param valor Valor a armazenar.
         * @return Retorno de setValor.
         */
        template<typename Dominio, typename Valor>
        static bool atribuir(Dominio& dominio, const Valor& valor, std::true_type);

        /**
         * @brief Versao de atribuir para setValor que retorna void (valor
         *        invalido gera excecao).
         * @param dominio Dominio que recebe o valor.
         * @param valor Valor a armazenar.
         * @return Sempre true.
         * @throw std::invalid_argument Repassada de setValor.
         */
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
