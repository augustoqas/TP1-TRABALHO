// Roda todos os testes do TP1. Quem chama e testes/rodar.bat (cmd) ou testes/rodar.sh
// (Git Bash), sempre a partir da raiz do repositorio. Uso: rodar [nome]
// (ex.: "rodar senha", "rodar Senha" ou "rodar teste_senha.cpp" rodam so testes/teste_senha.cpp;
// "rodar roteiros" roda so os roteiros).
//
// 1. Stubs: testes/stubs/X.hpp e copiado para testes/bin/stubs enquanto headers/X.hpp nao
//    existe ou esta em branco. Se headers/X.hpp tem qualquer conteudo, o stub sai de cena.
// 2. Producao: compila cada sources/*.cpp (quem nao compila vira AVISO) e junta
//    tudo em testes/bin/tp1.a. Cada teste usa so o que precisa dessa biblioteca.
// 3. Unidade: compila cada testes/teste_*.cpp com tp1.a, roda e le "RESULTADO ok/total".
// 4. Roteiros: compila testes/main_teste.cpp (teste.exe) e main.cpp (kanban.exe) e
//    confere cada testes/roteiros/NN.txt (entrada) com NN.esperado.
// 5. Build real: compila main.cpp + sources/*.cpp sem stubs, como o Code::Blocks faria.
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

// Os stubs vem antes de headers/ no -I, mas isso so vale para #include "x.hpp" escrito
// fora de headers/: dentro de headers/ o g++ procura primeiro na propria pasta. Por isso
// o stub de X so fica ativo enquanto headers/X.hpp nao existe ou esta em branco.
static const std::string FLAGS = "g++ -std=c++11 -Wall -Wextra -pedantic ";
static const std::string GXX = FLAGS + "-Itestes/bin/stubs -Iheaders ";
static const std::string GXX_REAL = FLAGS + "-Iheaders ";
static const std::string BIN = "testes/bin/";
static const std::string BIBLIOTECA = "testes/bin/tp1.a";
static const std::string MAIN_TESTE = "testes/main_teste.cpp";
static const int TEMPO_ESGOTADO = -1000;
static const int SAIDA_GRANDE = -1001;
static const int NAO_EXECUTOU = -1002;
static const unsigned long LIMITE_SAIDA = 1024 * 1024;

static int total = 0;
static int ok = 0;
static bool falhou = false;
static int avisos = 0;
static bool stubEmUso = false;
static bool fonteQuebrada = false;
static bool buildRealQuebrado = false;

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

static const char* const BRANCOS = " \t\r\n\f\v";

static std::string aparar(const std::string& texto) {
    std::string::size_type inicio = texto.find_first_not_of(BRANCOS);
    if (inicio == std::string::npos) {
        return "";
    }
    return texto.substr(inicio, texto.find_last_not_of(BRANCOS) - inicio + 1);
}

static bool comeca(const std::string& texto, const std::string& prefixo) {
    return texto.compare(0, prefixo.size(), prefixo) == 0;
}

static bool termina(const std::string& texto, const std::string& sufixo) {
    return texto.size() >= sufixo.size() && texto.compare(texto.size() - sufixo.size(), sufixo.size(), sufixo) == 0;
}

