#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

class HashNode {
public:
    int key;
    string val;

    HashNode(int k, const string& v) : key(k), val(v) {}
};
//Hash Table con concatenamento
class HashTableC {
    vector<list<HashNode>> table;
    int size;

    int h(int k) const {
        int resto = k % size;
        return resto < 0 ? resto + size : resto;
    }

public:
HashTableC(const string &filename,int size): size(size) {
    	table.resize(size);
        ifstream file(filename);
        if (!file) {
            cerr << "Error opening file " << filename << endl;
            return;
        }
        int k;
        string v;
        //formato stringa <int,string>
        
        char a,b;
        while (file >> a >> k >> b){
            if (!getline(file,v,'>')) break;
            insert(new HashNode(k,v));
        }
    }

    void insert(const HashNode *node) {
        auto& lista = table[h(node->key)];

        for (auto& elemento : lista) {
            if (elemento.key == node->key) {
                elemento.val = node->val;
                return;
            }
        }

        lista.push_back(*node);
    }

    const HashNode* find(int k) const {
        const auto& lista = table[h(k)];

        for (const auto& nodo : lista) {
            if (nodo.key == k)
                return &nodo;
        }

        return nullptr;
    }

    const string* findValue(int k) const {
        const HashNode* nodo = find(k);
        return nodo != nullptr ? &nodo->val : nullptr;
    }

    bool remove(int k) {
        auto& lista = table[h(k)];

        for (auto it = lista.begin(); it != lista.end(); ++it) {
            if (it->key == k) {
                lista.erase(it);
                return true;
            }
        }

        return false;
    }
    
    void print(const string& filename) const {
    ofstream file(filename);

    if (!file) {
        cerr << "Non sono riuscito ad aprire il file "
             << filename << endl;
        return;
    }

    for (const auto& lista : table) {
        for (const auto& nodo : lista) {
            file << nodo.key << " " << nodo.val << '\n';
        }
    }
    file.close();
}
};

int main(){
	//dimensione da verificare nel file di input
	HashTableC htc("Test/Hash/hashstringa.txt",4);
	htc.print("Test/Hash/outputC.txt");
	
	return 0;
}
