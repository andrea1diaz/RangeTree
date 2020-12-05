#include <iostream>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <algorithm>
#include <random>

#include "RangeTree.cpp"

const int N = 10000;

using namespace std::chrono; 

int main () {
    std::vector<pair<int, int>> vl;

	// inserto 100 000 puntos en el vector
	for (int i = 0; i < 100000; ++i) {
		int x = rand() % 1000;
		int y = rand() % 1000;

		vl.push_back({x, y});
	}

    RangeTree<pair<int, int>, int> rt(vl);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto rng = std::default_random_engine (seed);
    std::shuffle(begin(vl), end(vl), rng);
    vector<pair<int, int>> busqueda (vl.begin(), vl.begin() + N);

    auto start = high_resolution_clock::now(); 
    bool bien = true;

    for (int i = 0; i < busqueda.size(); ++i) {
        int x1 = busqueda[i].first;
        int y1 = busqueda[i].second;
        int i_r = rand()%busqueda.size();
        int x2 = busqueda[i_r].first;
        int y2 = busqueda[i_r].second;

        auto r = rt.range(x1, x2, y1, y2);

        vector<pair<int, int>> chequeador;
        for (auto it : vl) {
            if (it.first > x1 && it.first < x2 && it.second > y1 && it.second < y2)  chequeador.push_back(it);
        }

        for(auto it : vl){
            if (std::find(r.begin(), r.end(), it) == r.end()) {
                bien = false;
            }
        }
    }

    auto stop = high_resolution_clock::now(); 

    if (bien) {
        cout << "Las busquedas fueron exitosas." << endl;
    } else {
        cout << "Se cometieron errores." << endl;
    }

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Tiempo: " << duration.count() << " ms (N = " << N << ")." << endl; 

    return 0;
}
