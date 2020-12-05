#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#include "RangeTree.cpp"


int main () {
    std::vector<pair<int, int>> vl;

	// inserto 100 000 puntos en el vector
	for (int i = 0; i < 100000; ++i) {
		int x = rand() % 1000;
		int y = rand() % 1000;

		vl.push_back({x, y});
	}

    RangeTree<pair<int, int>, int> rt(vl);

    auto h = rt.range(14, 37, 10, 50);

    for(auto it: h){
        std::cout << it->coord.first << " " << it->coord.second << std::endl;
    }
    return 0;
}
