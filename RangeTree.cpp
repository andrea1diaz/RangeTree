#include <iostream>
#include <utility>
#include <vector>

using namespace std;

template <typename T, typename M>
class RangeTree;

template <typename T, typename M>
struct Node {
    Node<T, M> *next;
    Node<T, M> *prev;

    //child nodes
    Node<T, M> *left;
    Node<T, M> *right;

    //parent
    Node<T, M> *parent;

    M data;
    pair<M, M> coord;
    bool is_leaf;

    RangeTree<T, M> *y_tree;

    Node () { is_leaf = false; }
    Node (M data_) : data{data_} {  is_leaf = false; };
    Node (M data_, pair<M, M> coord_) : data{data_}, coord{coord_} { is_leaf = false; };
    Node (M data_, Node<T, M>* left_, Node<T, M>* right_) : data{data_}, left{left_}, right{right_} { is_leaf = false; };

    void init_y_tree () { y_tree = new RangeTree<T, M>(); }
};

template <typename T, typename M>
class RangeTree {
    Node<T, M> *root;

    Node<T, M> *aux = nullptr;

public:
std::vector<T> v;
    RangeTree() {}

    RangeTree(std::vector<T> v){
        this->v = v;

        root = create(0, v.size() - 1);
    }


    Node<T, M>* create(int l, int h){
        if (l == h)  {
            auto lo = new Node<T, M>(v[l].first);

            Node<T, M> *padre = new Node<T, M>(v[l].first);
            lo->parent = padre;

            padre->left = lo;
            lo->is_leaf = true;

            lo->prev = aux;

            if(aux) aux->next = lo;

            aux = lo;

            return padre;
        }

        if (l + 1 == h) {
            auto lo = new Node<T, M>(v[l].first);
            auto hi = new Node<T, M>(v[h].first);

            lo->next = hi;
            hi->prev = lo;

            lo->is_leaf = hi->is_leaf = true;

            Node<T, M> *padre = new Node<T, M>((v[l].first + v[h].first)/2, lo, hi);

            lo->parent = hi->parent = padre;

            if(aux) aux->next = lo;

            lo->prev = aux;

            aux = hi;

            return padre;
        }

        int m = (l + h)/2;
        Node<T, M> *pl = create(l, m);
        pl->init_y_tree();
        pl->y_tree->root = pl->y_tree->create_y(l, m, v);

        Node<T, M> *ph = create(m + 1, h);
        ph->init_y_tree();
        ph->y_tree->root = ph->y_tree->create_y(m + 1, h, v);

        Node<T, M> *padre = new Node<T, M>((pl->data + ph->data)/2, pl, ph);

        pl->parent = ph->parent = padre;

        padre->init_y_tree();
        padre->y_tree->root = padre->y_tree->create_y(l, h, v);

        return padre;
    }


    Node<T, M>* create_y(int l, int h, vector<T> v){
        if (l == h)  {
            auto lo = new Node<T, M>(v[l].second, v[l]);

            Node<T, M> *padre = new Node<T, M>(v[l].second);
            lo->parent = padre;

            padre->left = lo;
            lo->is_leaf = true;

            lo->prev = aux;

            if(aux) aux->next = lo;

            aux = lo;

            return padre;
        }

        if (l + 1 == h) {
            auto lo = new Node<T, M>(v[l].second, v[l]);
            auto hi = new Node<T, M>(v[h].second, v[h]);

            lo->next = hi;
            hi->prev = lo;

            lo->is_leaf = hi->is_leaf = true;

            Node<T, M> *padre = new Node<T, M>((v[l].second + v[h].second) / 2, lo, hi);

            lo->parent = hi->parent = padre;

            if(aux) aux->next = lo;

            lo->prev = aux;

            aux = hi;

            return padre;
        }

        int m = (l + h) / 2;
        Node<T, M> *pl = create_y(l, m, v);
        Node<T, M> *ph = create_y(m + 1, h, v);
        Node<T, M> *padre = new Node<T, M>((pl->data + ph->data)/2, pl, ph);

        pl->parent = ph->parent = padre;

        return padre;
   }

   Node<T, M>* find (M val) {
       auto it = root;

       while (!it->is_leaf){
           if (it->data >= val) it = it->left;

           else it = it->right;
       }

       if (it->data == val) return it;

       std::cout << "Not found\n";
       return nullptr;
   }


   std::pair<Node<T, M>*, Node<T, M>*> find (M val, M val1){
       auto it = root;

       while (!it->is_leaf){
           if (it->data >= val) it = it->left;

           else it = it->right;
       }

       if (it->data == val) return {it, it->parent->y_tree->find(val1)};

       std::cout << "Not found\n";
       return {nullptr, nullptr};
   }


   vector<Node<T, M>*> range (M left, M right, M down, M top) {
        auto iter_l = find(left);
        auto iter_r = find(right);

        while (iter_l->parent->data != iter_r->parent->data) {
            iter_l = iter_l->parent;
            iter_r = iter_r->parent;
        }

        return iter_l->parent->y_tree->range(down, top);

   }

   std::vector<Node<T, M>*> range (M left, M right){
        auto l = find(left);

        std::vector<Node<T, M>*> ret;

        while(l != nullptr && l->data <= right) {
            ret.push_back(l);

            l = l->next;
        }

        return ret;
   }




    void print(Node<T, M> * r) {
        if(!r) return;
        print(r->left);
        if(!r->left && !r->right)
            cout<<r->data<<" ";
        print(r->right);
    }
};
