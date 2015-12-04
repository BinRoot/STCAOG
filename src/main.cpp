#include <iostream>
#include "ClothAOG.h"

using namespace std;
int main() {
    ClothAOG aog("my_aog");
    cout << "# of spatial nodes: " << aog.num_spatial() << endl;
    cout << "# of temporal nodes: " << aog.num_temporal() << endl;
    cout << "# of causal nodes: " << aog.num_causal() << endl;
    cout << "# of fluent nodes: " << aog.num_fluent() << endl;
}