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

    int opcao;
    do {
        cout << "\n------ MENU ------\n";
        cout << "1. Exibir grafo completo\n";
        cout << "2. Encontrar menor caminho\n";
        cout << "3. Calcular o diâmetro do grafo\n";
        cout << "4. Identificar roteadores críticos\n";
        cout << "5. Gerar PDF do grafo\n";
        cout << "6. Gerar PNG do grafo\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;

        if(opcao == 1) {
            cout << "\nExibindo grafo:\n";
            g.draw();
        } 
        else if (opcao == 2) {
            string from, to;
            cout << "\nDigite o IP de origem: "; 
            cin >> from;
            cout << "Digite o IP de destino: "; 
            cin >> to;
            cout << "\nCaminho: ";
            g.printarMenorCaminho(from, to);
        } 
        else if (opcao == 3) {
            int diam = g.diameter();
            cout << "O diâmetro do grafo é: " << diam << "\n";
        } 
        else if (opcao == 4) {
            g.getCriticalRouters();
        } 
        else if (opcao == 5) {
            string nomeArquivo;
            cout << "\nDigite o nome do arquivo: ";
            cin >> nomeArquivo;
            g.generatePDF(nomeArquivo);
        } 
        else if (opcao == 6) {
            string nomeArquivo;
            cout << "\nDigite o nome do arquivo: ";
            cin >> nomeArquivo;
            g.generatePNG(nomeArquivo);
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