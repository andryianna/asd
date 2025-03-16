//
//  asd.hpp
//  asnjandsfj
//
//  Created by Andrea Iannaccone on 13/03/25.
//

#ifndef asd_hpp
#define asd_hpp

#include <iostream>
#include <cstdlib>
#include <cstring>
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
    int getKey(){return key;}
    Node *&getLeftNode(){return left;}
    Node *&getRightNode(){return right;}
    Node *&getParentNode(){return parent;}
    void setParentNode(Node *newNode){this->parent = newNode;}
    void setRightNode(Node *newNode){this->right = newNode;}
};

class ABR {
    Node *root;
    bool isEmpty(Node *ptr){
        return ptr == nullptr;
    }
    void insertH(Node **ptr, const int &chiave, const string valore){
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
            std::cout<<ptr->getKey();
            preOrderH(ptr->getLeftNode());
            preOrderH(ptr->getRightNode());
        }
    }
    void inOrderH(Node *ptr){
        if (!isEmpty(ptr)){
            inOrderH(ptr->getLeftNode());
            std::cout<<ptr->getKey();
            inOrderH(ptr->getRightNode());
        }
    }
    void postOrderH(Node *ptr){
        if(!isEmpty(ptr)){
            postOrderH(ptr->getLeftNode());
            postOrderH(ptr->getRightNode());
            std::cout<<ptr->getKey();
        }
    }
    int getTreeHeightH(Node *ptr){
        if (isEmpty(ptr)) {
            return 0;
        }
        return 1 + getTreeHeightH(ptr->getLeftNode()) + getTreeHeightH(ptr->getRightNode());
    }
    
    Node *getTreeMinimumH(Node *ptr){
        if (isEmpty(ptr->getLeftNode())) {
            return ptr;
        }
        return ptr->getLeftNode();
    }
    Node *getTreeMaximumH(Node *ptr){
        if (isEmpty(ptr->getRightNode())) {
            return ptr;
        }
        return ptr->getRightNode();
    }
    Node* searcH(Node *current, const int &key){
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
    
    int sumLeavesH(Node *ptr){
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
    ABR(Node *node):root(node){}
    void insert(const int &chiave,const string &valore){
        insertH(&root, chiave, valore);
    }
    void preOrder(){
        preOrderH(root);
    }
    void inOrder(){
        inOrderH(root);
    }
    void postOrder(){
        postOrderH(root);
    }
    int getTreeHeight(){
        return getTreeHeightH(root);
    }
    int getTreeMinimum(){
        Node *returnNode = getTreeMinimumH(root);
        return returnNode->getKey();
    }
    int getTreeMaximum(){
        Node *returnNode = getTreeMinimumH(root);
        return returnNode->getKey();
    }
    int getSuccessorOf(Node *x){
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
    int getPredecessorOf(Node *x){
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
    
    int search(const int& value){
        if (searcH(root, value) == nullptr) {
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
    listNode(const int &valore):data(valore),next(nullptr){}
    ~listNode();
    int get_data(){
        return data;
    }
    void set_data(int valore){
        this->data=valore;
    }
    
    listNode* get_next(){
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
