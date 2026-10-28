#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

int N;
int M;
int min_ms = 50;
int max_ms = 300;
int seed = 1;

void leer_config(string archivo) {
    ifstream f(archivo);
    if (!f) {
        cout << "No se pudo abrir " << archivo << endl;
        exit(1);
    }
    string nombre;
    int valor;
    while (f >> nombre >> valor) {
        if (nombre == "min_ms") {
            min_ms = valor;
        } else if (nombre == "max_ms") {
            max_ms = valor;
        } else if (nombre == "seed") {
            seed = valor;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 4) {
        cout << "Uso: ./torneo N M [config.txt]" << endl;
        return 1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    if (N <= 0 || M <= 0) {
        cout << "N y M deben ser mayores a 0" << endl;
        return 1;
    }

    if (argc == 4) {
        leer_config(argv[3]);
    }

    if (min_ms < 0 || min_ms > max_ms) {
        cout << "Retardos invalidos: min_ms debe ser >= 0 y <= max_ms" << endl;
        return 1;
    }

    srand(seed);

    cout << "Jugadores (N): " << N << endl;
    cout << "Mesas maximas (M): " << M << endl;
    cout << "Retardo: " << min_ms << " a " << max_ms << " ms" << endl;
    cout << "Seed: " << seed << endl;

    return 0;
}