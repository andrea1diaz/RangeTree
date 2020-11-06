#include <iostream>
#include <utility>

using namespace std;

template <typename T>

struct Node {
    Node<T> *next;
    Node<T> *prev;
    
    //child nodes
    Node<T> *left;
    Node<T> *right;

    T data;
    bool is_leaf;
}

template <typename T>
struct RangeTree {
    Node<T> * root;
    

    RangeTree() = default;


    void construct( vector< pair<int,int> > arr){
        sort(arr.rbegin(), arr.rend());
        //vector< Node<T> *> nodes;
    
        for(int i=0; i<arr.size(); ++i){
            
        }
    }

    // a < b
    Node* split (Node* a, Node* b) {
        Node new_parent = new Node;
        
        new_parent->data = (a->data + b->data) / 2;
        new_parent->left = a;
        new_parent->rigth = b;

        a->next = b;
        b->prev = a;

        return new_parent;
    }    
    
}



int main () {
    
    return 0;
}