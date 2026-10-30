// Roda todos os testes do TP1. Quem chama e testes/rodar.bat (cmd) ou testes/rodar.sh
// (Git Bash), sempre a partir da raiz do repositorio. Uso: rodar [nome]
// (ex.: "rodar senha" roda so testes/teste_senha.cpp; "rodar roteiros" so os roteiros).
//
// 1. Stubs: testes/stubs/X.hpp e copiado para testes/bin/stubs, a nao ser que
//    headers/X.hpp ja tenha "class X" (ai o stub sai de cena e o rodar avisa).
// 2. Producao: compila cada sources/*.cpp (quem nao compila vira AVISO) e junta
//    tudo em testes/bin/tp1.a. Cada teste usa so o que precisa dessa biblioteca.
// 3. Unidade: compila cada testes/teste_*.cpp com tp1.a, roda e le "RESULTADO ok/total".
// 4. Roteiros: compila testes/main_teste.cpp (teste.exe) e main.cpp (kanban.exe) e
//    confere cada testes/roteiros/NN.txt (entrada) com NN.esperado.
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <windows.h>
#endif

static const std::string GXX = "g++ -std=c++11 -Wall -Wextra -pedantic -Itestes/bin/stubs -Iheaders ";
static const std::string BIN = "testes/bin/";
static const std::string BIBLIOTECA = "testes/bin/tp1.a";
static const int TEMPO_ESGOTADO = -1000;
static const int SAIDA_GRANDE = -1001;
static const int NAO_EXECUTOU = -1002;
static const unsigned long LIMITE_SAIDA = 1024 * 1024;

static int total = 0;
static int ok = 0;
static bool falhou = false;

static std::string ler(const std::string& caminho) {
    std::ifstream arquivo(caminho.c_str(), std::ios::binary);
    std::ostringstream conteudo;
    if (arquivo) {
        conteudo << arquivo.rdbuf();
    }
    return conteudo.str();
}

static bool existe(const std::string& caminho) {
    std::ifstream arquivo(caminho.c_str());
    return arquivo.good();
}

static std::vector<std::string> linhas(const std::string& texto) {
    std::vector<std::string> resultado;
    std::istringstream entrada(texto);
    std::string linha;
    while (std::getline(entrada, linha)) {
        if (!linha.empty() && linha[linha.size() - 1] == '\r') {
            linha.erase(linha.size() - 1);
        }
        resultado.push_back(linha);
    }
    return resultado;
}

static std::string minusculas(std::string texto) {
    for (std::string::size_type i = 0; i < texto.size(); i++) {
        texto[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(texto[i])));
    }
    return texto;
}

// Nomes (sem o sufixo) dos arquivos de "pasta" que comecam com "prefixo" e terminam com "sufixo".
static std::vector<std::string> listar(const std::string& pasta, const std::string& prefixo, const std::string& sufixo) {
    std::vector<std::string> nomes;
    DIR* diretorio = opendir(pasta.c_str());
    if (diretorio == NULL) {
        return nomes;
    }
    for (dirent* item = readdir(diretorio); item != NULL; item = readdir(diretorio)) {
        std::string nome = item->d_name;
        if (nome.size() > prefixo.size() + sufixo.size() && nome.compare(0, prefixo.size(), prefixo) == 0 &&
            nome.compare(nome.size() - sufixo.size(), sufixo.size(), sufixo) == 0) {
            nomes.push_back(nome.substr(0, nome.size() - sufixo.size()));
        }
    }
    closedir(diretorio);
    std::sort(nomes.begin(), nomes.end());
    return nomes;
}

static void criarPasta(const std::string& caminho) {
#ifdef _WIN32
    CreateDirectoryA(caminho.c_str(), NULL);
#else
    mkdir(caminho.c_str(), 0777);
#endif
}

