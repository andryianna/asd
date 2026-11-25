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
    std::unordered_map<int, Vertice *> vertici;
    std::unordered_map<Vertice *, std::vector<Arco>> adj;

    void resetta() {
        for (auto v: vertici) {
            v.second->parent = nullptr;
            v.second->color = WHITE;
            v.second->distanza = INT_MAX;
            v.second->inizio = 0;
            v.second->fine = 0;
        }
    }

    void dfs_visit1(Vertice *u, std::stack<Vertice *> &s, int &time) {
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

    void dfs_visit2(Vertice *u, std::ofstream &output, int &time) {
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

    bool dfs_visit(Vertice *u, std::stack<Vertice *> &s, int &time) {
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
        auto gr = new Grafo(this->directed);
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

    void initializeSingleSource(Vertice &s) {
        for (auto it: vertici) {
            auto v = it.second;
            if (v == getVertice(s.key)) continue;
            v->parent = nullptr;
            v->distanza = INT_MAX;
        }
        s.parent = nullptr;
        s.distanza = 0;
    }

    void relax(Vertice &u, Vertice &v, int w) {
        if (u.distanza == INT_MAX) return;
        if (w < 0 && v.distanza == INT_MAX) return;
        if (v.distanza > u.distanza + w) {
            v.distanza = u.distanza + w;
            v.parent = &u;
        }
    }
public:
    void addVertice(Vertice *v) {
        if (vertici.find(v->key) != vertici.end())
            return;
        vertici[v->key] = v;
        adj[v] = {};
    }

    Vertice *getVertice(int key) {
        auto it = vertici.find(key);
        if (it == vertici.end()) return nullptr;
        return it->second;
    }

    void addArco(int from, int to,int peso) {
        auto f = getVertice(from), t = getVertice(to);

        adj[f].push_back(Arco(t,peso));

        if (!directed) {
            adj[t].push_back(Arco(f,peso));
        }
    }

    Grafo(bool directed = true): directed(directed) {}
    Grafo(const std::string &filename, bool directed = true): directed(directed) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening file " << filename << std::endl;
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

    void BFS(Vertice *src,const std::string &filename) {
        std::ofstream file(filename);
        auto s = getVertice(src->key);
        if (!file) {
            std::cerr << "Error opening file " << filename << std::endl;
            return;
        }
        resetta();
        s->color = GRAY;
        s->distanza = 0;
        s->parent = nullptr;
        std::queue<Vertice *> bfs;
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

    void SCC(const std::string &filename) {
        std::ofstream file(filename);
        resetta();
        std::stack<Vertice *> s;
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

    void dfs_topo(const std::string &filename) {
        std::ofstream file(filename);
        std::stack<Vertice *> s;
        resetta();
        int time = 0;
        for (auto v: vertici)
            if (v.second->color == WHITE && !dfs_visit(v.second,s,time))
                return;
        file << "Ordinamento topologico " << std::endl;
        while (!s.empty()) {
            auto v = s.top();
            s.pop();
            file << v->key << " ";
        }
    }

    struct CmpDist {
        bool operator()(const std::pair<int, Vertice *> &a,
                        const std::pair<int, Vertice *> &b) const {
            return a.first > b.first; // min-heap sulla distanza
        }
    };

    std::vector<Vertice *> djkstra(Vertice *src) {
        std::vector<Vertice *> ordineVisita;
        if (!src) return ordineVisita;

        Vertice *s = getVertice(src->key);
        if (!s) return ordineVisita;

        initializeSingleSource(*s);

        std::priority_queue<std::pair<int, Vertice *>,
                            std::vector<std::pair<int, Vertice *>>,
                            CmpDist> pq;
        pq.push({0, s});

        while (!pq.empty()) {
            auto [dist, u] = pq.top();
            pq.pop();

            if (dist != u->distanza || u->distanza == INT_MAX) continue;

            ordineVisita.push_back(u);

            for (const auto &arco : adj[u]) {
                Vertice *v = arco.to;
                if (!v || u->distanza == INT_MAX) continue;

                if (v->distanza > u->distanza + arco.peso) {
                    v->distanza = u->distanza + arco.peso;
                    v->parent = u;
                    pq.push({v->distanza, v});
                }
            }
        }

        return ordineVisita;
    }

    bool bellmanFord(Vertice *src) {
        auto s = getVertice(src->key);
        initializeSingleSource(*s);
        for (size_t i = 0; i < vertici.size() - 1; ++i) {
            bool aggiornato = false;
            for (const auto &pair : adj) {
                Vertice *u = pair.first;
                if (!u || u->distanza == INT_MAX) continue;

                for (const auto &arco : pair.second) {
                    auto v = arco.to;
                    if (!v) continue;

                    int oldDist = v->distanza;
                    relax(*u, *v, arco.peso);
                    if (v->distanza != oldDist) {
                        aggiornato = true;
                    }
                }
            }
            if (!aggiornato) break;
        }

        for (const auto &pair : adj) {
            auto u = pair.first;
            if (!u || u->distanza == INT_MAX) continue;

            for (const auto &arco : pair.second) {
                auto v = arco.to;
                if (!v) continue;

                if (v->distanza > u->distanza + arco.peso) {
                    return false;
                }
            }
        }

        return true;
    }
};


int main(void) {
    Grafo gr("Test/Grafi/input.txt",true);
    gr.SCC("Test/Grafi/SCC.txt");
    gr.BFS(new Vertice(1),"Test/Grafi/BFS.txt");
    gr.dfs_topo("Test/Grafi/topo.txt");
    
    return 0;
}
