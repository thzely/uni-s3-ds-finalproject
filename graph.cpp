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


          
    }; // fim da classe digraph

} //fim do namespace