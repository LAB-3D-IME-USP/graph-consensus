// TO-DO
//  1 - Comentar o código
//  2 - Ajustar o operador '=' na classe Node, (tá dando problema com a constante NUMBER)

/*              Bibliotecas que serão usadas no código              */

// Biblioteca que contém o tipo 'uint8_t'.
#include <cstdint>
// Biblioteca que contém o objeto 'vector'.
#include <vector>
// Bibliotecas responsáveis pela entrada e saída.
#include <stdio.h>
#include <iostream>
// Biblioteca responsável por fornecer os métodos de manipulação de string que serão usados nesse código.
#include <string>

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
                number (uint8_t):  número inteiro que representa o nó.
                color (Color): Struct do tipo 'Color' que representa a cor atual que o nó possui.
                neighbors (vector<uint8_t>): Array de inteiros que indica quais são os vizinhos do nó.
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
        uint8_t NUMBER;

        Color color;
        std::vector<uint8_t> neighbors;

        // Cria um construtor personalizado. Repare que, caso não seja passado nenhum parâmetro para o construtor da classe 'Node',
        // o construtor padrão criará um objeto do tipo 'Node' que possui o número 255.
        // Repare que a constante NUMBER e o atributo privado 'neighbors_count' são inicializados no array de inicialização
        // do construtor da classe 'Node', enquanto o atributo público color é inicializado dentro do construtor em questão.
        Node(uint8_t node_number = 255) : NUMBER(node_number), color(Configs::OFF), neighbors_count(0){
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
            for(uint8_t i = 0; i < this->neighbors.size(); i++){
                if(this->neighbors[i] == neighbor_number){
                    return true;
                }
            }
            return false;
        }

        // Sobrecarga do operador de inserção.
        friend std::ostream& operator<<(std::ostream& os, const Node& node) {
            os << "Node NUMBER: " << static_cast<int>(node.NUMBER) << std::endl; // Imprime o número do nó
            os << "Node Color (R,G,B): " << static_cast<int>(node.color.R) << "," 
                                        << static_cast<int>(node.color.G) << "," 
                                        << static_cast<int>(node.color.B) << std::endl; // Imprime a cor do nó
            os << "Node neighbors: " << node.get_neighbors_as_string() << std::endl;
            return os;
        }


    private:
        uint8_t neighbors_count;

        // Comentar sobre essa função e sobre o porque de usar const nela.
        std::string get_neighbors_as_string() const {
            std::string neighbors = "[";
            for(uint8_t i = 0; i < this->neighbors.size(); i++){
                neighbors += std::to_string(this->neighbors[i]) + ", ";
            }

            // Remove o último espaço e a última vírgula
            neighbors.pop_back();
            neighbors.pop_back();

            neighbors += "]";
            
            return neighbors;
        }

};

class Graph{
    /*
        Descrição:
            Essa classe é responsável por implementar a abstração de um grafo. Para essa classe, consideraremos um grafo como sendo um array
            fixa de nós.
        Atributos:
            public:
                NODES_NUMBER (const uint8_t): Variável que guardará a quantidade de nós que o grafo terá, isto é, a quantidade de elementos
                do array que representa o grafo.
                nodes (Node[]): Array de objetos do tipo 'Node' que conterá todos os nós do grafo. 
            private:
                current_nodes_number (uint8_t): Variável que guardará a quantidade atual de nós que o grafo possui, isto é, a quantidade de 
                elementos que o array que representa o grafo possui atualmente.
        Métodos:
            public:
                void set_node(Node node): Método responsável por adicionar um nó ao grafo, isto é, adicionar um objeto do tipo 'Node' o array
                que representa o grafo.
            private:
    */

    public:

        static const uint8_t NODES_NUMBER = Configs::NODES_NUMBER;

        Node nodes[NODES_NUMBER];

        // Cria um construtor para a classe 'Graph'.
        Graph() : current_nodes_number(0){}

        // Método responsável por adicionar um nó ao grafo.
        void set_node(const Node& node){
            if(current_nodes_number < NODES_NUMBER){
                this->nodes[current_nodes_number] = node;
                current_nodes_number++;
            }else{
                printf("Erro, o grafo já atingiu a sua quantidade máxima de nós.");
            }   
        }
    
    private:
        uint8_t current_nodes_number;


};

/*              Funções que serão usadas para testar o código               */

void run_node_tests(){
    /*
        Essa função é responsável por rodar alguns testes básicos para objetos do tipo 'Node'.
    */

    //Cria um objeto do tipo 'Node' para testes.
    Node n(0);
    
    // Imprime o atributo n.NUMBER.
    printf("%d\n\n", n.NUMBER);
     
    // Imprime o atributo n.color.
    printf("R: %d, G: %d, B: %d\n\n", n.color.R, n.color.G, n.color.B);
    // Testa o método 'set_color'.
    Color color_to_set = {100,0,200};
    n.set_color(color_to_set);
    printf("R: %d, G: %d, B: %d\n\n", n.color.R, n.color.G, n.color.B); 
    
    
    // Testa a inserção de vizinhos (método 'set_neighbor').
    std::cout << n << std::endl;
    for(uint8_t i = 1; i < 10; i++){
        n.set_neighbor(i);
    }
    std::cout << n << std::endl;

    // Testa o método 'is_neighbor'.
    printf("%d\n", n.is_neighbor(4)); // == 1.
    printf("%d\n\n", n.is_neighbor(20)); // == 0.   
}

void run_graph_tests(){
    /*
        Essa função é responsável por rodar alguns testes básicos para objetos do tipo 'Graph'.
    */

    //Cria um objeto do tipo 'Graph'
    Graph G;
    
    // Cria 10 nós e os adiciona ao grafo G, isto é, testa o método set_node().
    for(uint8_t i = 0; i < 10; i++){
        Node n(i);
        G.set_node(n);
        std::cout << G.nodes[i] << std::endl; // Imprime o objeto Node
    }
}

/*              Código principal                */

int main(){

    //run_node_tests();
    run_graph_tests();

    return 0;
}