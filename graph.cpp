/// biblioteca para grafos
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include<queue>


namespace graph{

    class digraph{
        private:
          // nodos do grafo - VERTÍCES
          struct node{
            std::string value; // valor
            std::vector<node*> links; //lista de ponteiros p/ os nodos vizinhos - ARESTAS
          };

          //tabela hash, que mapeia o rótulo do nó para o objeto node
                             //<chave>, <valor>    
          std::unordered_map<std::string,node> nodes;
        
          std::unordered_set<node*>visited;
        public:
         //Insere um novo nó no grafo com o rótulo s
          void insert_nodo(const std::string& s){
            node aux;
            aux.value = s;
            nodes[s] = aux;
          }

          //retorna a quantidade de nós (vertices) do grafo
          size_t size(){
            return nodes.size();
          }

          //busca um nó pelo seu rótulo e retorna o endereço do nodo 
          node* find(const std::string& s){
            auto it = nodes.find(s); //Este find é do unordered_map
            return it==nodes.end() ?  nullptr : &it->second;
          }

          //INsere um aresta dirigida de 'from' para 'to'
          bool insert_link(const std::string& from, const std::string& to){
            auto pfrom = find(from);
            if(!pfrom) return false;

            auto pto = find(to);
            if(!pto) return false;

            auto it = std::find(
                pfrom->links.begin(),
                pfrom->links.end(),
                pto
            );

            if(it != pfrom->links.end())
                return false; // aresta já existe

            pfrom->links.push_back(pto);
            return true;
        }

          //Numero de arestas que chegam a um vertice
          size_t indegree(const std::string& s){
            auto p = find(s);
            if(p == nullptr){
              return 0;
            }
            size_t contador = 0;
            for(auto nd : nodes){
               for (auto v : nd.second.links){
                    if(s == v->value){
                      contador ++;
                    }
               }
            }
            return  contador;

          }
          //Numero de arestas que saem de um vertice
          size_t outdegree(const std::string& s){
            auto p = find(s);
            return p == nullptr ? 0 : p->links.size();
            //mesma coisa
            //if(p==nullptr){
              //return 0;
            //}else{
              //return p->links.size();
            //}
          }
          //Numero total de arestas conectadas a um vertice
          size_t degree(const std::string& s){
            auto p = find(s);
            if(p == nullptr){
              return 0;
            }
            return indegree(s) + outdegree(s);

          }
      



          void export2dot(const std::string& filename){
            std::ofstream dot(filename);
            dot << "digraph{\n";

            for (auto nd : nodes){
                dot << "\t\"" << nd.first << "\"";   // <-- aspas no nó
                if(nd.second.links.size() > 0){
                    dot << " -> {";
                    for (auto vizinho : nd.second.links){
                        dot << "\"" << vizinho->value << "\" ";  // <-- aspas nos vizinhos
                    }
                    dot << "}";
                }
                dot << ";\n";
            }
            dot << "}\n";
        }      
          void draw(){
            export2dot("graphED2.dot");
            // std::system("dot -Tpng graphED2.dot -o graphED2.png");
            std::system("dot -Tx11 graphED2.dot");

          }

          void removeLink(const std::string& from, const std::string& to){
            auto pfrom = find(from);
            if(!pfrom) return;
            auto pto = find(to);
            if(!pto) return;

            auto it = std::find(pfrom->links.begin(), pfrom->links.end(), pto);
            if(it==pfrom->links.end()) return;

            pfrom->links.erase(it);
          }

          void removeNode(const std::string& key){
            auto n = find(key);
            if(!n) return;

           for(auto &[k,nd] : nodes){
            auto it = std::find(nd.links.begin(), nd.links.end(), n);
            if(it != nd.links.end())
              nd.links.erase(it);
           }
           nodes.erase(key);
          }

          void recursive_DFS(node* p, int level=0){
            if(visited.count(p)>0){
              return;
            }
            std::cout << std::string(level, '\t')<< p->value << '\n';
            visited.insert(p);
            for(auto ln : p->links){
              recursive_DFS(ln, level+1);
            }
          }

          void DFS_from(const std::string& s){
            auto p = find(s);
            if(!p) return;
            visited.clear();

            recursive_DFS(p);

          }

          void BFS_from (const std::string& s){
            auto p = find(s);
            if(!p) return;
            visited.clear();
            std::queue<node*>q;
            q.push(p);
            visited.insert(p);
            while(!q.empty()){
              auto current = q.front();
              q.pop();
              std::cout << current->value<<" ";
              for(auto &viz : current->links){
                if(visited.count(viz)==0){
                  q.push(viz);
                  visited.insert(viz);
                }
              }
            }
            std::cout<<"\n";
          }

