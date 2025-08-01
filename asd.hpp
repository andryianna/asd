
#ifndef asd_hpp
#define asd_hpp
#include <vector>

enum color{
    black = true,
    red = false
};

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

template<typename  T>
class Node{
    T key;
    string value;
    Node *left;
    Node *right;
    Node *parent;
    
public:
    Node(const T &chiave, const string &valore): key(chiave), left(nullptr), right(nullptr), parent(nullptr){
        this->value = valore;
    }
    string getValue() {
        return value;
    }
    T getKey() const{return key;}
    Node *&getLeftNode(){return left;}
    Node *&getRightNode(){return right;}
    Node *&getParentNode(){return parent;}
    void setParentNode(Node *newNode){this->parent = newNode;}
    void setRightNode(Node *newNode){this->right = newNode;}
    void setLeftNode(Node *newNode){this->left = newNode;}
};

template<typename T>
class HeapTree {
    vector<T> data;
    void swap(auto &a, auto &b) {
        auto tmp = a;
        a = b;
        b = tmp;
    }
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (data[index] < data[parent]) {
                swap(data[index], data[parent]);
                index = parent;
            }
            else
                return;
        }
    }

    void heapifyDown(int index) {
        int n = data.size();
        while (true) {
            int l = 2 * index + 1;
            int r = 2 * index + 2;
            int smallest = index;
            if (l < n && data[l] < data[smallest]) {
                smallest = l;
            }
            if (r < n && data[r] < data[smallest]) {
                smallest = r;
            }
            if (smallest != index) {
                swap(data[index], data[smallest]);
                index = smallest;
            }
            else
                return;
        }
    }
public:
    explicit HeapTree(vector<T> &data) : data(data) {}
    void insert(const T& value) {
        this->data.push_back(value);
        heapifyUp(this->data.size() - 1);
    }
    void print() {
        for (const auto &val : this->data) {
            cout << val << " ";
        }
    }
};


template <typename T>
class ABR {
    Node<T> *root;
    ofstream file;
    static bool isEmpty(const Node<T> *ptr){
        return ptr == nullptr;
    }
    void static insertH(Node<T> **ptr, const T &chiave, const string &valore){
        if (isEmpty(*ptr)){
            *ptr = new Node<T>(chiave,valore);
        }
        else if (chiave < (*ptr)->getKey()){
            insertH(&(*ptr)->getLeftNode(), chiave, valore);
        }
        else if (chiave > (*ptr)->getKey()){
            insertH(&((*ptr)->getRightNode()), chiave, valore);
        }
        else
            std::cout<< "duplicato" << chiave << " " << valore << endl;
    }
    
    void preOrderH(Node<T> *ptr){
        if (!isEmpty(ptr)){
            if (file.is_open()) {
                file<<ptr->getKey()<<","<<ptr->getValue()<<std::endl;
            }
            else
                cout<<ptr->getKey()<<","<<ptr->getValue()<<endl;
            preOrderH(ptr->getLeftNode());
            preOrderH(ptr->getRightNode());
        }
    }
    void inOrderH(Node<T> *ptr){
        if (!isEmpty(ptr)){
            inOrderH(ptr->getLeftNode());
            if (file.is_open())
                file<<ptr->getKey()<<","<<ptr->getValue()<<std::endl;
            else
                std::cout<<ptr->getKey()<<","<<ptr->getValue()<<std::endl;
            inOrderH(ptr->getRightNode());
        }
    }
    void postOrderH(Node<T> *ptr){
        if(!isEmpty(ptr)){
            postOrderH(ptr->getLeftNode());
            postOrderH(ptr->getRightNode());
            if (file.is_open())
                file<<ptr->getKey()<<","<<ptr->getValue()<<std::endl;
            else
                std::cout<<ptr->getKey()<<std::endl;
        }
    }
    int static getTreeHeightH(Node<T> *ptr){
        if (isEmpty(ptr)) {
            return 0;
        }
        return 1 + getTreeHeightH(ptr->getLeftNode()) + getTreeHeightH(ptr->getRightNode());
    }
    
    static Node<T> *getTreeMinimumH(Node<T> *ptr){
        if (isEmpty(ptr->getLeftNode())) {
            return ptr;
        }
        return ptr->getLeftNode();
    }
    static Node<T> *getTreeMaximumH(Node<T> *ptr){
        if (isEmpty(ptr->getRightNode())) {
            return ptr;
        }
        return ptr->getRightNode();
    }
    static Node<T>* searchH(Node<T> *current, const int &key){
        if (isEmpty(current)) {
            return nullptr;
        }
        else if (key == current->getKey()) {
            return current;
        }
        else if (key < current->getKey())
        {
            return current->getLeftNode();
        }
        else
        {
            return current->getRightNode();
        }
    }
    
