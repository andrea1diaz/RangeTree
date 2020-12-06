#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>

using namespace std;

template <typename T, typename M>
class RangeTree;

template <typename T, typename M>
struct Node {
    Node<T, M> *next = nullptr;
    Node<T, M> *prev = nullptr;

    //child nodes
    Node<T, M> *left = nullptr;
    Node<T, M> *right = nullptr;

    //parent
    Node<T, M> *parent = nullptr;

    M data;
    pair<M, M> coord;
    bool is_leaf;
    bool visited = false;

    RangeTree<T, M> *y_tree = nullptr;

    Node () { is_leaf = false; }
    Node (M data_) : data{data_} {  is_leaf = false; };
    Node (M data_, pair<M, M> coord_) : data{data_}, coord{coord_} { is_leaf = false; };
    Node (M data_, Node<T, M>* left_, Node<T, M>* right_) : data{data_}, left{left_}, right{right_} { is_leaf = false; };

    void init_y_tree () { y_tree = new RangeTree<T, M>(); }
};

template <typename T, typename M>
class RangeTree {
    Node<T, M> *root = nullptr;
    unordered_map<int, M> indices;

    Node<T, M> *aux = nullptr;

public:
    std::vector<T> v;


    RangeTree() = default;

    RangeTree(std::vector<T> vl){
        sort(vl.begin(), vl.end());

        for(int i=0; i<vl.size(); ++i)  indices.insert({i, vl[i].second});

        this->v = vl;

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
        std::vector<pair<int, M>> lys;
        int i = l;

        while (i <= m) {
            lys.push_back({indices[i], i});
            i++;
        }
        sort(lys.begin(), lys.end());

        pl->y_tree->root = pl->y_tree->create_y(0, lys.size() - 1, lys, v);

        Node<T, M> *ph = create(m + 1, h);
        ph->init_y_tree();
        std::vector<pair<int, M>> mys;
        i = m + 1;

        while (i <= h) {
            mys.push_back ({indices[i], i});
            i++;
        }
        sort(mys.begin(), mys.end());
        ph->y_tree->root = ph->y_tree->create_y(0, mys.size() - 1, mys, v);

        Node<T, M> *padre = new Node<T, M>((pl->data + ph->data)/2, pl, ph);

        pl->parent = ph->parent = padre;

        padre->init_y_tree();

        std::vector<pair<int, M>> ys;
        i = l;

        while (i <= h) {
            ys.push_back({indices[i], i});
            i++;
        }

        sort(ys.begin(), ys.end());
        padre->y_tree->root = padre->y_tree->create_y(0, ys.size() - 1, ys, v);

        return padre;
    }


    Node<T, M>* create_y (int l, int h, std::vector<pair<int, M>> &ys, std::vector<T> &v){
        if (l == h)  {
            auto lo = new Node<T, M>(ys[l].first, v[ys[l].second]);

            Node<T, M> *padre = new Node<T, M>(ys[l].first);
            lo->parent = padre;

            padre->left = lo;
            lo->is_leaf = true;

            lo->prev = aux;

            if(aux) aux->next = lo;

            aux = lo;

            return padre;
        }

        if (l + 1 == h) {
            auto lo = new Node<T, M>(ys[l].first, v[ys[l].second]);
            auto hi = new Node<T, M>(ys[h].first, v[ys[h].second]);

            lo->next = hi;
            hi->prev = lo;

            lo->is_leaf = hi->is_leaf = true;

            Node<T, M> *padre = new Node<T, M>((ys[l].first + ys[h].first) / 2, lo, hi);

            lo->parent = hi->parent = padre;

            if(aux) aux->next = lo;

            lo->prev = aux;

            aux = hi;

            return padre;
        }

        int m = (l + h) / 2;
        Node<T, M> *pl = create_y(l, m, ys, v);
        Node<T, M> *ph = create_y(m + 1, h, ys, v);
        Node<T, M> *padre = new Node<T, M>((pl->data + ph->data)/2, pl, ph);

        pl->parent = ph->parent = padre;

        return padre;
   }

   Node<T, M>* find (M val) {
        Node<T, M> *it = root;

        while (!it->is_leaf) {
            if (it->data >= val) it = it->left;

            else {
                if (it->right) it = it->right;
                else break;
            }
        }

        if (it->data == val || val < it->data) return it;

        return nullptr;
    }


   std::pair<Node<T, M>*, Node<T, M>*> find (M val, M val1){
       auto it = root;

       while (!it->is_leaf){
           if (it->data >= val) it = it->left;

           else {
                if (it->right) it = it->right;
                else break;
           }
       }

       if (it->data == val) return {it, it->parent->y_tree->find(val1)};

       return {nullptr, nullptr};
   }


    vector<pair<M, M>> range (M left, M right, M down, M top) {
        if (left > right) swap(left, right);
        if (down > top) swap(down, top);
        auto iter_l = find(left);
        auto iter_r = find(right);

        while (iter_l->parent->data != iter_r->parent->data) {
            iter_l = iter_l->parent;
            iter_r = iter_r->parent;
        }

        return iter_l->parent->y_tree->range_y(left, right, down, top);

    }

    std::vector<std::pair<M, M>> range_y (M left, M right, M down, M top) {
        auto d = find(down);

        std::vector<std::pair<M, M>> ret;

        while(d != nullptr && d->data <= top && d->coord.first >= left && d->coord.first <= right) {
            if (d->coord.first >= left && d->coord.first <= right && d->coord.second >= down && d->coord.second <= top)
            ret.push_back(d->coord);

            d = d->next;
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

    ~RangeTree() {
        cout << "Destruyendo el range tree..." << endl;
        destroy(root);
    }

    void destroy(Node<T, M>* node) {
        if (node && !node->visited) {
            destroy(node->y_tree);
            if (node->left) node->left->visited = true; destroy(node->left);
            if (node->right) node->right->visited = true; destroy(node->right);
            if (node->next) node->next->visited = true; destroy(node->next);
            if (node->prev) node->prev->visited = true; destroy(node->prev);
            delete node;
        }
    }

    void destroy(RangeTree<T, M>* tree) {
        if (tree)
            if (tree->root) {
                tree->root->visited = true;
                destroy(tree->root);
            }
    }
};

