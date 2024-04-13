// TO - DO

// Talvez seja interessante adicionar métodos get a classe Graph, para evitar, por exemplo, que se troque a cor diretamente no atributo nodes[i] e não faça o mesmo
// no atributo nodes_color[i].

// Tomar cuidado na hora de setar cor no algoritmo de simulação, eu devo lembrar que apenas objetos do tipo 'Node' tem o atributo set_color, ou seja, 
// quando eu for lidar com objetos do tipo CRGB, como é o caso dos objetos do array G.nodes_color (Para um Graph G), eu devo setar a cor manualmente, isto é,
// G.nodes_color[i] = CRGB(r,g,b)


// Biblioteca responsável por fornecer o tipo 'uint8_t'.
#include <cstdint>
// Biblioteca responsável por fornecer o tipo 'vector'.
#include <vector>
// Bibliotecas responsáveis pela entrada e saída de dados.
#include <stdio.h>
#include <iostream>
// Biblioteca responsável por fornecer os métodos de manipulação de string que serão usados nesse código.
#include <string>
// Biblioteca responsável por fornecer os métodos necessários para controlar os leds.
#include <FastLED.h>


/*              Structs que serão usadas no código              */


// Struct responsável por armazenar algumas constantes que serão necessárias no código.
struct Configs{
    // Constante responsável por setar a quantidade de nós que um objeto do tipo 'Graph' terá.
    static const uint8_t NODES_NUMBER; // Perceba que, essa constante também setará, por consequência, o número de leds.
    // Constante responsável por setar o pino que enviará os dados para a placa.
    static const uint8_t LED_PIN;
    // Constante responsável por setar o valor de uma das cores que os leds podem assumir.
    static const CRGB COLOR_1;
    // Constante responsável por setar o valor de uma das cores que os leds podem assumir.
    static const CRGB COLOR_2;
    // Constante responsável por setar a "cor apagada" aos leds.
    static const CRGB OFF;
};

/*              Constantes que serão utilizadas no código               */
// Inicializa os membros estáticos da struct Configs fora da definição da struct.
const uint8_t Configs::NODES_NUMBER = 100;
const uint8_t Configs::LED_PIN = 12;
const CRGB Configs::COLOR_1 = CRGB(255, 0, 0);
const CRGB Configs::COLOR_2 = CRGB(0, 255, 0);
const CRGB Configs::OFF = CRGB(0, 0, 0);


/*        Declaração da assinatura das funções auxiliares       */


/*              Classes que serão usadas no código              */
class Node{
    /*
        Descrição:
            Essa classe é responsável por implementar uma abstração específica de um nó que representa o vértice de um grafo.
        
        Atributos:
            public:
                number (uint8_t): Número inteiro que representa o nó.
                color (CRGB): Struct do tipo 'CRGB' que representa a cor atual que o nó possui.
                neighbors (vector<uint8_t>): Array de inteiros que indica quais são os vizinhos do nó, isto é, que indica quais nós estão 
                                             conectados a esse nó.
            private:
                neighbors_count (uint8_t): Valor inteiro que representa a quantidade de vizinhos do nó.

        Métodos:
            public:
                void set_color(CRGB color_to_set): Seta a cor 'c' no nó.
                void set_neighbor(uint8_t neighbor_number): Adiciona um novo vizinho ao nó.
                boolean is_neighbor(uint8_t neighbor_number): Verifica se um dado nó é vizinho.

                std::ostream& operator<<(std::ostream& os, const Node& node): Cria uma visualização para objetos do tipo 'Node'.

                OBS: Não serão adicionados métodos get para os atributos, a ideia é que eles sejam acessados diretamente.
                
            private:
                std::string get_neighbors_as_string(): Retorna uma string que mostra os vizinhos do nó seguindo o formato '[vizinho_1,...,vizinho_N]'.
                                                       Esse método auxilia na construção do método que sobrecarrega o operador '<<'.
    */

    public:
        uint8_t number;
        CRGB color;
        std::vector<uint8_t> neighbors;

        // Cria um construtor personalizado. Repare que, caso não seja passado nenhum parâmetro para o construtor da classe 'Node',
        // o construtor padrão criará um objeto do tipo 'Node' que possui o 'Node'.number == 255.
        // Repare que os atributos públicos number e color, além do atributo privado 'neighbors_count' são inicializados no array de inicialização
        // do construtor da classe 'Node', enquanto o atributo público neighbors é inicializado dentro do construtor em questão.
        Node(uint8_t node_number = 255) : number(node_number), color(Configs::OFF), neighbors_count(0){
            //Setamos o nó como sendo o seu próprio (e primeiro) vizinho.
            neighbors.push_back(number);
        }

        // Método responsável por setar uma nova cor em objetos do tipo 'Node'.
        void set_color(CRGB color_to_set){
            this->color = color_to_set;
        }

