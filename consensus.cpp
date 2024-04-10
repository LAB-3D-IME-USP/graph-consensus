/*              Bibliotecas que serão usadas no código              */

// Biblioteca que contém o tipo 'uint8_t'.
#include <cstdint>


/*              Structs que serão usadas no código              */

// Struct responsável por armazenar uma cor RGB.
struct Color{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

// Struct responsável por armazenar algumas constantes que serão necessárias no código.
struct Configs{
    //
    static const uint8_t NODES_NUMBER;

    //
    static const Color COLOR_1;
    static const Color COLOR_2;
    static const Color OFF;
};


/*              Constantes que serão utilizadas no código               */

// Inicialização de membros estáticos da struct Configs fora da definição da classe.
const uint8_t Configs::NODES_NUMBER = 100;
const Color Configs::COLOR_1 = {255, 0, 0};
const Color Configs::COLOR_2 = {0, 0, 255};
const Color Configs::OFF = {0, 0, 0};


/*              Classes que serão usadas no código              */
class Node{
    /*
        Descrição:
            Essa classe é responsável por implementar uma abstração específica de um nó que pertence a um grafo.
        
        Atributos:
            public:
                NUMBER (const uint8_t): Constante numérica inteira que representa o número do nó.
                color (Color): Struct do tipo 'Color' que representa a cor atual que o nó possui.
                neighbors (vector<uint8_t>): Lista de inteiros que indica quais são os vizinhos do nó.
            private:
                neighbors_count (uint8_t): Valor númerico que representa a quantidade de vizinhos do nó.

        Métodos:
            public:
                get_color(): Retorna a cor atual do nó.
                set_color(Color c): Seta a cor 'c' no nó.
                get_neighbors(): Retorna um objeto do tipo vector<uint8_t> que contém os vizinhos do nó.
                set_neighbors(uint8_t): Adiciona um novo vizinho ao nó.
            private:

    */
};

//class Graph{}

