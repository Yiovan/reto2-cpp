#include <iostream>
#include <vector>

using namespace std;

const int FILAS = 11;
const int COLUMNAS = 11;

int main() {
    // Crea una matriz (grid) para almacenar el laberinto
    // Inicialmente, haremos que todo sea una pared ('#')
    char laberinto[FILAS][COLUMNAS];

    // Llenar toda la rejilla con paredes
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            laberinto[i][j] = '#';
        }
    }

    // --- ¡Aquí iría la LÓGICA del algoritmo de generación (DFS)! ---
    // (Esta es la parte compleja que "excava" los caminos)
    
    // Dejamos un pequeño camino visible para demostrar la impresión
    laberinto[1][1] = ' ';
    laberinto[1][2] = ' ';
    laberinto[2][2] = ' ';
    laberinto[3][2] = ' ';
    laberinto[3][3] = ' ';
    laberinto[4][3] = ' ';
    
    // Imprimir el laberinto
    cout << "--- Laberinto ---" << endl;
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            cout << laberinto[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}