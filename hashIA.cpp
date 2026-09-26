#include <fstream>
#include <iostream>
#include <vector>

using HashState = enum {FREE, DELETED, OCCUPIED};
using namespace std;

class HashNode {
public:
    int key;
    string val;
    HashState state = FREE;

    HashNode (int k,string v): key(k), val(move(v)) {}
};


//Hash Table ad indirizzamento aperto
class HashTableIA {
    vector<HashNode *> table;
    long long size;
    
    //metodo hash divisione
    int h(int k,int i) {
        return (k+i) % size;
    }
    
public:
    HashTableIA(const string &filename,int size): size(size) {
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

	void insert(HashNode *node) {
	for (int i = 0; i < size; i++) {
		int j = h(node->key,i);
			if (table[j] == nullptr || table[j]->state == FREE || table[j]->state == DELETED) {
			delete table[j];
			table[j] = node;
			node->state = OCCUPIED;
			return;
			}
		}
	}
    
    //se chiede la stringa associata alla chiave
    string findS(int key) {
        int i = 0, j = h(key,i);
        while (table[j] != nullptr && i != size) {
            if (table[j]->key == key && table[j]->state == OCCUPIED)
                return table[j]->val;
            i++;
            j = h(key,i);
        }
    }
    
    //se chiede find generico
    HashNode* find(int key) {
        int i = 0, j = h(key,i);
        while (table[j] != nullptr && i != size) {
            if (table[j]->key == key && table[j]->state == OCCUPIED)
                return table[j];
            i++;
            j = h(key,i);
        }
    }
    
    void print(const string &filename) {
        ofstream file(filename);
        if (!file) {
            std::cerr << "Error opening file " << filename << std::endl;
            return;
        }
        for (int i = 0; i < size; i++)
        	if (table[i] != nullptr && table[i]->state == OCCUPIED)
        		file <<table[i]->key << " " <<  table[i]->val << " ";
        file << endl;
        file.close();
    }

    void remove(int key) {
        int i = 0, j = h(key,i);
        while (table[j] != nullptr && i != size) {
            if (table[j]->key == key && table[j]->state == OCCUPIED) {
                table[j]->state = DELETED;
                return;
            }
        }
    }
};

int main(){
	//dimensione da verificare nel file di input
	HashTableIA htia("Test/Hash/hashstringa.txt",4);
	htia.print("Test/Hash/outputIA.txt");
	
	return 0;
}

