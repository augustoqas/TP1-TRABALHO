#ifndef ESTADO_H
#define ESTADO_H

#include <string>
using namespace std;

class Estado {
    private:
    static const string A_FAZER;
    static const string FAZENDO;
    static const string FEITO;
    string valor;
    bool validar(string);

    public:
    bool setValor(string);
    string getValor(string);
};

inline string getValor() {
    return valor;
}

#endif