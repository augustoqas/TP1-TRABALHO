#ifndef EXCECAOSESSAOENCERRADA_HPP_INCLUDED
#define EXCECAOSESSAOENCERRADA_HPP_INCLUDED

#include <stdexcept>

/**
 * @brief Lancada pela tela de pessoas quando o usuario exclui a propria conta.
 *
 * IApresentacaoPessoa::executar retorna void e a interface nao pode mudar,
 * entao a sessao termina por excecao: o Controle captura e volta ao menu
 * inicial. Nao e erro, e so um aviso entre telas.
 */
class ExcecaoSessaoEncerrada : public std::runtime_error {
    public:
        /** @brief Cria a excecao com a mensagem padrao. */
        ExcecaoSessaoEncerrada();
};

#endif // EXCECAOSESSAOENCERRADA_HPP_INCLUDED