// Roda "comando" com a entrada lida de um arquivo ("" = nenhuma) e stdout + stderr gravados
// em "saida". Devolve o codigo de saida, TEMPO_ESGOTADO, SAIDA_GRANDE ou NAO_EXECUTOU.
static int executar(std::string comando, const std::string& entrada, const std::string& saida, int segundos) {
#ifdef _WIN32
    std::replace(comando.begin(), comando.end(), '/', '\\');
    SECURITY_ATTRIBUTES herdavel = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    HANDLE in = CreateFileA(entrada.empty() ? "NUL" : entrada.c_str(), GENERIC_READ, FILE_SHARE_READ,
                            &herdavel, OPEN_EXISTING, 0, NULL);
    HANDLE out = CreateFileA(saida.c_str(), GENERIC_WRITE | FILE_READ_ATTRIBUTES, FILE_SHARE_READ,
                             &herdavel, CREATE_ALWAYS, 0, NULL);
    STARTUPINFOA info;
    ZeroMemory(&info, sizeof(info));
    info.cb = sizeof(info);
    info.dwFlags = STARTF_USESTDHANDLES;
    info.hStdInput = in;
    info.hStdOutput = out;
    info.hStdError = out;
    PROCESS_INFORMATION processo;
    std::vector<char> linha(comando.begin(), comando.end());
    linha.push_back('\0');
    int codigo = NAO_EXECUTOU;
    if (CreateProcessA(NULL, &linha[0], NULL, NULL, TRUE, 0, NULL, NULL, &info, &processo)) {
        codigo = TEMPO_ESGOTADO;
        for (int ms = 0; ms < segundos * 1000 && codigo == TEMPO_ESGOTADO; ms += 50) {
            DWORD fim = 0;
            if (WaitForSingleObject(processo.hProcess, 50) == WAIT_OBJECT_0) {
                GetExitCodeProcess(processo.hProcess, &fim);
                codigo = static_cast<int>(fim);
            } else if (GetFileSize(out, NULL) > LIMITE_SAIDA) {
                codigo = SAIDA_GRANDE;
            }
        }
        if (codigo == TEMPO_ESGOTADO || codigo == SAIDA_GRANDE) {
            TerminateProcess(processo.hProcess, 1);
            WaitForSingleObject(processo.hProcess, INFINITE);
        }
        CloseHandle(processo.hThread);
        CloseHandle(processo.hProcess);
    }
    CloseHandle(in);
    CloseHandle(out);
    return codigo;
#else
    // Fora do Windows (nao testado): usa o comando "timeout" do sistema.
    std::ostringstream linha;
    linha << "timeout " << segundos << " " << comando << " < " << (entrada.empty() ? "/dev/null" : entrada)
          << " > " << saida << " 2>&1";
    return std::system(linha.str().c_str()) == 0 ? 0 : 1;
#endif
}

static std::string problema(int codigo) {
    if (codigo == TEMPO_ESGOTADO) {
        return "passou do tempo limite (laco infinito?)";
    }
    if (codigo == SAIDA_GRANDE) {
        return "escreveu mais de 1 MB (laco infinito?)";
    }
    if (codigo == NAO_EXECUTOU) {
        return "nao conseguiu executar";
    }
    std::ostringstream texto;
    texto << "codigo de saida " << codigo;
    return texto.str();
}

// Mostra ate "maximo" linhas de erro/aviso do compilador; o texto completo esta em "log".
static void mostrarCompilacao(const std::string& texto, std::string::size_type maximo, const std::string& log) {
    std::vector<std::string> mensagens;
    std::vector<std::string> todas = linhas(texto);
    for (std::string::size_type i = 0; i < todas.size(); i++) {
        if (todas[i].find("error") != std::string::npos || todas[i].find("warning") != std::string::npos ||
            todas[i].find("undefined reference") != std::string::npos) {
            mensagens.push_back(todas[i]);
        }
    }
    if (mensagens.empty()) {
        mensagens = todas;
    }
    for (std::string::size_type i = 0; i < mensagens.size() && i < maximo; i++) {
        std::cout << "        " << mensagens[i] << std::endl;
    }
    if (mensagens.size() > maximo) {
        std::cout << "        ... (completo em " << log << ")" << std::endl;
    }
}

static bool compilar(const std::string& argumentos, const std::string& log) {
    return executar(GXX + argumentos, "", log, 120) == 0;
}

static void relatar(bool passou, const std::string& nome, int acertos, int casos, const std::string& observacao) {
    ok += acertos;
    total += casos;
    if (!passou) {
        falhou = true;
    }
    std::string espaco(nome.size() < 26 ? 26 - nome.size() : 1, ' ');
    std::cout << (passou ? "PASS  " : "FAIL  ") << nome << espaco << acertos << "/" << casos
              << (observacao.empty() ? "" : "  " + observacao) << std::endl;
}

