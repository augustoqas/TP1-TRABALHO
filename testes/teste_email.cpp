// Testes do dominio Email: cada linha e um caso (valido = setValor aceita; invalido = lanca).
#include "email.hpp"
#include "teste.hpp"

int main() {
    TesteDominio<Email> email("sentinela@unb.br");
    const std::string B63(63, 'b');  // parte de dominio com 63 letras

    email.valido("joao@unb.br", "basico com dominio de duas partes");
    email.valido("a@b", "minimo: 1 char local, dominio de uma parte");
    email.valido("a@localhost", "dominio de uma unica parte e permitido (uma ou mais partes)");
    email.valido("joao.silva@unb.br", "ponto no meio da parte local");
    email.valido("joao-silva@unb.br", "hifen no meio da parte local");
    email.valido("a.b-c.d@x.y", "ponto e hifen alternados, cada um seguido de letra");
    email.valido("a1.2b-3@x.y", "ponto/hifen seguidos de digito");
    email.valido("123@456", "so digitos nas duas metades");
    email.valido("aluno@aluno.unb.br", "dominio com tres partes");
    email.valido("a@meu-dominio.com", "hifen no meio de parte do dominio");
    email.valido("a@x--y.com", "hifen seguido de hifen no dominio (regra so vale p/ parte local)");
    email.valido("a@1.2.3", "partes do dominio so com digitos");
    email.valido(std::string(64, 'a') + "@unb.br", "parte local com exatamente 64 chars");
    email.valido("a@" + std::string(251, 'b') + ".com", "dominio com exatamente 255 chars");
    email.valido("a@" + B63 + "." + B63 + "." + B63 + "." + B63, "dominio 255 chars em 4 partes de 63");
    email.valido(std::string(64, 'a') + "@" + std::string(255, 'b'), "maximos simultaneos: 64 + @ + 255 = 320 chars");
    email.invalido("", "string vazia");
    email.invalido("joaounb.br", "sem @");
    email.invalido("a@b@c", "dois @");
    email.invalido("@unb.br", "parte local vazia");
    email.invalido("joao@", "dominio vazio");
    email.invalido("@", "so o @");
    email.invalido("Joao@unb.br", "maiuscula na parte local (so a-z)");
    email.invalido("joao_silva@unb.br", "sublinhado na parte local");
    email.invalido("joao+tag@unb.br", "mais na parte local");
    email.invalido("jo ao@unb.br", "espaco na parte local");
    email.invalido(" joao@unb.br", "espaco no inicio");
    email.invalido("joao@unb.br ", "espaco no fim");
    email.invalido(".joao@unb.br", "parte local inicia com ponto");
    email.invalido("-joao@unb.br", "parte local inicia com hifen");
    email.invalido("joao.@unb.br", "parte local termina com ponto");
    email.invalido("joao-@unb.br", "parte local termina com hifen");
    email.invalido("jo..ao@unb.br", "ponto seguido de ponto");
    email.invalido("jo--ao@unb.br", "hifen seguido de hifen na parte local");
    email.invalido("jo.-ao@unb.br", "ponto seguido de hifen");
    email.invalido("jo-.ao@unb.br", "hifen seguido de ponto");
    email.invalido(".@unb.br", "parte local so com ponto");
    email.invalido("-@unb.br", "parte local so com hifen");
    email.invalido(std::string(65, 'a') + "@unb.br", "parte local com 65 chars");
    email.invalido("joao@UnB.br", "maiuscula no dominio (so a-z)");
    email.invalido("joao@unb_br.com", "sublinhado no dominio");
    email.invalido("joao@un b.br", "espaco no dominio");
    email.invalido("joao@.unb.br", "dominio inicia com ponto (parte vazia)");
    email.invalido("joao@unb.br.", "dominio termina com ponto (parte vazia)");
    email.invalido("joao@unb..br", "dois pontos seguidos no dominio (parte vazia)");
    email.invalido("joao@.", "dominio so com ponto");
    email.invalido("joao@-unb.br", "parte do dominio inicia com hifen");
    email.invalido("joao@unb-.br", "parte do dominio termina com hifen");
    email.invalido("joao@unb.-br", "segunda parte inicia com hifen");
    email.invalido("joao@unb.br-", "ultima parte termina com hifen");
    email.invalido("joao@-", "dominio so com hifen");
    email.invalido("a@" + std::string(252, 'b') + ".com", "dominio com 256 chars");
    email.invalido(std::string(65, 'a') + "@" + std::string(255, 'b'), "local 65 com dominio 255");
    email.invalido(std::string(64, 'a') + "@" + std::string(256, 'b'), "local 64 com dominio 256");

    return resultado();
}
