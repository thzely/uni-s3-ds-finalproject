#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "graph.cpp"

using namespace std;

void menuFormato(graph::digraph& g, const string& nomeBase, const string& from = "", const string& to = "") {
    int formato;
    cout << "\nSelecione o formato de saída do Graphviz: " << endl;
    cout << "1. Tela" << endl;
    cout << "2. Imagem (PNG)" << endl;
    cout << "3. Documento (PDF)" << endl;
    cout << "Opção: ";
    cin >> formato;
    switch (formato) {
        case 1:
            if (from.empty()) g.draw();
            else g.drawShortestPath(from, to);
            break;
        case 2:
            if (from.empty()) g.generatePNG(nomeBase);
            else g.generatePNGShortestPath(from, to, nomeBase);
            break;
        case 3:
            if (from.empty()) g.generatePDF(nomeBase);
            else g.generatePDFShortestPath(from, to, nomeBase);
            break;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2)
        throw invalid_argument("Entre apenas o caminho do arquivo. Ex: ./programa /home/user/pasta/arquivo");

    string caminhoArquivo = argv[1];
    ifstream traceroute(caminhoArquivo);
    string nomeBase = filesystem::path(caminhoArquivo).filename().string();
    if (!traceroute) {
        cerr << "Falha ao abrir o arquivo: " << caminhoArquivo << "\n";
        return 1;
    }

    graph::digraph g;

    auto getColuna = [](const string& linha, int colAlvo) -> string {
        int coluna = 0, inicio = 0;
        for (int i = 0; i <= (int)linha.size(); i++) {
            if (i == (int)linha.size() || linha[i] == ',') {
                if (coluna == colAlvo)
                    return linha.substr(inicio, i - inicio);
                inicio = i + 1;
                coluna++;
            }
        }
        return "";
    };

    string linha;
    bool primeiraLinha = true;

    while (getline(traceroute, linha)) {
        if (primeiraLinha) {
            primeiraLinha = false;
            continue;
        }

        string hop_from = getColuna(linha, 4);
        string hop_to   = getColuna(linha, 5);


        if (hop_from.empty() || hop_to.empty() || hop_to == "*")
            continue;

        if (!g.find(hop_from))
            g.insert_nodo(hop_from);
        if (!g.find(hop_to))
            g.insert_nodo(hop_to);

        g.insert_link(hop_from, hop_to);
    }

    traceroute.close();
    cout << "Grafo carregado com sucesso!\n";
    cout << "Vertices unicos : " << g.size()  << "\n";
    cout << "Arestas inseridas: " << g.edges() << "\n";

    int opcao;
    do {
        cout << "\n------ MENU ------\n";
        cout << "1. Exibir grafo completo\n";
        cout << "2. Encontrar menor caminho\n";
        cout << "3. Calcular o diâmetro do grafo\n";
        cout << "4. Identificar roteadores críticos\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;

        if(opcao == 1) {
            cout << "\nExibindo grafo:\n";
            menuFormato(g, nomeBase);
        } 
        else if (opcao == 2) {
            string from, to;
            cout << "\nDigite o IP de origem: "; 
            cin >> from;
            cout << "Digite o IP de destino: "; 
            cin >> to;
            cout << "\nCaminho: ";
            g.printarMenorCaminho(from, to);

            menuFormato(g, nomeBase, from, to);
        } 
        else if (opcao == 3) {
            int diam = g.diameter();
            cout << "O diâmetro do grafo é: " << diam << "\n";
        } 
        else if (opcao == 4) {
            g.getCriticalRouters();
        } 
        else if (opcao == 0) {
            cout << "\nPrograma encerrado\n";
        } 
        else {
            cout << "\nOpção inválida! Tente novamente.\n";
        }
    } while (opcao != 0);

    return 0;
}