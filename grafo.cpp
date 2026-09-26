#include <fstream>
#include <iostream>
#include <climits>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_map>

using namespace std;

using Color = enum{WHITE, BLACK, GRAY};

class Vertice {
public:
    int key;
    Color color = WHITE;
    Vertice *parent = nullptr;
    int distanza = INT_MAX,inizio = 0, fine = 0;
    Vertice(int k): key(k){}
};

class Arco {
public:
    Vertice *to;
    int peso;

    Arco(Vertice *to, int peso): to(to), peso(peso){}
};

class Grafo {
    bool directed;
    unordered_map<int, Vertice *> vertici;
    unordered_map<Vertice *, vector<Arco>> adj;

    void resetta() {
        for (auto v: vertici) {
            v.second->parent = nullptr;
            v.second->color = WHITE;
            v.second->distanza = INT_MAX;
            v.second->inizio = 0;
            v.second->fine = 0;
        }
    }

    void dfs_visit1(Vertice *u, stack<Vertice *> &s, int &time) {
        u->color = GRAY;
        u->inizio = ++time;
        for (auto arco: adj.at(u)) {
            auto v = arco.to;
            if (v->color == WHITE) {
                v->parent = u;
                dfs_visit1(v, s, time);
            }
        }
        s.push(u);
        u->color = BLACK;
        u->fine = ++time;
    }

    void dfs_visit2(Vertice *u, ofstream &output, int &time) {
        u->color = GRAY;
        u->inizio = ++time;

        output << u->key << " ";

        for (const auto &arco: adj.at(u)) {
            auto v = arco.to;
            if (v->color == WHITE) {
                v->parent = u;
                dfs_visit2(v, output, time);
            }
        }
        u->color = BLACK;
        u->fine = ++time;
    }

    bool dfs_visit(Vertice *u, stack<Vertice *> &s, int &time) {
        u->color = GRAY;
        u->inizio = ++time;
        for (auto arco: adj.at(u)) {
            auto v = arco.to;
            if (v->color == GRAY) {
                return false;
            }
            if (v->color == WHITE) {
                v->parent = u;
                if (!dfs_visit(v, s, time)) return false;
            }
        }
        u->color = BLACK;
        u->fine = ++time;
        s.push(u);
        return true;
    }

    Grafo *transpose() {
        auto gr = new Grafo(true);
        for (auto v: vertici)
            gr->addVertice(new Vertice(v.first));
        for (auto arco: adj) {
            auto v = arco.first;
            for (auto u: arco.second) {
                auto from = gr->getVertice(u.to->key);
                auto to = gr->getVertice(v->key);

                gr->adj.at(from).emplace_back(to,u.peso);
            }
        }
        return gr;
    }
public:
    void addVertice(Vertice *v) {
        if (vertici.find(v->key) != vertici.end())
            return;
        vertici[v->key] = v;
        adj[v] = {};
    }

    Vertice *getVertice(int key) {
        return vertici.find(key) != vertici.end() ? vertici.find(key)->second : nullptr; 
    }

    void addArco(int from, int to,int peso) {
        auto f = getVertice(from), t = getVertice(to);

        adj[f].push_back(Arco(t,peso));

        if (!directed) {
            adj[t].push_back(Arco(f,peso));
        }
    }

    Grafo(bool directed): directed(directed) {}
    Grafo(const string &filename, bool directed): directed(directed) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error opening file " << filename << endl;
            return;
        }
        int kV,kA;
        file >> kV >> kA;
        int from,to,peso;
        for (int i = 1; i <= kV; i++)
            addVertice(new Vertice(i));
        for (int i = 0; i < kA; i++)
            if (file >> from >> to >> peso)
                addArco(from,to,peso);
        file.close();
    }

    void BFS(Vertice *src,const string &filename) {
        ofstream file(filename);
        auto s = getVertice(src->key);
        if (!file) {
            std::cerr << "Error opening file " << filename << endl;
            return;
        }
        resetta();
        s->color = GRAY;
        s->distanza = 0;
        s->parent = nullptr;
        queue<Vertice *> bfs;
        bfs.push(s);
        while (!bfs.empty()) {
            auto u = bfs.front();
            bfs.pop();
            for (auto arco: adj.at(u)) {
                auto v = arco.to;
                if (v->color == WHITE) {
                    v->color = GRAY;
                    v->parent = u;
                    v->distanza = u->distanza + 1;
                    bfs.push(v);
                }
            }
            u->color = BLACK;
        }
        for (auto v: vertici)
            file << "Distanza dal nodo " << v.first <<" "<< v.second->distanza << std::endl;
        file.close();
    }

    void SCC(const string &filename) {
        ofstream file(filename);
        resetta();
        stack<Vertice *> s;
        int time = 0;
        for (auto v: vertici)
            if (v.second->color == WHITE)
                dfs_visit1(v.second,s,time);
        auto gt = transpose();
        time = 0;
        while (!s.empty()) {
            auto v = gt->getVertice(s.top()->key);
            s.pop();
            if (v->color == WHITE)
                gt->dfs_visit2(v,file,time);
        }
        file.close();
    }

    void dfs_topo(const string &filename) {
        ofstream file(filename);
        stack<Vertice *> s;
        resetta();
        int time = 0;
        for (auto v: vertici)
            if (v.second->color == WHITE && !dfs_visit(v.second,s,time))
                return;
        file << "Ordinamento topologico " << endl;
        while (!s.empty()) {
            auto v = s.top();
            s.pop();
            file << v->key << " ";
        }
    }
};

int main(void) {
    Grafo gr("Test/Grafi/input.txt",true);
    gr.SCC("Test/Grafi/SCC.txt");
    gr.BFS(new Vertice(1),"Test/Grafi/BFS.txt");
    gr.dfs_topo("Test/Grafi/topo.txt");
    
    return 0;
}
