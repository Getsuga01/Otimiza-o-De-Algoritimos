/*
Trabalho II - Análise de Algoritmos
Aluno(s): Ronaldo Pereira Avansini - 2165578
          Marcus Paulo Dib Lopes - 2147404
          Diego José de Souza Sirvelli - 1901777
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <climits>
#include <algorithm>

using namespace std;

class Grafo {
private:
    unordered_map<string, vector<string>> adj;  // Lista de adjacência para grafo não direcionado
    map<char, vector<char>> adjDFS;             // Lista de adjacência para DFS

public:
    // Método para carregar o grafo a partir de um arquivo
    void carregarDeArquivo(const string& nomeArquivo) {
        adj.clear();  // Limpa qualquer dado antigo
        ifstream arquivo(nomeArquivo);
        if (!arquivo.is_open()) {
            cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
            exit(1);
        }

        string linha;
        while (getline(arquivo, linha)) {
            stringstream ss(linha);
            string u, v;
            if (getline(ss, u, ';') && getline(ss, v, ';')) {
                u.erase(remove(u.begin(), u.end(), '\r'), u.end());
                v.erase(remove(v.begin(), v.end(), '\r'), v.end());

                adj[u].push_back(v);
                adj[v].push_back(u); // Grafo não direcionado
            }
        }

        arquivo.close();

        // Ordena os vizinhos de cada vértice em ordem lexicográfica
        for (auto& par : adj) {
            sort(par.second.begin(), par.second.end());
        }
    }

    // Método para realizar a busca em largura (BFS)
    void buscaEmLargura(const string& inicio) {
        set<string> visitados;
        queue<string> fila;
        
        fila.push(inicio);
        visitados.insert(inicio);

        cout << "Ordem de visita BFS a partir do vertice '" << inicio << "': ";

        while (!fila.empty()) {
            string atual = fila.front();
            fila.pop();
            cout << atual << " ";

            for (const string& vizinho : adj[atual]) {
                if (visitados.find(vizinho) == visitados.end()) {
                    visitados.insert(vizinho);
                    fila.push(vizinho);
                }
            }
        }
        cout << endl;
    }

    // Método para realizar a busca em profundidade (DFS)
    void buscaEmProfundidade(char u) {
        set<char> visitados;
        vector<char> ordemVisita;

        dfsHelper(u, visitados, ordemVisita);

        cout << "Ordem de visita DFS a partir do vertice '" << u << "': ";
        for (char c : ordemVisita) {
            cout << c << " ";
        }
        cout << endl;
    }

private:
    // Função auxiliar para DFS (recursiva)
    void dfsHelper(char u, set<char>& visitados, vector<char>& ordemVisita) {
        visitados.insert(u);
        ordemVisita.push_back(u);

        sort(adjDFS[u].begin(), adjDFS[u].end()); // Ordena para garantir visita lexicográfica

        for (char v : adjDFS[u]) {
            if (visitados.find(v) == visitados.end()) {
                dfsHelper(v, visitados, ordemVisita);
            }
        }
    }

public:
    // Método para carregar dados de g2.txt para DFS
    void carregarGrafoDFS(const string& nomeArquivo) {
        adjDFS.clear(); // Adicionar no início de carregarGrafoDFS

        ifstream fin(nomeArquivo);
        if (!fin) {
            cerr << "Erro ao abrir arquivo " << nomeArquivo << endl;
            return;
        }

        string linha;
        while (getline(fin, linha)) {
            stringstream ss(linha);
            char u, v, pontoEVirgula;
            ss >> u >> pontoEVirgula >> v;
            adjDFS[u].push_back(v);
            adjDFS[v].push_back(u);  // Grafo não direcionado
        }
        fin.close();
    }
};

// Algoritmo de Dijkstra
class Dijkstra {
private:
    const static int SIZE = 7;
    string names[SIZE] = {"x", "v", "z", "r", "s", "t", "u"};

    int minimumDistance(int distance[], bool visited[]) {
        int min = INT_MAX, min_index = -1;
        for (int i = 0; i < SIZE; i++) {
            if (!visited[i] && distance[i] <= min) {
                min = distance[i];
                min_index = i;
            }
        }
        return min_index;
    }

    void printPath(int parent[], int j) {
        if (parent[j] == -1)
            return;
        printPath(parent, parent[j]);
        cout << " -> " << names[j];
    }

public:
    // Método para rodar o algoritmo de Dijkstra
    void dijkstra(int graph[SIZE][SIZE], int src, int dest) {
        int distance[SIZE];
        bool visited[SIZE];
        int parent[SIZE];

        for (int i = 0; i < SIZE; i++) {
            distance[i] = INT_MAX;
            visited[i] = false;
            parent[i] = -1;
        }

        distance[src] = 0;

        for (int count = 0; count < SIZE - 1; count++) {
            int u = minimumDistance(distance, visited);
            visited[u] = true;

            for (int v = 0; v < SIZE; v++) {
                if (!visited[v] && graph[u][v] && distance[u] + graph[u][v] < distance[v]) {
                    parent[v] = u;
                    distance[v] = distance[u] + graph[u][v];
                }
            }
        }

        cout << "Menor custo de " << names[src] << " até " << names[dest] << ": " << distance[dest] << endl;
        cout << "Caminho: " << names[src];
        printPath(parent, dest);
        cout << endl;
    }
};

// Função principal
int main() {
    Grafo g;
    
    // Exercício 1 e 2: Carregar e fazer a BFS para G1, G2 e G3
    g.carregarDeArquivo("g1.txt");
    g.buscaEmLargura("b");

    g.carregarDeArquivo("g2.txt");
    g.buscaEmLargura("b");

    g.carregarDeArquivo("g3.txt");
    g.buscaEmLargura("b");

    cout << "\n==============================" << endl;

    // Exercício 3: Carregar G2 e realizar DFS a partir do vértice 'a'
    g.carregarGrafoDFS("g2.txt");
    g.buscaEmProfundidade('a');

    cout << "\n==============================" << endl;

    // Exercício 4: Rodar o algoritmo de Dijkstra com matriz de adjacência
    int graph[7][7] = {
        { 0, 30, 12,  0,  0,  0,  0 },  // x
        {30,  0, 35, 17,  0,  0, 15 },  // v
        {12, 35,  0, 25, 20,  0,  0 },  // z
        { 0, 17, 25,  0,  7,  0, 15 },  // r
        { 0,  0, 20,  7,  0, 12, 10 },  // s
        { 0,  0,  0,  0, 12,  0,  5 },  // t
        { 0, 15,  0, 15, 10,  5,  0 }   // u
    };

    Dijkstra d;
    d.dijkstra(graph, 0, 5); // x até t

    return 0;
}
