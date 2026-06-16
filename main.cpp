#include <iostream>
#include <fstream>
#include <string>
#include "graph.cpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2)
        throw invalid_argument("Entre apenas o caminho do arquivo. Ex: ./programa /home/user/pasta/arquivo");

    string caminhoArquivo = argv[1];
    ifstream traceroute(caminhoArquivo);
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
    g.printarMenorCaminho("217.85.238.118", "62.155.246.229");
    g.draw();
    return 0;
}