// Alguma linha comeca com "class Nome" (sem diferenciar maiusculas)? "class Nome;" nao conta.
static bool temClasse(const std::string& conteudo, const std::string& nome) {
    std::vector<std::string> todas = linhas(minusculas(conteudo));
    std::string procurado = "class " + nome;
    for (std::string::size_type i = 0; i < todas.size(); i++) {
        std::string resto = todas[i].compare(0, procurado.size(), procurado) == 0 ? todas[i].substr(procurado.size()) : "x";
        std::string::size_type p = resto.find_first_not_of(' ');
        if (resto.empty() || ((resto[0] == ' ' || resto[0] == '{' || resto[0] == ':') && (p == std::string::npos || resto[p] != ';'))) {
            return true;
        }
    }
    return false;
}

static void prepararStubs() {
    // Limpa as copias da rodada anterior: um stub apagado de testes/stubs nao pode
    // continuar em testes/bin/stubs escondendo a classe real.
    std::vector<std::string> antigos = listar(BIN + "stubs", "", ".hpp");
    for (std::string::size_type i = 0; i < antigos.size(); i++) {
        std::remove((BIN + "stubs/" + antigos[i] + ".hpp").c_str());
    }
    std::string ativos;
    std::vector<std::string> stubs = listar("testes/stubs", "", ".hpp");
    for (std::string::size_type i = 0; i < stubs.size(); i++) {
        std::string copia = BIN + "stubs/" + stubs[i] + ".hpp";
        if (temClasse(ler("headers/" + stubs[i] + ".hpp"), stubs[i])) {
            std::cout << "AVISO stub desligado: headers/" << stubs[i] << ".hpp ja tem a classe real."
                      << " Apague testes/stubs/" << stubs[i] << ".hpp" << std::endl;
        } else {
            std::ofstream destino(copia.c_str(), std::ios::binary);
            destino << ler("testes/stubs/" + stubs[i] + ".hpp");
            ativos += " " + stubs[i];
        }
    }
    if (!ativos.empty()) {
        std::cout << "Stubs em uso (headers/ ainda sem a classe):" << ativos << std::endl;
    }
}

static void compilarProducao() {
    std::string objetos;
    std::vector<std::string> fontes = listar("sources", "", ".cpp");
    for (std::string::size_type i = 0; i < fontes.size(); i++) {
        std::string objeto = BIN + "obj/" + fontes[i] + ".o";
        std::string log = BIN + "obj/" + fontes[i] + ".txt";
        std::remove(objeto.c_str());
        if (compilar("-c sources/" + fontes[i] + ".cpp -o " + objeto, log)) {
            objetos += " " + objeto;
            if (!ler(log).empty()) {
                std::cout << "AVISO sources/" << fontes[i] << ".cpp compila com avisos:" << std::endl;
                mostrarCompilacao(ler(log), 4, log);
            }
        } else {
            std::cout << "AVISO sources/" << fontes[i] << ".cpp nao compila (fica fora dos testes):" << std::endl;
            mostrarCompilacao(ler(log), 1, log);
        }
    }
    std::remove(BIBLIOTECA.c_str());
    executar("ar rcs " + BIBLIOTECA + objetos, "", BIN + "obj/ar.txt", 60);
}

static void rodarTeste(const std::string& nome) {
    std::string rotulo = nome.substr(6);
    std::string executavel = BIN + nome + ".exe";
    std::string log = BIN + nome + ".txt";
    std::string logCompilacao = BIN + nome + ".compilacao.txt";
    if (!compilar("testes/" + nome + ".cpp " + BIBLIOTECA + " -o " + executavel, logCompilacao)) {
        relatar(false, rotulo, 0, 1, "nao compilou:");
        mostrarCompilacao(ler(logCompilacao), 8, logCompilacao);
        return;
    }
    std::string avisos = ler(logCompilacao);
    int codigo = executar(executavel, "", log, 10);
    std::vector<std::string> saida = linhas(ler(log));
    std::vector<std::string> falhas;
    int acertos = 0;
    int casos = 0;
    bool achou = false;
    for (std::string::size_type i = 0; i < saida.size(); i++) {
        std::string::size_type p = saida[i].find("RESULTADO ");
        if (p != std::string::npos) {
            achou = std::sscanf(saida[i].c_str() + p, "RESULTADO %d/%d", &acertos, &casos) == 2;
        } else if (saida[i].compare(0, 6, "FAIL  ") == 0) {
            falhas.push_back(saida[i].substr(6));
        }
    }
    if (!achou) {
        relatar(false, rotulo, 0, 1, "terminou sem RESULTADO: " + problema(codigo));
    } else {
        bool explicado = codigo == 0 || (codigo == 1 && acertos < casos);
        relatar(codigo == 0 && acertos == casos, rotulo, acertos, casos, explicado ? "" : problema(codigo));
    }
    for (std::string::size_type i = 0; i < falhas.size() && i < 15; i++) {
        std::cout << "      - " << falhas[i] << std::endl;
    }
    if (falhas.size() > 15 || (!achou && falhas.empty())) {
        std::cout << "      (saida completa em " << log << ")" << std::endl;
    }
    if (!avisos.empty()) {
        std::cout << "      avisos de compilacao:" << std::endl;
        mostrarCompilacao(avisos, 4, logCompilacao);
    }
}

