#include <iostream>
#include <utility>
#include <vector>

using namespace std;

template <typename T>

struct Node {
    Node<T> *next;
    Node<T> *prev;
    
    //child nodes
    Node<T> *left;
    Node<T> *right;

    //parent
    Node<T> *parent;

    T data;
    bool is_leaf;

    Node(T data_) : data{data_} {};
    Node (T data_, Node<T>* left_, Node<T>* right_) : data{data_}, left{left_}, right{right_} {};
};

template <typename T>
class RangeTree {
    Node<T> *root;
    std::vector<T> v;
    Node<T> * aux = nullptr;
    
public:
    RangeTree(std::vector<T> v){
        this->v = v;

        root = create(0, v.size() - 1);
    }

    
    Node<T>* create(int l, int h){
        if (l == h)  {
            auto lo = new Node<T>(v[l]);
            
            Node<T> * padre = lo;
            
            padre->left = lo;
            lo->is_leaf = true;
            
            lo->prev = aux;
            
            if(aux) aux->next = lo;
    
            aux = lo;
            
            return padre;
        } 
    
        if (l + 1 == h) {
            auto lo = new Node<T>(v[l]);
            auto hi = new Node<T>(v[h]);

            lo->next = hi;
            hi->prev = lo;

            lo->is_leaf = hi->is_leaf = true;
            
            Node<T> * padre = new Node<T>((v[l] + v[h])/2, lo, hi);

            aux->next = lo;
            lo->prev = aux;

            aux = hi;

                        

            return padre;
        }
   
        int m = (l + h)/2;
        Node<T> * pl = create(l, m);
        Node<T> * ph = create(m + 1, h);
        
        Node<T> * padre = new Node<T>((pl->data + ph->data)/2, pl, ph);
        return padre;
   }
   
   
    void print(Node<T> * r){ 
        if(!r) return;
        print(r->left);
        if(!r->left && !r->right)
            cout<<r->data<<" ";
        print(r->right);
    }
};



int main () {
    std::vector<int> vl {36,37,42,51,62,72};

    RangeTree<int> rt(vl);
    return 0;
}
