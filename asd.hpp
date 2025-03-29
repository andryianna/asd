
#ifndef asd_hpp
#define asd_hpp

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class Node{
    int key;
    string value;
    Node *left;
    Node *right;
    Node *parent;
    
public:
    Node(const int &chiave, const string &valore): key(chiave), left(nullptr), right(nullptr), parent(nullptr){
        this->value = valore;
    }
    int getKey() const{return key;}
    Node *&getLeftNode(){return left;}
    Node *&getRightNode(){return right;}
    Node *&getParentNode(){return parent;}
    void setParentNode(Node *newNode){this->parent = newNode;}
    void setRightNode(Node *newNode){this->right = newNode;}
    void setLeftNode(Node *newNode){this->left = newNode;}
};

class ABR {
    Node *root;
    ofstream file;
    static bool isEmpty(const Node *ptr){
        return ptr == nullptr;
    }
    void static insertH(Node **ptr, const int &chiave, const string &valore){
        if (isEmpty(*ptr)){
            *ptr = new Node(chiave,valore);
        }
        else if (chiave < (*ptr)->getKey()){
            insertH(&(*ptr)->getLeftNode(), chiave, valore);
        }
        else if (chiave > (*ptr)->getKey()){
            insertH(&((*ptr)->getRightNode()), chiave, valore);
        }
        else
            std::cout<< "duplicato";
    }
    
    void preOrderH(Node *ptr){
        if (!isEmpty(ptr)){
            if (file.is_open()) {
                file<<ptr->getKey();
            }
            else
                std::cout<<ptr->getKey();
            preOrderH(ptr->getLeftNode());
            preOrderH(ptr->getRightNode());
        }
    }
    void inOrderH(Node *ptr){
        if (!isEmpty(ptr)){
            inOrderH(ptr->getLeftNode());
            if (file.is_open())
                file<<ptr->getKey();
            else
                std::cout<<ptr->getKey();
            inOrderH(ptr->getRightNode());
        }
    }
    void postOrderH(Node *ptr){
        if(!isEmpty(ptr)){
            postOrderH(ptr->getLeftNode());
            postOrderH(ptr->getRightNode());
            if (file.is_open())
                file<<ptr->getKey();
            else
                std::cout<<ptr->getKey();
        }
    }
    int static getTreeHeightH(Node *ptr){
        if (isEmpty(ptr)) {
            return 0;
        }
        return 1 + getTreeHeightH(ptr->getLeftNode()) + getTreeHeightH(ptr->getRightNode());
    }
    
    static Node *getTreeMinimumH(Node *ptr){
        if (isEmpty(ptr->getLeftNode())) {
            return ptr;
        }
        return ptr->getLeftNode();
    }
    static Node *getTreeMaximumH(Node *ptr){
        if (isEmpty(ptr->getRightNode())) {
            return ptr;
        }
        return ptr->getRightNode();
    }
    static Node* searchH(Node *current, const int &key){
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
    
    static int sumLeavesH(Node *ptr){
        if (isEmpty(ptr))
            return 0;
        if (isEmpty(ptr->getLeftNode()) && isEmpty(ptr->getRightNode())) {
            return ptr->getKey();
        }
        
        return sumLeavesH(ptr->getLeftNode()) + sumLeavesH(ptr->getRightNode());
    }
    
   
    void transplant(Node *source, Node *replacement){
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
    explicit ABR(Node *node):root(node){}
    explicit ABR(const string &filename):root(nullptr){
        file.open(filename, ios::app);
    }
    ABR(const string &filename, Node *node): root(node) {
        file.open(filename, ios::app);
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
        Node *returnNode = getTreeMinimumH(root);
        return returnNode->getKey();
    }
    int getTreeMaximum() const {
        Node *returnNode = getTreeMinimumH(root);
        return returnNode->getKey();
    }
    static int getSuccessorOf(Node *x){
        if (!isEmpty(x->getRightNode()))
            return getTreeMinimumH(x->getRightNode())->getKey();
        Node *y = nullptr;
        y = x->getParentNode();
        while (!isEmpty(y) && x == y->getRightNode()) {
            x = y;
            y = y->getParentNode();
        }
        
        return y->getKey();
    }

    static int getPredecessorOf(Node *x){
        if (!isEmpty(x->getLeftNode()))
            return getTreeMinimumH(x->getLeftNode())->getKey();
        Node *y = nullptr;
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
    
    void deleteNode(Node *ptr){
        if (isEmpty(ptr->getLeftNode())) {
            transplant(ptr, ptr->getRightNode());
        }
        else if(isEmpty(ptr->getRightNode()))
            transplant(ptr, ptr->getLeftNode());
        else{
            Node *tmp = getTreeMinimumH(ptr->getRightNode());
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
    int get_data() const{
        return data;
    }
    void set_data(int valore){
        this->data=valore;
    }
    
    listNode* get_next() const {
        return next;
    }
    
    void set_next(listNode* next){
        this->next=next;
    }
private:
    int data;
    listNode *next;
    
};



#endif /* asd_hpp */
