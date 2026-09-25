#ifndef PAPEL_H
#define PAPEL_H

#include <string>
using namespace std;

class Papel {
    private:
    static const string GESTOR;
    static const string DESENVOLVEDOR;
    string valor;
    bool validar(string);

    public:
    bool setValor(string);
    string getValor();
};

inline string Papel::getValor() {
    return valor;
}

#endif