// Devolve "" se a saida bate com o .esperado; senao, o primeiro problema encontrado.
// "> trecho" deve aparecer (em ordem, sem diferenciar maiusculas); "! trecho" nao pode aparecer.
static std::string conferirRoteiro(const std::string& saidaBruta, const std::vector<std::string>& regras, int codigo) {
    if (codigo != 0) {
        return problema(codigo);
    }
    std::string saida = minusculas(saidaBruta);
    std::string::size_type posicao = 0;
    for (std::string::size_type i = 0; i < regras.size(); i++) {
        std::string trecho = regras[i].size() > 2 ? regras[i].substr(2) : "";
        if (regras[i].compare(0, 2, "> ") == 0) {
            posicao = saida.find(minusculas(trecho), posicao);
            if (posicao == std::string::npos) {
                return "faltou (em ordem): \"" + trecho + "\"";
            }
            posicao += trecho.size();
        } else if (regras[i].compare(0, 2, "! ") == 0 && saida.find(minusculas(trecho)) != std::string::npos) {
            return "nao devia aparecer: \"" + trecho + "\"";
        }
    }
    return "";
}

static void rodarRoteiros(const std::string& programa, const std::string& principal) {
    std::string executavel = BIN + programa + ".exe";
    std::string rotulo = "roteiros " + programa + ".exe";
    if (!compilar(principal + " " + BIBLIOTECA + " -o " + executavel, BIN + programa + ".txt")) {
        relatar(false, rotulo, 0, 1, "nao compilou (" + principal + "):");
        mostrarCompilacao(ler(BIN + programa + ".txt"), 8, BIN + programa + ".txt");
        return;
    }
    std::vector<std::string> roteiros = listar("testes/roteiros", "", ".txt");
    std::vector<std::string> falhas;
    int casos = 0;
    for (std::string::size_type i = 0; i < roteiros.size(); i++) {
        std::string base = "testes/roteiros/" + roteiros[i];
        std::vector<std::string> regras = linhas(ler(base + ".esperado"));
        if (std::find(regras.begin(), regras.end(), "@apenas_teste") != regras.end() && programa != "teste") {
            continue;
        }
        casos++;
        std::string saida = BIN + "roteiros/" + programa + "_" + roteiros[i] + ".txt";
        std::string erro = existe(base + ".esperado") ? "" : "falta o arquivo " + base + ".esperado";
        if (erro.empty()) {
            int codigo = executar(executavel, base + ".txt", saida, 5);
            erro = conferirRoteiro(ler(saida), regras, codigo);
        }
        if (!erro.empty()) {
            falhas.push_back(roteiros[i] + ": " + erro + "  (saida em " + saida + ")");
        }
    }
    relatar(falhas.empty(), rotulo, casos - static_cast<int>(falhas.size()), casos, "");
    for (std::string::size_type i = 0; i < falhas.size(); i++) {
        std::cout << "      - " << falhas[i] << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::string so = argc > 1 ? argv[1] : "";
    criarPasta(BIN + "stubs");
    criarPasta(BIN + "obj");
    criarPasta(BIN + "roteiros");
    prepararStubs();
    compilarProducao();

    bool achou = false;
    std::vector<std::string> testes = listar("testes", "teste_", ".cpp");
    for (std::string::size_type i = 0; i < testes.size(); i++) {
        if (so.empty() || testes[i] == "teste_" + so) {
            rodarTeste(testes[i]);
            achou = true;
        }
    }
    if (so.empty() || so == "roteiros") {
        rodarRoteiros("teste", "testes/main_teste.cpp");
        rodarRoteiros("kanban", "main.cpp");
        achou = true;
    }
    if (!achou) {
        std::cout << "FAIL  nao existe testes/teste_" << so << ".cpp" << std::endl;
        falhou = true;
    }
    std::cout << std::endl << "TOTAL " << ok << "/" << total << " verificacoes  ->  "
              << (falhou ? "HA FALHAS (procure FAIL acima)" : "TUDO PASSOU") << std::endl;
    return falhou ? 1 : 0;
}
