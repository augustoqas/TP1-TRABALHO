#ifndef TESTE_HPP_INCLUDED
#define TESTE_HPP_INCLUDED

// Conferencia minima usada por todos os testes/teste_*.cpp (ver testes/README.md).
//   verificar(condicao, "descricao");   imprime PASS ou FAIL
//   return resultado();                 ultima linha do main (o rodar le o placar)
//   TesteDominio<Email> email("ana@unb.br");   valor valido usado como "valor anterior"
//   email.valido("a@b", "descricao");  email.invalido("A@b", "descricao");

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

struct Placar {
    int total;
    int falhas;
};

inline Placar& placar() {
    static Placar p = {0, 0};
    return p;
}

inline void verificar(bool condicao, const std::string& descricao) {
    placar().total++;
    if (!condicao) {
        placar().falhas++;
    }
    std::cout << (condicao ? "PASS  " : "FAIL  ") << descricao << std::endl;
}

inline int resultado() {
    std::cout << "RESULTADO " << placar().total - placar().falhas << "/" << placar().total << std::endl;
    return placar().falhas == 0 ? 0 : 1;
}

// Mostra bytes invisiveis como \xHH e encurta textos longos.
inline std::string visivel(const std::string& s) {
    const char* hex = "0123456789ABCDEF";
    std::string r = "\"";
    for (std::string::size_type i = 0; i < s.size() && i < 40; i++) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (c >= 32 && c < 127) {
            r += static_cast<char>(c);
        } else {
            r += "\\x";
            r += hex[c >> 4];
            r += hex[c & 15];
        }
    }
    std::ostringstream tamanho;
    tamanho << s.size();
    return r + (s.size() > 40 ? "...\" (" + tamanho.str() + " caracteres)" : "\"");
}

inline std::string visivel(int valor) {
    std::ostringstream r;
    r << valor;
    return r.str();
}

// Testa um dominio cujo setValor lanca std::invalid_argument e cujo getValor e const.
// Dominio que ainda devolve bool no setValor nao compila aqui: use verificar (ver README).
// Valor = std::string (padrao) ou int (ex.: TesteDominio<Limite, int> limite(5); depois que
// Limite seguir a convencao).
template <class Dominio, class Valor = std::string>
class TesteDominio {
    private:
        Valor base;

        void conferir(bool esperaValido, const Valor& entrada, const std::string& descricao) {
            Dominio dominio;
            const Dominio& lido = dominio;
            std::string erro;
            try {
                dominio.setValor(base);
            } catch (...) {
                verificar(false, "valor base " + visivel(base) + " foi rejeitado (corrija o teste)");
                return;
            }
            try {
                dominio.setValor(entrada);
                if (!esperaValido) {
                    erro = "aceitou (devia lancar invalid_argument)";
                } else if (lido.getValor() != entrada) {
                    erro = "getValor diferente da entrada";
                }
            } catch (const std::invalid_argument&) {
                if (esperaValido) {
                    erro = "lancou invalid_argument";
                } else if (lido.getValor() != base) {
                    erro = "valor anterior foi alterado";
                }
            } catch (...) {
                erro = "lancou excecao de tipo errado";
            }
            verificar(erro.empty(), std::string(esperaValido ? "V " : "I ") + visivel(entrada) + (descricao.empty() ? "" : "  ") +
                                    descricao + (erro.empty() ? "" : "  <- " + erro));
        }

    public:
        explicit TesteDominio(const Valor& valorBase) : base(valorBase) {
        }

        void valido(const Valor& entrada, const std::string& descricao = "") {
            conferir(true, entrada, descricao);
        }

        void invalido(const Valor& entrada, const std::string& descricao = "") {
            conferir(false, entrada, descricao);
        }
};

#endif // TESTE_HPP_INCLUDED
