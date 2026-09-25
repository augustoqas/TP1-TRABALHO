#ifndef LIMITE_H
#define LIMITE_H

class Limite {
    private:
    static const int MINIMO = 1;
    static const int MAXIMO = 25;
    int valor;
    bool validar(int);

    public:
    bool setValor(int);
    int getValor();
};

inline int Limite::getValor() {
    return valor;
}

#endif