        // Método responsável por setar um novo vizinho a um objeto do tipo 'Node'.
        void set_neighbor(uint8_t neighbor_number){
            this->neighbors.push_back(neighbor_number);
            this->neighbors_count ++;
        }

        // Método responsável por checar se um dado nó é vizinho.
        bool is_neighbor(uint8_t neighbor_number){
            for(uint8_t i = 0; i < this->neighbors.size(); i++){
                if(this->neighbors[i] == neighbor_number){
                    return true;
                }
            }
            return false;
        }

        // Método responsável por gerar uma visualização de objetos do tipo 'Node'.
        friend std::ostream& operator<<(std::ostream& os, const Node& node) {
            // Imprime o número do nó.
            os << "Node number: " << static_cast<int>(node.number) << std::endl; 
            // Imprime a cor RGB do nó.
            os << "Node Color (R,G,B): " << static_cast<int>(node.color.r) << "," 
                                        << static_cast<int>(node.color.g) << "," 
                                        << static_cast<int>(node.color.b) << std::endl; 
            // Imprime os vizinhos do nó.
            os << "Node neighbors: " << node.get_neighbors_as_string() << std::endl;

            return os;
        }


    private:
        uint8_t neighbors_count;

        // Repare que a função abaixo é setada como 'const', isso quer dizer que essa função não modificará o estado do objeto em que ela é chamada.
        // Método responsável por retornar uma string que contém uma representação que exibe os atributos de um objeto do tipo 'Node'.
        std::string get_neighbors_as_string() const {
            std::string neighbors_representation = "[";
            for(uint8_t i = 0; i < this->neighbors.size(); i++){
                neighbors_representation += std::to_string(this->neighbors[i]) + ", ";
            }

            // Remove o último espaço e a última vírgula.
            neighbors_representation.pop_back();
            neighbors_representation.pop_back();

            neighbors_representation += "]";
            
            return neighbors_representation;
        }

};

class Graph{
    /*
        Descrição:
            Essa classe é responsável por implementar a abstração de um grafo. Para essa classe, consideraremos um grafo como sendo um array
            fixo de nós (vértices).
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
                friend std::ostream& operator<<(std::ostream& os, const Graph& graph): Método responsável por gerar uma representação para o grafo.

            private:
                N/A
    */          

    public:

        static const uint8_t NODES_NUMBER = Configs::NODES_NUMBER;
        Node nodes[NODES_NUMBER];
        CRGB nodes_color[NODES_NUMBER];

        // Cria um construtor para a classe 'Graph'.
        Graph() : current_nodes_number(0){}

        // Método responsável por adicionar um nó ao grafo e também por adicionar a cor desse nó ao vetor que guarda as cores de todos os nós do grafo.
        void set_node(const Node& node){
            if(current_nodes_number < NODES_NUMBER){
                this->nodes[current_nodes_number] = node;
                nodes_color[current_nodes_number] = node.color;
                current_nodes_number++;
            }else{
                printf("Erro, o grafo já atingiu a sua quantidade máxima de nós.");
            }   
        }

        // Método responsável por adicionar uma visualização a objetos do tipo 'Graph'.
        // Perceba que o método abaixo utiliza a visualização gerada para objetos do tipo 'Node'.
        friend std::ostream& operator<<(std::ostream& os, const Graph& graph) {
            os << "\nGraph nodes: {\n" << std::endl;
            for (uint8_t i = 0; i < graph.current_nodes_number; i++) {
                os << graph.nodes[i] << std::endl;
            }
            os << "}" << std::endl;
            
            return os;
        }
    
    private:
        uint8_t current_nodes_number;
};


/*        Variáveis globais usadas no código        */
Graph G;


/*        Setup da placa        */
void setup() {
  // Com base na configuração abaixo, sempre que usarmos 'FastLED.show()' as cores que estiverem em 'G.nodes_color' serão as cores que irão ser exibidas
  // nos leds. Por conta disso, convencionaremos na hora de implementarmos a simulação que o array 'G.nodes' será o array que guardará o estado antigo dos
  // leds, enquanto o array 'G.nodes_color' será o array que guardará o estado novo dos leds. Dito isso, É IMPORTANTE RESSALTAR QUE 'G_nodes_color' e 'G.nodes'
  // são estruturas de dados diferentes, já que, 'G_nodes' é um array de objetos do tipo 'Node', enquanto 'G_nodes_color' é um array de objetos do tipo 'CRGB'.
  // Contudo, como todo 'G_nodes[i]' possui o atributo color que é do tipo 'CRGB', podemos operar com os dois arrays citados como sendo os vetores de estado.
  FastLED.addLeds<NEOPIXEL, Configs::LED_PIN>(G.nodes_color, Configs::NODES_NUMBER); // Inicializa a biblioteca FastLED
}

void loop() {

}


/*        Funções auxiliares        */