    static int sumLeavesH(Node<T> *ptr){
        if (isEmpty(ptr))
            return 0;
        if (isEmpty(ptr->getLeftNode()) && isEmpty(ptr->getRightNode())) {
            return ptr->getKey();
        }
        
        return sumLeavesH(ptr->getLeftNode()) + sumLeavesH(ptr->getRightNode());
    }
    
   
    void transplant(Node<T> *source, Node<T> *replacement){
        if (isEmpty(source->getParentNode()))
            root = replacement;
        else if(source == source->getParentNode()->getLeftNode())
            source->getParentNode()->getLeftNode() = replacement;
        else
            source->getParentNode()->getRightNode() = replacement;
        if (!isEmpty(replacement)) {
            replacement->getParentNode()->setParentNode(source->getParentNode());
        }
    }
    
public:
    ABR():root(nullptr){}
    explicit ABR(Node<T> *node):root(node){}
    explicit ABR(const string &filename):root(nullptr){
        file.open(filename, ios::app);
    }
    ABR(const string &filename, Node<T> *node): root(node) {
        file.open(filename, ios::app);
    }
    void insertFromFile(const std::string& filename) {
        std::ifstream infile(filename);
        if (!infile.is_open()) {
            std::cerr << "Errore nell'apertura del file: " << filename << std::endl;
            return;
        }

        std::string line;
        if (std::getline(infile, line)) {
            std::stringstream ss(line);
            std::string token;

            while (std::getline(ss, token, ',')) {
                std::stringstream pairStream(token);
                std::string keyStr, value;
                if (pairStream >> keyStr >> value) {
                    int key = std::stoi(keyStr);
                    insert(key, value);
                }
            }
        }

        infile.close();
    }
    void insert(const int &chiave,const string &valore){
        insertH(&root, chiave, valore);
    }
    void preOrder(){
        preOrderH(root);
    }
    void inOrder() {
        inOrderH(root);
    }
    void postOrder() {
        postOrderH(root);
    }
    int getTreeHeight() const{
        return getTreeHeightH(root);
    }
    int getTreeMinimum() const {
        Node<T> *returnNode = getTreeMinimumH(root);
        return returnNode->getKey();
    }
    int getTreeMaximum() const {
        Node<T> *returnNode = getTreeMinimumH(root);
        return returnNode->getKey();
    }
    static int getSuccessorOf(Node<T> *x){
        if (!isEmpty(x->getRightNode()))
            return getTreeMinimumH(x->getRightNode())->getKey();
        Node<T> *y = nullptr;
        y = x->getParentNode();
        while (!isEmpty(y) && x == y->getRightNode()) {
            x = y;
            y = y->getParentNode();
        }

        return y->getKey();
    }

    static int getPredecessorOf(Node<T> *x){
        if (!isEmpty(x->getLeftNode()))
            return getTreeMinimumH(x->getLeftNode())->getKey();
        Node<T> *y = nullptr;
        y = x->getParentNode();
        while (!isEmpty(y) && x == y->getLeftNode()) {
            x = y;
            y = y->getParentNode();
        }
        return y->getKey();
    }

    int search(const int& value) const {
        if (searchH(root, value) == nullptr) {
            return -1;
        }
        return 1;
    }
    
    void deleteNode(Node<T> *ptr){
        if (isEmpty(ptr->getLeftNode())) {
            transplant(ptr, ptr->getRightNode());
        }
        else if(isEmpty(ptr->getRightNode()))
            transplant(ptr, ptr->getLeftNode());
        else{
            Node<T> *tmp = getTreeMinimumH(ptr->getRightNode());
            if (tmp->getParentNode() != ptr) {
                transplant(tmp, tmp->getRightNode());
                tmp->getRightNode() = ptr->getRightNode();
                tmp->getRightNode()->getParentNode() = tmp;
            }
            transplant(ptr, tmp);
            tmp->getLeftNode() = ptr->getLeftNode();
            tmp->getLeftNode()->getParentNode() = tmp;
        }
    }
    
    
};

class listNode {
public:
    explicit listNode(const int &valore):data(valore),next(nullptr){}
    ~listNode();
    [[nodiscard]] int get_data() const{
        return data;
    }
    void set_data(int valore){
        this->data=valore;
    }
    
    [[nodiscard]] listNode* get_next() const {
        return next;
    }
    
    void set_next(listNode* next){
        this->next=next;
    }
private:
    int data;
    listNode *next;
    
};

class LCS {
    string x;
    string y;
    public:
    LCS(string a,string b): x(move(a)),y(move(b)) {}
    void setx(string newx) {
        x = move(newx);
    }
    [[nodiscard]] string getx() const {
        return x;
    }
    void sety(string newy) {
        y = move(newy);
    }
    [[nodiscard]] string gety() const {
        return y;
    }

    [[nodiscard]]int calculate() const{
        int m = x.length();
        int n = y.length();

        vector<vector<int>> dp(m+1,vector<int>(n+1,0));

        for(int i=1;i<=m;i++) {
            for(int j=1;j<=n;j++) {
                if (x[i - 1] == y[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else {
                    dp[i][j] = dp[i - 1][j];
                }
            }
        }
        return dp[m][n];
    }
};


#endif /* asd_hpp */
