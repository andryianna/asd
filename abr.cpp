#include <fstream>
#include <iostream>
#include <climits>

using namespace std;

class TreeNode {
public:
    int key;
    char val;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
    TreeNode *parent = nullptr;

    TreeNode(int k,char v): key(k), val(v) {}
};

class ABR {
    TreeNode *root = nullptr;
    bool isEmpty(TreeNode *node) {
        return node == nullptr;
    }

    void PreOrder(TreeNode *ptr, ofstream &file) {
        if (!isEmpty(ptr)) {
            file << "Chiave :"<< ptr->key << " Valore: "<< ptr->val << endl;
            PreOrder(ptr->left, file);
            PreOrder(ptr->right, file);
        }
    }
    void InOrder(TreeNode *ptr, ofstream &file) {
        if (!isEmpty(ptr)) {
            InOrder(ptr->left, file);
	    file << "Chiave :"<< ptr->key << " Valore: "<< ptr->val << endl; 
            InOrder(ptr->right, file);
        }
    }
    void PostOrder(TreeNode *ptr, ofstream &file) {
        if (!isEmpty(ptr)) {
            
            PostOrder(ptr->left, file);
            PostOrder(ptr->right, file);
            file << "Chiave :"<< ptr->key << " Valore: "<< ptr->val << endl;
        }
    }

    TreeNode *minimum(TreeNode *x) {
        while (!isEmpty(x->left))
            x = x->left;
        return x;
    }
    TreeNode *maximum(TreeNode *x) {
        while (!isEmpty(x->right))
            x = x->right;
        return x;
    }
    
//parte di Huffman
bool cercaCodice(TreeNode *curr, char lettera, string &codice) {
    if (isEmpty(curr))
        return false;

    if (isEmpty(curr->left) && isEmpty(curr->right))
        return curr->val != '*' && curr->val == lettera;

    codice += '0';

    if (cercaCodice(curr->left, lettera, codice))
        return true;

    codice.pop_back();

    codice += '1';

    if (cercaCodice(curr->right, lettera, codice))
        return true;

    codice.pop_back();

    return false;
}
public:
    ABR(const std::string &filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error opening file " << filename << endl;
            return;
        }
        int k;
        char v;
        while (file >> k >> v)
            insert(new TreeNode(k,v));
        file.close();

    }
    void insert(TreeNode *z) {
        auto x = root;
        TreeNode *y = nullptr;
        while (!isEmpty(x)) {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }
        z->parent = y;
        if (isEmpty(y))
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;
    }

    TreeNode *successor(TreeNode *x) {
        if (!isEmpty(x->right)) {
            return minimum(x->right);
        }
        auto y = x->parent;
        while (!isEmpty(y) && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }
    TreeNode *predecessor(TreeNode *x) {
        if (!isEmpty(x->left)) {
            return maximum(x->left);
        }
        auto y = x->parent;
        while (!isEmpty(y) && x == y->left) {
            x = y;
            y = y->parent;
        }
        return y;
    }
    void preOrder(const string &filename) {
        std::ofstream file(filename);
        if (!file) {
            cerr << "Error opening file " << filename << std::endl;
            return;
        }
        PreOrder(root, file);
        file.close();
    }
    void inOrder(const string &filename) {
        std::ofstream file(filename);
        if (!file) {
            cerr << "Error opening file " << filename << endl;
            return;
        }
        InOrder(root, file);
        file.close();
    }
    void postOrder(const string &filename) {
        std::ofstream file(filename);
        if (!file) {
            cerr << "Error opening file " << filename << std::endl;
            return;
        }
        PostOrder(root, file);
        file.close();
    }
    
    TreeNode *successorDispari(TreeNode *x) {
        if (isEmpty(x))
            return nullptr;

        x = successor(x);

        while (x != nullptr && x->key % 2 == 0)
            x = successor(x);

        return x;
	}

    TreeNode *successorPari(TreeNode *x) {
        if (x == nullptr)
           return nullptr;

        x = successor(x);

        while (x != nullptr && x->key % 2 != 0)
           x = successor(x);

        return x;
	}
	
	//codifica (6 pt)
	string codificaHuffman(const string &s) {
    		string tmp;

    		for (char lettera : s) {
        		string codice;

        		if (!cercaCodice(root, lettera, codice)) {
            			cerr << "Lettera non presente nell'albero: " << lettera << endl;
            			continue;
        		}

        	if (codice.empty())
            		codice = "0";

        	tmp += codice;
    	}

    	return tmp;
	}
	
	//decodifica (6 pt)
	string decodificaHuffman(const string &s) {
    		string tmp;
    		TreeNode *curr = root;
    		
    		if (s.empty())
        		return tmp;

    		if (isEmpty(root)){
        		cerr << "Albero vuoto" << endl;
        		return "";
        	}

    		if (isEmpty(root->left) && isEmpty(root->right)) {
        		for (char bit : s) {
            			if (bit != '0')
                throw invalid_argument("Codice non valido");

            tmp += root->val;
        }

        return tmp;
    }

    for (char bit : s) {
        if (bit == '0')
            curr = curr->left;
        else if (bit == '1')
            curr = curr->right;
        else
            throw invalid_argument("Inserire solo 0 e 1");

        if (isEmpty(curr))
            throw invalid_argument("Percorso non valido");

        if (isEmpty(curr->right) && isEmpty(curr->left)) {
            if (curr->val < 'A' || curr->val > 'Z')
                throw invalid_argument("Foglia non valida");

            tmp += curr->val;
            curr = root;
        }
    }

    if (curr != root)
        throw invalid_argument("La sequenza termina con un codice incompleto");

    return tmp;
}
};



int main(void) {
    ABR abr("Test/ABR/inputStringa.txt");
    //Mi raccomando a quello che chiede la traccia e fai visualizzare quello che chiede
    abr.preOrder("Test/ABR/out.txt");
    
    return 0;
}
