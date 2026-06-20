# Analisador de Rotas de Rede com Grafos
Ferramentas como o traceroute mapeiam sequencialmente os roteadores pelos quais um pacote trafega até alcançar um destino final. Esse conjunto de dados pode ser modelado como um grafo direcionado, onde cada roteador com IP válido é um vértice e cada enlace de comunicação direta é uma aresta.

Este projeto lê logs de rede, constrói essa topologia dinamicamente em memória e aplica algoritmos para responder a consultas de rota e análise de topologia.

## Integrantes
- Marianne Fisch
- Henrique Tonello Zotti
- Izadora Freb da Silva

## Compilação
```bash
g++ -std=c++17 main.cpp -o programa
```
 O `-std=c++17` é necessário pois utilizamos `std::filesystem` para extrair automaticamente o nome base do arquivo de entrada.

## Execução

```bash
./programa nome_arquivo_entrada.log
```

## Funcionalidades

- Exibir grafo completo (tela, PNG ou PDF)
- Encontrar e exibir menor caminho entre dois IPs com destaque visual e número de saltos
- Calcular o diâmetro do grafo
- Identificar Top 5 roteadores críticos por in-degree

## Decisões de implementação

- **Lista de adjacência** com `unordered_map` para armazenamento eficiente dos vértices e acesso O(1) por IP
- **BFS** para encontrar o menor caminho em número de saltos, garantindo o caminho mínimo em grafos não ponderados
- **Graphviz** para geração visual do grafo, com destaque em roxo/azul para o caminho encontrado
- Sanitização dos dados descarta entradas com `*`, campos vazios e arestas duplicadas