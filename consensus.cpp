/*              Bibliotecas que serão usadas no código              */

// Biblioteca que contém o tipo 'uint8_t'.
#include <cstdint>
// Biblioteca que contém o objeto 'vector'.
#include <vector>
// Biblioteca responsável pela entrada e saída.
#include <stdio.h>

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
                void set_color(Color color_to_set): Seta a cor 'c' no nó.
                void set_neighbor(uint8_t neighbor_number): Adiciona um novo vizinho ao nó.
                boolean is_neighbor(uint8_t neighbor_number): Verifica se um dado nó é vizinho.

                OBS: Não serão adicionados métodos get para os atributos, a ideia é que eles sejam acessados diretamente.
                
            private:

    */

    public:
        const uint8_t NUMBER;

        Color color;
        std::vector<uint8_t> neighbors;

        // Repare que a constante NUMBER e o atributo privado 'neighbors_count' são inicializados na lista de inicialização
        // do construtor da classe 'Node', enquanto o atributo público color é inicializado dentro do construtor em questão.
        Node(uint8_t node_number) : NUMBER(node_number), neighbors_count(0){
            color = Configs::OFF;

            //Setamos n como sendo o seu próprio (e primeiro) vizinho.
            neighbors.push_back(NUMBER);
        }

        void set_color(Color color_to_set){
            this->color = color_to_set;
        }

        void set_neighbor(uint8_t neighbor_number){
            this->neighbors.push_back(neighbor_number);
            this->neighbors_count ++;
        }

        bool is_neighbor(uint8_t neighbor_number){
            bool result = false;

            for(uint8_t i = 0; i < this->neighbors.size(); i++){
                if(this->neighbors[i] == neighbor_number){
                    result = true;
                }
            }

            return result;
        }


    private:
        uint8_t neighbors_count;

};

//class Graph{}

/*              Funções que serão usadas para testar o código               */

void show_vector(std::vector<uint8_t> v){
    printf("[ ");
    for(uint8_t i = 0; i < v.size(); i++){
        printf("%d, ", v[i]);
    }
    printf("]\n\n");
}

void run_tests(){
    /*
        Essa função é responsável por rodar alguns testes básicos.
    */

    //Cria um objeto do tipo 'Node' para testes.
    Node n(0);
    
    // Imprime o atributo n.NUMBER
    printf("%d\n\n", n.NUMBER);
     
    // Imprime o atributo n.color
    printf("R: %d, G: %d, B: %d\n\n", n.color.R, n.color.G, n.color.B);
    // Testa o método 'set_color'
    Color color_to_set = {100,0,200};
    n.set_color(color_to_set);
    printf("R: %d, G: %d, B: %d\n\n", n.color.R, n.color.G, n.color.B); 
    
    
    // Testa a inserção de vizinhos (método 'set_neighbor')
    show_vector(n.neighbors);
    for(uint8_t i = 1; i < 10; i++){
        n.set_neighbor(i);
    }
    show_vector(n.neighbors);

    // Testa o método 'is_neighbor'
    printf("%d\n", n.is_neighbor(4)); // == 1.
    printf("%d\n\n", n.is_neighbor(20)); // == 0.   
}


/*              Código principal                */

int main(){

    run_tests();

    return 0;
}