#ifndef EXCECAOSESSAOENCERRADA_HPP_INCLUDED
#define EXCECAOSESSAOENCERRADA_HPP_INCLUDED

#include <stdexcept>

/**
 * @brief Excecao que avisa que a sessao do usuario terminou dentro de uma
 *        controladora de apresentacao (ex.: o usuario excluiu a propria conta).
 *
 * IApresentacaoPessoa::executar retorna void e a interface nao pode mudar.
 * Por isso a apresentacao de pessoas lanca esta excecao depois de mostrar as
 * mensagens ao usuario, e o Controle (menu principal) a captura e volta ao
 * menu inicial. Assim o Controle continua dependendo somente das interfaces
 * (e desta classe), sem conhecer a controladora concreta nem o servico.
 *
 * Nao e erro de programa: e um sinal de fluxo entre camadas de apresentacao.
 */
class ExcecaoSessaoEncerrada : public std::runtime_error {
    public:
        /**
         * @brief Cria a excecao com uma mensagem padrao.
         */
        ExcecaoSessaoEncerrada();
};

#endif // EXCECAOSESSAOENCERRADA_HPP_INCLUDED
