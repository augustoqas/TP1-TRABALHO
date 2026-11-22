#include "timestamp.hpp"

#include <cstdio>
#include <ctime>
#include <stdexcept>

const char* const Timestamp::MESES[12] = {
    "JAN", "FEV", "MAR", "ABR", "MAI", "JUN", "JUL", "AGO", "SET", "OUT", "NOV", "DEZ"
};

bool Timestamp::ehBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0;
}

// mes de 1 a 12.
int Timestamp::diasNoMes(int mes, int ano) {
    static const int DIAS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && ehBissexto(ano)) {
        return 29;
    }
    return DIAS[mes - 1];
}

// Compara faixas em vez de usar isdigit: essa depende de locale e tem
// comportamento indefinido com char negativo (ex.: letra acentuada).
bool Timestamp::lerNumero(const std::string& texto, std::string::size_type inicio,
                          std::string::size_type tamanho, int& numero) {
    numero = 0;
    for (std::string::size_type i = inicio; i < inicio + tamanho; i++) {
        char c = texto[i];
        if (c < '0' || c > '9') {
            return false;
        }
        numero = numero * 10 + (c - '0');
    }
    return true;
}

long Timestamp::validar(std::string valor) {
    // O dia tem 1 ou 2 digitos; o resto ("-MES-AAAA-HH:MM") tem posicoes fixas a partir de d.
    std::string::size_type d = valor.find('-');
    if ((d != 1 && d != 2) || valor.size() != d + TAMANHO_SEM_DIA || valor[d + 4] != '-' ||
        valor[d + 9] != '-' || valor[d + 12] != ':') {
        throw std::invalid_argument("Timestamp invalido: use o formato DIA-MES-AAAA-HH:MM (ex.: 5-MAR-2026-14:30).");
    }

    int dia = 0;
    int ano = 0;
    int hora = 0;
    int minuto = 0;
    if (!lerNumero(valor, 0, d, dia) || !lerNumero(valor, d + 5, 4, ano) ||
        !lerNumero(valor, d + 10, 2, hora) || !lerNumero(valor, d + 13, 2, minuto)) {
        throw std::invalid_argument("Timestamp invalido: dia, ano, hora e minuto devem ser digitos.");
    }

    int mes = 0;
    std::string sigla = valor.substr(d + 1, 3);
    for (int i = 0; i < 12; i++) {
        if (sigla == MESES[i]) {
            mes = i + 1;
        }
    }
    if (mes == 0) {
        throw std::invalid_argument("Timestamp invalido: mes deve ser JAN FEV MAR ABR MAI JUN JUL AGO SET OUT NOV DEZ.");
    }

    if (ano < ANO_MINIMO || ano > ANO_MAXIMO) {
        throw std::invalid_argument("Timestamp invalido: ano deve ser de 2000 a 2099.");
    }
    if (dia < 1 || dia > diasNoMes(mes, ano)) {
        throw std::invalid_argument("Timestamp invalido: esse dia nao existe nesse mes.");
    }
    if (hora > 23 || minuto > 59) {
        throw std::invalid_argument("Timestamp invalido: horario deve ser de 00:00 a 23:59.");
    }

    long dias = dia - 1;
    for (int a = ANO_MINIMO; a < ano; a++) {
        dias += ehBissexto(a) ? 366 : 365;
    }
    for (int m = 1; m < mes; m++) {
        dias += diasNoMes(m, ano);
    }
    return (dias * 24 + hora) * 60 + minuto;
}

void Timestamp::setValor(std::string valor) {
    long minutos = validar(valor);
    this->valor = valor;
    this->minutos = minutos;
}

Timestamp Timestamp::agora() {
    std::time_t segundos = std::time(nullptr);
    std::tm* local = std::localtime(&segundos);
    if (local == nullptr) {
        throw std::invalid_argument("Timestamp invalido: nao foi possivel ler o relogio.");
    }

    char texto[32];
    std::snprintf(texto, sizeof(texto), "%02d-%s-%04d-%02d:%02d", local->tm_mday,
                  MESES[local->tm_mon], local->tm_year + 1900, local->tm_hour, local->tm_min);

    Timestamp timestamp;
    timestamp.setValor(texto);
    return timestamp;
}
