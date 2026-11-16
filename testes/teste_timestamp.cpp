// Testes do dominio Timestamp: "DD-MES-AAAA-HH:MM", ano de 2000 a 2099, data valida
// (com bissextos), mais getMinutos e agora.
#include "timestamp.hpp"
#include "teste.hpp"

static long minutos(const std::string& valor) {
    Timestamp timestamp;
    timestamp.setValor(valor);
    return timestamp.getMinutos();
}

int main() {
    TesteDominio<Timestamp> timestamp("01-JAN-2000-00:00");

    timestamp.valido("15-MAR-2026-14:30");
    timestamp.valido("01-JAN-2000-00:00", "menor valor");
    timestamp.valido("31-DEZ-2099-23:59", "maior valor");
    timestamp.valido("31-JAN-2026-12:00");
    timestamp.valido("28-FEV-2025-08:00");
    timestamp.valido("29-FEV-2024-08:00", "bissexto");
    timestamp.valido("29-FEV-2000-08:00", "2000 e bissexto (divisivel por 400)");
    timestamp.valido("30-ABR-2026-10:10");
    timestamp.valido("31-MAI-2026-10:10");
    timestamp.valido("30-JUN-2026-10:10");
    timestamp.valido("31-JUL-2026-10:10");
    timestamp.valido("31-AGO-2026-10:10");
    timestamp.valido("30-SET-2026-10:10");
    timestamp.valido("31-OUT-2026-10:10");
    timestamp.valido("30-NOV-2026-10:10");
    timestamp.invalido("29-FEV-2025-08:00", "2025 nao e bissexto");
    timestamp.invalido("30-FEV-2024-08:00");
    timestamp.invalido("31-ABR-2026-10:10");
    timestamp.invalido("31-JUN-2026-10:10");
    timestamp.invalido("31-SET-2026-10:10");
    timestamp.invalido("31-NOV-2026-10:10");
    timestamp.invalido("32-JAN-2026-10:10");
    timestamp.invalido("00-JAN-2026-10:10", "dia 0");
    timestamp.invalido("15-MAR-1999-14:30", "ano antes de 2000");
    timestamp.invalido("15-MAR-2100-14:30", "ano depois de 2099");
    timestamp.invalido("15-MAR-2026-24:00", "hora 24");
    timestamp.invalido("15-MAR-2026-23:60", "minuto 60");
    timestamp.invalido("15-mar-2026-14:30", "mes em minusculas");
    timestamp.invalido("15-MAC-2026-14:30", "mes inexistente");
    timestamp.invalido("15-FEB-2026-14:30", "mes em ingles");
    timestamp.invalido("15-03-2026-14:30", "mes em numero");
    timestamp.invalido("5-MAR-2026-14:30", "dia com 1 digito");
    timestamp.invalido("15-MAR-26-14:30", "ano com 2 digitos");
    timestamp.invalido("15-MAR-2026-9:30", "hora com 1 digito");
    timestamp.invalido("15-MAR-2026 14:30", "espaco no lugar do hifen");
    timestamp.invalido("15/MAR/2026-14:30");
    timestamp.invalido("15-MAR-2026-14-30");
    timestamp.invalido("15-MAR-2026-14:30 ", "espaco no fim");
    timestamp.invalido(" 15-MAR-2026-14:30", "espaco no inicio");
    timestamp.invalido("15-MAR-2026");
    timestamp.invalido("");
    timestamp.invalido("1A-MAR-2026-14:30");
    timestamp.invalido("15-MAR-20A6-14:30");
    timestamp.invalido("15-MAR-2026-1A:30");
    timestamp.invalido("-1-MAR-2026-14:30");
    timestamp.invalido(std::string("15-MAR-2026-14:3") + '\0', "byte nulo no fim");

    verificar(Timestamp().getMinutos() == 0, "minutos sem valor guardado = 0");
    verificar(minutos("01-JAN-2000-00:00") == 0, "minutos de 01-JAN-2000-00:00 = 0");
    verificar(minutos("01-JAN-2000-01:01") == 61, "minutos somam hora e minuto");
    verificar(minutos("02-JAN-2000-00:00") == 1440, "um dia = 1440 minutos");
    verificar(minutos("01-MAR-2000-00:00") == (31 + 29) * 1440, "fevereiro de 2000 tem 29 dias");
    verificar(minutos("01-JAN-2001-00:00") == 366 * 1440, "2000 tem 366 dias");
    verificar(minutos("01-JAN-2002-00:00") - minutos("01-JAN-2001-00:00") == 365 * 1440, "2001 tem 365 dias");
    verificar(minutos("01-MAR-2024-00:00") - minutos("28-FEV-2024-23:00") == 25 * 60, "virada de fevereiro bissexto");
    verificar(minutos("01-JAN-2027-00:10") - minutos("31-DEZ-2026-23:50") == 20, "virada de ano");
    verificar(minutos("15-MAR-2026-14:30") - minutos("15-MAR-2026-10:00") == 270, "diferenca no mesmo dia");

    Timestamp copia;
    copia.setValor("15-MAR-2026-14:30");
    try {
        copia.setValor("29-FEV-2025-08:00");
    } catch (const std::invalid_argument&) {
    }
    verificar(copia.getMinutos() == minutos("15-MAR-2026-14:30"), "valor invalido nao muda os minutos");

    bool agoraValido = true;
    Timestamp atual;
    try {
        atual = Timestamp::agora();
        Timestamp conferido;
        conferido.setValor(atual.getValor());
        agoraValido = conferido.getMinutos() == atual.getMinutos() && atual.getMinutos() > 0;
    } catch (const std::invalid_argument&) {
        agoraValido = false;
    }
    verificar(agoraValido, "agora devolve timestamp valido (" + atual.getValor() + ")");

    return resultado();
}