          std::vector<std::string> shortestPath(const std::string& from, const std::string& to){
            std::vector<std::string> path;
            auto pfrom = find(from);
            if(!pfrom) return path;
            auto pto = find(to);
            if(!pto) return path;
            std::unordered_map<node*, node*>source;
            std::queue<node*>q;
            q.push(pfrom);
            source[pfrom] = nullptr;
            bool found = false;
            while(!q.empty()){
              auto current = q.front();
              q.pop();
              if(current == pto){
                found = true;
                break;
              }
              for(auto& viz : current->links){
                if(source.count(viz)==0){
                  q.push(viz);
                  source[viz] = current;
                }
              }
            }
            if(found){
              auto p = pto;
              while(p!=nullptr){
                path.push_back(p->value);
                p = source[p];
              }
              std::reverse(path.begin(), path.end());
            }
            return path;

          } 
          size_t edges(){
            size_t total = 0;

            for(auto& [k, nd] : nodes)
                total += nd.links.size();

            return total;
          }

          void printarMenorCaminho(const std::string& from, const std::string& to){
            auto caminho = shortestPath(from, to);
            if(caminho.empty()){
              std::cout<< "Caminho não encontrado!\n";
              return; 
            }
            std::cout << "Caminho encontrado (" << caminho.size() - 1 << " saltos):\n";
            for(size_t i=0; i<caminho.size(); i++){
              std::cout << caminho[i];
              if(i+1 < caminho.size())
                std::cout<< " -> ";
            }
            std::cout << "\n";

          }

          int diameter(){
            int max_diam = 0;
            for (auto& [start_node, nd] : nodes) {
              std::unordered_map<node*, int> dist;
              std::queue<node*> q;
              auto p = &nd;
              q.push(p);
              dist[p] = 0;
              while (!q.empty()) {
                auto curr = q.front();
                q.pop();
                for (auto& viz : curr->links) {
                  if (dist.count(viz) == 0) {
                    dist[viz] = dist[curr] + 1;
                    max_diam = std::max(max_diam, dist[viz]);
                    q.push(viz);
                  }
                }
              }
            }
            return max_diam;
          }

          void getCriticalRouters(){
            std::vector<std::pair<std::string, size_t>> degrees;
            for(auto& [key, nd] : nodes) {
                degrees.push_back({key, degree(key)});
            }

            std::sort(degrees.begin(), degrees.end(), [](auto& a, auto& b){
                return a.second > b.second;
            });

            std::cout << "\nTop 5 Roteadores Críticos (Por volume de conexões)\n";
            for(size_t i = 0; i < std::min<size_t>(5, degrees.size()); i++){
                std::cout << i+1 << ". IP: " << degrees[i].first << " | Grau total: " << degrees[i].second << "\n";
            }
          }

          void generatePDF(const std::string& filename = "grafo.pdf"){
            export2dot("temp_graph.dot");
            std::string cmd = "dot -Tpdf temp_graph.dot -o " + filename + ".pdf";
            std::system(cmd.c_str());
            std::cout << "Arquivo " << filename << " gerado com sucesso!\n";
          }

          void generatePNG(const std::string& filename = "grafo.png"){
            export2dot("temp_graph.dot");
            std::string cmd = "dot -Tpng temp_graph.dot -o " + filename + ".png";
            std::system(cmd.c_str());
            std::cout << "Arquivo " << filename << " gerado com sucesso!\n";
          }

          void export2dot(const std::vector<std::string>& caminho, const std::string& filename){
            std::unordered_set<std::string> nosDestacados(caminho.begin(), caminho.end());
            std::unordered_set<std::string> arestasDestacadas;
            for(size_t i = 0; i + 1 < caminho.size(); i++)
                arestasDestacadas.insert(caminho[i] + "->" + caminho[i+1]);

            std::ofstream dot(filename);
            dot << "digraph{\n";

            for(auto& [key, nd] : nodes){
                dot << "\t\"" << key << "\"";
                if(nosDestacados.count(key)){
                  if(key == caminho.front() || key == caminho.back())
                      dot << " [style=filled, fillcolor=\"#7B2FBE\"]";
                  else
                      dot << " [style=filled, fillcolor=\"#4A90D9\"]";
                }
                dot << ";\n";
            }

            for(auto& [key, nd] : nodes){
                for(auto viz : nd.links){
                    std::string aresta = key + "->" + viz->value;
                    dot << "\t\"" << key << "\" -> \"" << viz->value << "\"";
                    if(arestasDestacadas.count(aresta))
                        dot << " [color=\"#7B2FBE\", penwidth=2.0]";
                    dot << ";\n";
                }
            }
            dot << "}\n";
          }

          void generatePNGShortestPath(const std::string& from, const std::string& to, const std::string& filename){
            auto caminho = shortestPath(from, to);
            if(caminho.empty()){ std::cout << "Caminho não encontrado!\n"; return; }
            export2dot(caminho, "temp_path.dot");
            std::string cmd = "dot -Tpng temp_path.dot -o " + filename + ".png";
            std::system(cmd.c_str());
            std::cout << "Arquivo " << filename << ".png gerado!\n";
          }

        
        void generatePDFShortestPath(const std::string& from, const std::string& to, const std::string& filename){
            auto caminho = shortestPath(from, to);
            if(caminho.empty()){ std::cout << "Caminho não encontrado!\n"; return; }
            export2dot(caminho, "temp_path.dot");
            std::string cmd = "dot -Tpdf temp_path.dot -o " + filename + ".pdf";
            std::system(cmd.c_str());
            std::cout << "Arquivo " << filename << ".pdf gerado!\n";
          }
    }; // fim da classe digraph

} //fim do namespace