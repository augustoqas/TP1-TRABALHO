#ifndef TIMESTAMP_HPP_INCLUDED
#define TIMESTAMP_HPP_INCLUDED

#include <stdexcept>
#include <string>

/**
 * @brief Dominio de data e hora (inicio e termino de Projeto; entrada, inicio
 *        e termino de Cartao).
 *
 * Formato: "DIA-MES-AAAA-HH:MM", com DIA de 1 a 31 (1 ou 2 digitos: "5" ou
 * "05"), MES em JAN FEV MAR ABR MAI JUN JUL AGO SET OUT NOV DEZ, AAAA de 2000
 * a 2099 e HH:MM de 00:00 a 23:59. A data precisa existir, considerando anos
 * bissextos (29-FEV-2025 e invalido).
 *
 * Ex.: "15-MAR-2026-14:30" ou "5-MAR-2026-14:30"
 */
class Timestamp {
    private:
        // Tamanho de "-MES-AAAA-HH:MM", o que vem depois do dia.
        static const std::string::size_type TAMANHO_SEM_DIA = 15;
        static const int ANO_MINIMO = 2000;
        static const int ANO_MAXIMO = 2099;
        static const char* const MESES[12];

        std::string valor;
        long minutos = 0;

        static bool ehBissexto(int ano);
        static int diasNoMes(int mes, int ano);
        static bool lerNumero(const std::string& texto, std::string::size_type inicio,
                              std::string::size_type tamanho, int& numero);

        // Valida o formato e devolve os minutos desde 01-JAN-2000-00:00.
        static long validar(std::string valor);

    public:
        /**
         * @brief Guarda o timestamp.
         * @param valor Data e hora no formato "DIA-MES-AAAA-HH:MM" (dia com 1 ou 2 digitos).
         * @throw std::invalid_argument Se o formato ou a data forem invalidos (o valor anterior fica).
         */
        void setValor(std::string valor);

        /** @brief Retorna o timestamp como foi informado em setValor ("" se nada foi guardado). */
        std::string getValor() const;

        /**
         * @brief Minutos desde 01-JAN-2000-00:00 (0 se nada foi guardado).
         *
         * A diferenca entre dois timestamps da a duracao em minutos (usada no
         * tempo de ciclo e no lead time dos cartoes).
         * @return Minutos desde 01-JAN-2000-00:00.
         */
        long getMinutos() const;

        /**
         * @brief Cria um timestamp com a data e a hora atuais do computador.
         * @return Timestamp do momento atual.
         * @throw std::invalid_argument Se o relogio estiver fora de 2000 a 2099.
         */
        static Timestamp agora();
};

inline std::string Timestamp::getValor() const {
    return valor;
}

inline long Timestamp::getMinutos() const {
    return minutos;
}

#endif // TIMESTAMP_HPP_INCLUDED
