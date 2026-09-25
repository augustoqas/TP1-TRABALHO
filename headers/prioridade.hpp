#ifndef PRIORIDADE_H
#define PRIORIDADE_H

#include <string>
using namespace std;

class Prioridade {
    private:
    static const string ALTA;
    static const string MEDIA;
    static const string BAIXA;
    string valor;
    bool validar(string);

    public:
    bool setValor(string);
    string getValor();
};

inline string Prioridade::getValor() {
    return valor;
}

#endif
