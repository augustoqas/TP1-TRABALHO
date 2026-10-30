#include "excecaosessaoencerrada.hpp"

ExcecaoSessaoEncerrada::ExcecaoSessaoEncerrada()
    : std::runtime_error("Sessao encerrada: o usuario nao esta mais cadastrado.") {
}
