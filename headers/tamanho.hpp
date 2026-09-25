#ifndef TAMANHO_H
#define TAMANHO_H

#include <string>
using namespace std;

class Tamanho {
    private:
    static const string GRANDE;
    static const string MEDIO;
    static const string PEQUENO;
    string valor;
    bool validar(string);

    public:
    bool setValor(string);
    string getValor();
};

inline string Tamanho::getValor() {
    return valor;
}

#endif