static void aviso(const std::string& texto) {
    avisos++;
    std::cout << "AVISO " << texto << std::endl;
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
        if (nome.size() > prefixo.size() + sufixo.size() && comeca(nome, prefixo) && termina(nome, sufixo)) {
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
    if (in == INVALID_HANDLE_VALUE || out == INVALID_HANDLE_VALUE) {
        if (in != INVALID_HANDLE_VALUE) {
            CloseHandle(in);
        }
        if (out != INVALID_HANDLE_VALUE) {
            CloseHandle(out);
        }
        return NAO_EXECUTOU;
    }
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
        return "nao conseguiu executar (ou abrir a entrada/saida)";
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

static std::string primeiroErro(const std::string& texto) {
    std::vector<std::string> todas = linhas(texto);
    for (std::string::size_type i = 0; i < todas.size(); i++) {
        if (todas[i].find("error") != std::string::npos || todas[i].find("undefined reference") != std::string::npos) {
            return todas[i];
        }
    }
    return todas.empty() ? "" : todas[0];
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
    std::string espaco(nome.size() < 32 ? 32 - nome.size() : 1, ' ');
    std::cout << (passou ? "PASS  " : "FAIL  ") << nome << espaco << acertos << "/" << casos
              << (observacao.empty() ? "" : "  " + observacao) << std::endl;
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
        if (!aparar(ler("headers/" + stubs[i] + ".hpp")).empty()) {
            aviso("stub desligado: headers/" + stubs[i] + ".hpp nao esta vazio, entao a classe real e usada."
                  " Apague testes/stubs/" + stubs[i] + ".hpp");
        } else {
            std::ofstream destino(copia.c_str(), std::ios::binary);
            destino << ler("testes/stubs/" + stubs[i] + ".hpp");
            ativos += " " + stubs[i];
        }
    }
    if (!ativos.empty()) {
        stubEmUso = true;
        std::cout << "Stubs em uso (headers/X.hpp ainda vazio ou ausente):" << ativos << std::endl;
    }
}

// Devolve false se nao conseguiu montar a biblioteca (ai nenhum teste consegue ligar).
static bool compilarProducao() {
    std::string objetos;
    std::vector<std::string> fontes = listar("sources", "", ".cpp");
    for (std::string::size_type i = 0; i < fontes.size(); i++) {
        std::string objeto = BIN + "obj/" + fontes[i] + ".o";
        std::string log = BIN + "obj/" + fontes[i] + ".txt";
        std::remove(objeto.c_str());
        if (compilar("-c sources/" + fontes[i] + ".cpp -o " + objeto, log)) {
            objetos += " " + objeto;
            if (!ler(log).empty()) {
                aviso("sources/" + fontes[i] + ".cpp compila com avisos:");
                mostrarCompilacao(ler(log), 4, log);
            }
        } else {
            fonteQuebrada = true;
            aviso("sources/" + fontes[i] + ".cpp nao compila (fica fora dos testes):");
            mostrarCompilacao(ler(log), 1, log);
        }
    }
    std::string log = BIN + "obj/ar.txt";
    std::remove(BIBLIOTECA.c_str());
    int codigo = executar("ar rcs " + BIBLIOTECA + objetos, "", log, 60);
    if (codigo != 0) {
        relatar(false, "biblioteca tp1.a", 0, 1, "ar falhou (" + problema(codigo) + "):");
        mostrarCompilacao(ler(log), 4, log);
        return false;
    }
    return true;
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
    std::string avisosCompilacao = ler(logCompilacao);
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
        } else if (comeca(saida[i], "FAIL  ")) {
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
    if (!avisosCompilacao.empty()) {
        std::cout << "      avisos de compilacao:" << std::endl;
        mostrarCompilacao(avisosCompilacao, 4, logCompilacao);
    }
}

// Conteudo de um NN.esperado. "erro" fica preenchido se o proprio arquivo tem problema.
struct Esperado {
    std::vector<std::string> regras;
    bool apenasTeste;
    std::string erro;
};

// Linhas validas (depois de aparadas): vazia, "# ...", "> trecho", "! trecho",
// "@apenas_teste" e "@sem_trechos". Qualquer outra e erro, para nada passar em silencio.
static Esperado lerEsperado(const std::string& caminho) {
    Esperado esperado;
    esperado.apenasTeste = false;
    if (!existe(caminho)) {
        esperado.erro = "falta o arquivo " + caminho;
        return esperado;
    }
    bool temTrecho = false;
    bool semTrechos = false;
    std::vector<std::string> todas = linhas(ler(caminho));
    for (std::string::size_type i = 0; i < todas.size(); i++) {
        std::string linha = aparar(todas[i]);
        if (linha.empty() || linha[0] == '#') {
            continue;
        }
        if (linha == "@apenas_teste") {
            esperado.apenasTeste = true;
        } else if (linha == "@sem_trechos") {
            semTrechos = true;
        } else if (linha.size() > 2 && (comeca(linha, "> ") || comeca(linha, "! "))) {
            // Guarda sem os espacos da esquerda, mas com os da direita (fazem parte do trecho).
            esperado.regras.push_back(todas[i].substr(todas[i].find_first_not_of(BRANCOS)));
            temTrecho = temTrecho || linha[0] == '>';
        } else if (esperado.erro.empty()) {
            esperado.erro = "linha invalida no .esperado: " + linha;
        }
    }
    if (esperado.erro.empty() && !temTrecho && !semTrechos) {
        esperado.erro = "nenhum trecho \"> ...\" no .esperado (se for de proposito, ponha uma linha @sem_trechos)";
    }
    return esperado;
}

// Devolve "" se a saida bate com as regras; senao, o primeiro problema encontrado.
// "> trecho" deve aparecer (em ordem, sem diferenciar maiusculas); "! trecho" nao pode aparecer.
static std::string conferirRoteiro(const std::string& saidaBruta, const std::vector<std::string>& regras, int codigo) {
    if (codigo != 0) {
        return problema(codigo);
    }
    std::string saida = minusculas(saidaBruta);
    std::string::size_type posicao = 0;
    for (std::string::size_type i = 0; i < regras.size(); i++) {
        std::string trecho = regras[i].substr(2);
        if (regras[i][0] == '>') {
            posicao = saida.find(minusculas(trecho), posicao);
            if (posicao == std::string::npos) {
                return "faltou (em ordem): \"" + trecho + "\"";
            }
            posicao += trecho.size();
        } else if (saida.find(minusculas(trecho)) != std::string::npos) {
            return "nao devia aparecer: \"" + trecho + "\"";
        }
    }
    return "";
}

static void rodarRoteiros(const std::string& programa, const std::string& principal, const std::string& rotulo) {
    std::string executavel = BIN + programa + ".exe";
    if (!compilar(principal + " " + BIBLIOTECA + " -o " + executavel, BIN + programa + ".txt")) {
        relatar(false, rotulo, 0, 1, "nao compilou (" + principal + "):");
        mostrarCompilacao(ler(BIN + programa + ".txt"), 8, BIN + programa + ".txt");
        return;
    }
    bool ehTeste = programa == "teste";
    bool temMainTeste = existe(MAIN_TESTE);
    std::vector<std::string> roteiros = listar("testes/roteiros", "", ".txt");
    std::vector<std::string> falhas;
    int casos = 0;
    for (std::string::size_type i = 0; i < roteiros.size(); i++) {
        std::string base = "testes/roteiros/" + roteiros[i];
        Esperado esperado = lerEsperado(base + ".esperado");
        if (esperado.apenasTeste && !ehTeste && temMainTeste) {
            continue;
        }
        casos++;
        std::string saida = BIN + "roteiros/" + programa + "_" + roteiros[i] + ".txt";
        std::string erro = esperado.erro;
        if (erro.empty() && esperado.apenasTeste && !ehTeste) {
            erro = "marcado @apenas_teste, mas " + MAIN_TESTE + " nao existe (tire a marca ou o roteiro)";
        }
        if (erro.empty()) {
            int codigo = executar(executavel, base + ".txt", saida, 5);
            erro = conferirRoteiro(ler(saida), esperado.regras, codigo);
            if (!erro.empty()) {
                erro += "  (saida em " + saida + ")";
            }
        }
        if (!erro.empty()) {
            falhas.push_back(roteiros[i] + ": " + erro);
        }
    }
    relatar(falhas.empty(), rotulo, casos - static_cast<int>(falhas.size()), casos, "");
    for (std::string::size_type i = 0; i < falhas.size(); i++) {
        std::cout << "      - " << falhas[i] << std::endl;
    }
}

struct Linha {
    int numero;
    std::string texto;
};

// Linhas que contam na comparacao entre main.cpp e testes/main_teste.cpp: aparadas, sem as
// em branco, sem o comentario de cabecalho do arquivo e sem os blocos
// "// INICIO DADOS DE TESTE" ... "// FIM DADOS DE TESTE".
static std::vector<Linha> semDadosDeTeste(const std::string& texto, bool& blocoAberto) {
    std::vector<Linha> resultado;
    std::vector<std::string> todas = linhas(texto);
    bool cabecalho = true;
    bool emComentario = false;
    blocoAberto = false;
    for (std::string::size_type i = 0; i < todas.size(); i++) {
        std::string linha = aparar(todas[i]);
        if (linha.empty()) {
            continue;
        }
        if (cabecalho) {
            if (emComentario) {
                emComentario = linha.find("*/") == std::string::npos;
                continue;
            }
            if (comeca(linha, "/*")) {
                emComentario = linha.find("*/", 2) == std::string::npos;
                continue;
            }
            if (comeca(linha, "//")) {
                continue;
            }
            cabecalho = false;
        }
        if (linha == "// INICIO DADOS DE TESTE") {
            blocoAberto = true;
        } else if (linha == "// FIM DADOS DE TESTE") {
            blocoAberto = false;
        } else if (!blocoAberto) {
            Linha item = {static_cast<int>(i) + 1, linha};
            resultado.push_back(item);
        }
    }
    return resultado;
}

static std::string descrever(const std::string& arquivo, const std::vector<Linha>& lista, std::string::size_type i) {
    if (i >= lista.size()) {
        return arquivo + ": (acabou)";
    }
    std::ostringstream texto;
    texto << arquivo << ":" << lista[i].numero << ": " << lista[i].texto;
    return texto.str();
}

static void conferirMainTeste() {
    bool abertoPrincipal = false;
    bool abertoTeste = false;
    std::vector<Linha> principal = semDadosDeTeste(ler("main.cpp"), abertoPrincipal);
    std::vector<Linha> teste = semDadosDeTeste(ler(MAIN_TESTE), abertoTeste);
    if (abertoTeste) {
        aviso(MAIN_TESTE + " tem \"// INICIO DADOS DE TESTE\" sem o \"// FIM DADOS DE TESTE\" correspondente.");
    }
    std::string::size_type i = 0;
    while (i < principal.size() && i < teste.size() && principal[i].texto == teste[i].texto) {
        i++;
    }
    if (i < principal.size() || i < teste.size()) {
        aviso(MAIN_TESTE + " difere do main.cpp fora dos blocos DADOS DE TESTE."
              " Atualize o " + MAIN_TESTE + " (primeira diferenca):");
        std::cout << "        " << descrever("main.cpp", principal, i) << std::endl;
        std::cout << "        " << descrever(MAIN_TESTE, teste, i) << std::endl;
    }
}

// Compila como o Code::Blocks: main.cpp + todos os sources/*.cpp, sem stubs.
static void buildReal() {
    std::string rotulo = "build real (sem stubs)";
    std::string fontes;
    std::vector<std::string> nomes = listar("sources", "", ".cpp");
    for (std::string::size_type i = 0; i < nomes.size(); i++) {
        fontes += " sources/" + nomes[i] + ".cpp";
    }
    std::string log = BIN + "build_real.txt";
    int codigo = executar(GXX_REAL + "main.cpp" + fontes + " -o " + BIN + "build_real.exe", "", log, 300);
    if (codigo == 0) {
        relatar(true, rotulo, 1, 1, ler(log).empty() ? "" : "com avisos (ver " + log + ")");
    } else if (stubEmUso) {
        buildRealQuebrado = true;
        aviso(rotulo + " ainda nao compila; vira FAIL quando nao houver stub em uso. Primeiro erro:");
        std::cout << "        " << (codigo < 0 ? problema(codigo) : primeiroErro(ler(log))) << std::endl;
        std::cout << "        (completo em " << log << ")" << std::endl;
    } else {
        relatar(false, rotulo, 0, 1, "nao compila:");
        mostrarCompilacao(ler(log), 8, log);
    }
}

// Aceita "senha", "Senha", "teste_senha", "teste_senha.cpp" e "testes\teste_senha.cpp".
static std::string normalizarFiltro(std::string filtro) {
    filtro = minusculas(filtro);
    std::replace(filtro.begin(), filtro.end(), '\\', '/');
    if (comeca(filtro, "testes/")) {
        filtro = filtro.substr(7);
    }
    if (comeca(filtro, "teste_")) {
        filtro = filtro.substr(6);
    }
    if (termina(filtro, ".cpp")) {
        filtro = filtro.substr(0, filtro.size() - 4);
    }
    return filtro;
}

static std::string conclusao() {
    if (falhou) {
        return "HA FALHAS (procure FAIL acima)";
    }
    if (buildRealQuebrado) {
        return "testes passaram, MAS o build real ainda nao compila (ver AVISOS)";
    }
    if (fonteQuebrada) {
        return "testes passaram, MAS ha fontes em sources/ que nao compilam (ver AVISOS)";
    }
    return avisos > 0 ? "TUDO PASSOU (ha AVISOS acima)" : "TUDO PASSOU";
}

int main(int argc, char* argv[]) {
    std::string so = argc > 1 ? normalizarFiltro(argv[1]) : "";
    criarPasta(BIN + "stubs");
    criarPasta(BIN + "obj");
    criarPasta(BIN + "roteiros");
    prepararStubs();
    if (compilarProducao()) {
        bool achou = false;
        std::vector<std::string> testes = listar("testes", "teste_", ".cpp");
        for (std::string::size_type i = 0; i < testes.size(); i++) {
            if (so.empty() || minusculas(testes[i]) == "teste_" + so) {
                rodarTeste(testes[i]);
                achou = true;
            }
        }
        if (so.empty() || so == "roteiros") {
            if (existe(MAIN_TESTE)) {
                conferirMainTeste();
                rodarRoteiros("teste", MAIN_TESTE, "roteiros teste.exe");
            }
            rodarRoteiros("kanban", "main.cpp", stubEmUso ? "roteiros kanban.exe (com stubs)" : "roteiros kanban.exe");
            achou = true;
        }
        if (!achou) {
            std::cout << "FAIL  nao existe testes/teste_" << so << ".cpp" << std::endl;
            falhou = true;
        }
        if (so.empty()) {
            buildReal();
        }
    }
    std::cout << std::endl << "TOTAL " << ok << "/" << total << " verificacoes  ->  " << conclusao() << std::endl;
    return falhou ? 1 : 0;
}
