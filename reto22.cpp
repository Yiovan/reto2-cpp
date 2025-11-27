                    #include <iostream>
                    #include <vector>
                    #include <stack>   
                    #include <cstdlib> 
                    #include <ctime>   
                    #include <conio.h> // Para _kbhit() y _getch() en Windows
                    #include <stdlib.h> // Para system("cls") en Windows

                    #ifdef _WIN32 // Comprobación para sistemas Windows
                    #include <windows.h>
                    #else // Para sistemas basados en Unix (Linux, macOS)
                    #include <unistd.h>
                    #include <termios.h>
                    #endif

                    using namespace std;    

                    // Dimensiones del laberinto (preferiblemente IMPARES)
                    const int FILAS = 21;
                    const int COLUMNAS = 21;

                    // Caracteres para representar el laberinto
                    const char PARED = '#';
                    const char CAMINO = ' ';
                    const char RATON = 'R'; // Carácter para el Ratón
                    const char GATO = 'G'; // Carácter para el Gato

                    // Estructura para representar una celda (coordenadas)
                    struct Celda {
                        int fila, col;
                    };

                    // --- Funciones para la Plataforma (Manejo de Entrada y Limpieza de Pantalla) ---

                    // Configuración de entrada no bloqueante para plataformas Unix/Linux/macOS
                    #ifndef _WIN32
                    void setup_terminal() {
                        struct termios t;
                        tcgetattr(STDIN_FILENO, &t);
                        t.c_lflag &= ~(ICANON | ECHO); // Desactiva modo canónico y eco
                        tcsetattr(STDIN_FILENO, TCSANOW, &t);
                    }

                    // Restaura la configuración de entrada
                    void restore_terminal() {
                        struct termios t;
                        tcgetattr(STDIN_FILENO, &t);
                        t.c_lflag |= (ICANON | ECHO);
                        tcsetattr(STDIN_FILENO, TCSANOW, &t);
                    }

                    // Simulación de _kbhit y _getch
                    int _kbhit() {
                        struct timeval tv = {0L, 0L};
                        fd_set fds;
                        FD_ZERO(&fds);
                        FD_SET(STDIN_FILENO, &fds);
                        return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
                    }

                    char _getch() {
                        char c;
                        read(STDIN_FILENO, &c, 1);
                        return c;
                    }
                    #endif

                    void limpiar_pantalla() {
                    #ifdef _WIN32
                        system("cls");
                    #else
                        system("clear");
                    #endif
                    }

                    // --- Funciones de Generación de Laberinto (Sin cambios) ---

                    // Función para verificar si una celda está dentro de los límites del laberinto
                    bool esValida(int r, int c) {
                        return r >= 0 && r < FILAS && c >= 0 && c < COLUMNAS;
                    }

                    // Algoritmo de generación de laberintos (DFS)
                    void generarLaberinto(vector<vector<char>>& laberinto, int inicioF, int inicioC) {
                        stack<Celda> pila;
                        pila.push({inicioF, inicioC});

                        laberinto[inicioF][inicioC] = CAMINO;

                        int dr[] = {-2, 2, 0, 0};
                        int dc[] = {0, 0, -2, 2};

                        while (!pila.empty()) {
                            Celda actual = pila.top();
                            
                            vector<int> vecinosDisponibles; 

                            for (int i = 0; i < 4; ++i) {
                                int nuevaF = actual.fila + dr[i];
                                int nuevaC = actual.col + dc[i];

                                if (esValida(nuevaF, nuevaC) && laberinto[nuevaF][nuevaC] == PARED) {
                                    vecinosDisponibles.push_back(i);
                                }
                            }

                            if (!vecinosDisponibles.empty()) {
                                int indiceAleatorio = rand() % vecinosDisponibles.size();
                                int direccionElegida = vecinosDisponibles[indiceAleatorio];

                                int nuevaF = actual.fila + dr[direccionElegida];
                                int nuevaC = actual.col + dc[direccionElegida];

                                int paredF = actual.fila + dr[direccionElegida] / 2;
                                int paredC = actual.col + dc[direccionElegida] / 2;

                                laberinto[paredF][paredC] = CAMINO;
                                laberinto[nuevaF][nuevaC] = CAMINO;
                                pila.push({nuevaF, nuevaC});
                            } else {
                                pila.pop();
                            }
                        }
                    }

                    // --- Funciones de Juego ---

                    /**
                     * Mueve el ratón según la entrada del usuario.
                     * @param laberinto El mapa del laberinto.
                     * @param ratonF Fila actual del ratón (pasada por referencia).
                     * @param ratonC Columna actual del ratón (pasada por referencia).
                     * @param dir La dirección de movimiento ('w', 'a', 's', 'd').
                     */
                    void moverRaton(const vector<vector<char>>& laberinto, int& ratonF, int& ratonC, char dir) {
                        int nuevaF = ratonF;
                        int nuevaC = ratonC;

                        switch (dir) {
                            case 'w': nuevaF--; break; // Arriba
                            case 's': nuevaF++; break; // Abajo
                            case 'a': nuevaC--; break; // Izquierda
                            case 'd': nuevaC++; break; // Derecha
                            default: return; // Ignora otras teclas
                        }

                        // Comprobar si la nueva posición es válida y no es una pared
                        if (esValida(nuevaF, nuevaC) && laberinto[nuevaF][nuevaC] != PARED) {
                            ratonF = nuevaF;
                            ratonC = nuevaC;
                        }
                    }

                    /**
                     * Mueve el gato con una IA básica de persecución (un paso más cerca).
                     * @param laberinto El mapa del laberinto.
                     * @param gatoF Fila actual del gato (pasada por referencia).
                     * @param gatoC Columna actual del gato (pasada por referencia).
                     * @param ratonF Fila actual del ratón.
                     * @param ratonC Columna actual del ratón.
                     */
                    void moverGato(const vector<vector<char>>& laberinto, int& gatoF, int& gatoC, int ratonF, int ratonC) {
                        int difF = ratonF - gatoF;
                        int difC = ratonC - gatoC;
                        int nuevaF = gatoF;
                        int nuevaC = gatoC;

                        // Array para las direcciones
                        int dr[] = {-1, 1, 0, 0}; // Arriba, Abajo, Izquierda, Derecha
                        int dc[] = {0, 0, -1, 1};

                        // Prioriza moverse en la dirección con mayor diferencia (más cerca del ratón)
                        int mejorDireccion = -1;
                        double minDistancia = 1e9; // Distancia cuadrática

                        // Probar las 4 direcciones (Arriba, Abajo, Izquierda, Derecha)
                        for (int i = 0; i < 4; ++i) {
                            int tempF = gatoF + dr[i];
                            int tempC = gatoC + dc[i];

                            if (esValida(tempF, tempC) && laberinto[tempF][tempC] != PARED) {
                                // Calcular la nueva distancia cuadrática al ratón (evita la raíz cuadrada)
                                double nuevaDistancia = pow(ratonF - tempF, 2) + pow(ratonC - tempC, 2);

                                if (nuevaDistancia < minDistancia) {
                                    minDistancia = nuevaDistancia;
                                    mejorDireccion = i;
                                }
                            }
                        }

                        if (mejorDireccion != -1) {
                            gatoF += dr[mejorDireccion];
                            gatoC += dc[mejorDireccion];
                        } else {
                            // Si no puede moverse en la dirección óptima, puede quedarse quieto o moverse aleatoriamente.
                            // Aquí lo dejamos quieto.
                        }
                    }

                    /**
                     * Imprime el laberinto, incluyendo la posición del ratón y el gato.
                     */
                    void imprimirLaberintoConPersonajes(const vector<vector<char>>& laberinto, int ratonF, int ratonC, int gatoF, int gatoC) {
                        for (int i = 0; i < FILAS; ++i) {
                            for (int j = 0; j < COLUMNAS; ++j) {
                                if (i == ratonF && j == ratonC) {
                                    cout << RATON << " ";
                                } else if (i == gatoF && j == gatoC) {
                                    cout << GATO << " ";
                                } else {
                                    cout << laberinto[i][j] << " ";
                                }
                            }
                            cout << endl;
                        }
                    }


                    int main() {
                        srand(time(0)); 

                    #ifndef _WIN32
                        setup_terminal(); // Configura la terminal para entrada no bloqueante en Unix
                    #endif

                        // 1. Generación del Laberinto
                        vector<vector<char>> laberinto(FILAS, vector<char>(COLUMNAS, PARED));
                        generarLaberinto(laberinto, 1, 1); 

                        // 2. Definición de Puntos Clave
                        const Celda ENTRADA = {0, 1};
                        const Celda SALIDA = {FILAS - 1, COLUMNAS - 2};
                        
                        laberinto[ENTRADA.fila][ENTRADA.col] = CAMINO; 
                        laberinto[SALIDA.fila][SALIDA.col] = CAMINO; 

                        // 3. Posiciones Iniciales del Gato y Ratón
                        int ratonF = ENTRADA.fila;
                        int ratonC = ENTRADA.col;

                        // Coloca el gato cerca de la salida o en un punto aleatorio lejos del ratón
                        int gatoF = FILAS - 2; 
                        int gatoC = COLUMNAS - 2;

                        // 4. Bucle Principal del Juego
                        bool juegoTerminado = false;
                        char input = ' ';

                        cout << "=== Laberinto del Ratón y el Gato ===" << endl;
                        cout << "Controles: W (Arriba), A (Izquierda), S (Abajo), D (Derecha). Q para salir." << endl;
                        cout << "Meta: Llega a la salida (S) sin que te atrape el Gato (G)." << endl;

                        while (!juegoTerminado) {
                            limpiar_pantalla();
                            imprimirLaberintoConPersonajes(laberinto, ratonF, ratonC, gatoF, gatoC);

                            // A. Entrada del Usuario (Movimiento del Ratón)
                            if (_kbhit()) {
                                input = _getch();
                                input = tolower(input); // Convertir a minúscula

                                if (input == 'q') {
                                    juegoTerminado = true;
                                } else {
                                    moverRaton(laberinto, ratonF, ratonC, input);
                                }
                            }

                            // B. Lógica del Gato (Se mueve cada N ticks o después del ratón)
                            // Para que el juego sea más jugable, el gato se mueve solo si el ratón se movió.
                            static int contadorMovimientoGato = 0;
                            const int VELOCIDAD_GATO = 2; // El gato se mueve cada 2 movimientos del ratón

                            if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
                                contadorMovimientoGato++;
                                if (contadorMovimientoGato >= VELOCIDAD_GATO) {
                                    moverGato(laberinto, gatoF, gatoC, ratonF, ratonC);
                                    contadorMovimientoGato = 0;
                                }
                                input = ' '; // Reinicia el input para el siguiente ciclo
                            }

                            // C. Condición de Victoria
                            if (ratonF == SALIDA.fila && ratonC == SALIDA.col) {
                                cout << "\n¡FELICIDADES! ¡El Ratón ha escapado!" << endl;
                                juegoTerminado = true;
                            }

                            // D. Condición de Derrota
                 if (ratonF == gatoF && ratonC == gatoC) {
                 cout << "\n¡GAME OVER! El Gato ha atrapado al Ratón." << endl;
                    juegoTerminado = true;
                            }

                    #ifdef _WIN32
                            Sleep(100); // Pequeña pausa para reducir el uso de CPU y estabilidad en Windows
                    #else
                            usleep(100000); // 100 milisegundos en Unix
                    #endif
                        }

                    #ifndef _WIN32
                        restore_terminal(); // Restaura la terminal en Unix
                    #endif

                        return 0;
                    }