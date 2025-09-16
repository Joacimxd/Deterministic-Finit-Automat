#include <iostream>
#include <unordered_map>
using namespace std;

int main() {
    unordered_map<int, string> mapa;

    mapa[1] = "uno";
    mapa[2] = "dos";
    mapa[3] = "tres";

    // eliminar la clave 2
    mapa.erase(4);

    for (auto &par : mapa) {
        cout << par.first << " => " << par.second << endl;
    }
}
