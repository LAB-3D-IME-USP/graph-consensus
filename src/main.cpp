// Warning!
// Tomar cuidado na hora de setar cor no algoritmo de simulação, eu devo lembrar que apenas objetos do tipo 'Node' tem o atributo set_color, ou seja, 
// quando eu for lidar com objetos do tipo CRGB, como é o caso dos objetos do array G.nodes_color (Para um Graph G), eu devo setar a cor manualmente, isto é,
// G.nodes_color[i] = CRGB(r,g,b)

//TO - DO AND ALERTS! [LCD && KEYPAD]        
// Trocar 'byte' por 'uint8_t'
// Fazer as alterações necessárias para adaptar esse código ao código principal
// Implementar o reset após o fim da simulação. 
// conferir comentários e o que deve ser setado como constante
// Alterar os tipos das funções
// Remover os códigos de teste

/*              Bibliotecas que serão usadas no código              */
// Biblioteca responsável por fornecer o tipo 'uint8_t'.
#include <cstdint>
// Biblioteca responsável por fornecer o tipo 'vector'.
#include <vector>
// Bibliotecas responsáveis pela entrada e saída de dados.
#include <stdio.h>
#include <iostream>
// Biblioteca responsável por fornecer os métodos de manipulação de string que serão usados nesse código.
#include <string>
// Biblioteca responsável por fornecer os métodos necessários para lidar com números pseudo-aleatórios.
#include <random>
// Biblioteca responsável por fornecer os métodos necessários para controlar os leds.
#include <FastLED.h>
// Biblioteca responsável por fornecer o método para obter as horas a partir do esp32.
#include <chrono>
// Biblioteca padrão do Arduino
#include <Arduino.h>
// Biblioteca utilizada gerenciar a comunicação entre dispositicos através do protocolo I2C
#include <Wire.h>
// Biblioteca responsável por controlar o teclado matricial 4x4.
#include <Keypad.h>
// Biblioteca responsável por controlar o display LCD.
#include <LiquidCrystal_I2C.h>


/*              Structs que serão usadas no código              */
// Struct responsável por armazenar algumas constantes que serão necessárias no código.
struct Configs{
    // Constante responsável por setar a quantidade de nós que um objeto do tipo 'Graph' terá.
    static const int NODES_NUMBER; // Perceba que, essa constante também setará, por consequência, o número de leds.
    // Constante responsável por setar o pino que enviará os dados para a placa.
    static const int LED_PIN;
    // Constante responsável por setar o valor de uma das cores que os leds podem assumir.
    static const CRGB COLOR_1;
    // Constante responsável por setar o valor de uma das cores que os leds podem assumir.
    static const CRGB COLOR_2;
    // Constante responsável por setar a "cor apagada" aos leds.
    static const CRGB OFF;
    // Constante responsável por setar o número de linhas do teclado matricial.
    static const uint8_t ROWS;
    // Constante responsável por setar o número de colunas do teclado matricial.
    static const uint8_t COLS;
    // Constante responsável por setar a probabilidade de um led não mudar a sua cor em uma dada iteração da simulação do algoritmo de consenso.
    // OBS: Usaremos a escala de 0 a 100 para evitar de usar a comparação entre números do tipo 'float' ou 'double'. Ou seja,
    // SAME_COLOR_CHANCE = 55 representa uma probabilidade de 55% de um led não mudar a sua cor em uma dada iteração da simulação, por exemplo.
    static const int SAME_COLOR_CHANCE;
    // Constante responsável por setar a quantidade máxima de vértices que cada jogador poderá escolher.
    static const int NUMBER_OF_VERTICES_TO_CHOOSE;
};


/*              Constantes que serão utilizadas no código               */
// Inicializa os membros estáticos da struct Configs fora da definição da struct.
const int Configs::NODES_NUMBER = 100;
const int Configs::LED_PIN = 12;
const CRGB Configs::COLOR_1 = CRGB::Green;
const CRGB Configs::COLOR_2 = CRGB::Purple;
const CRGB Configs::OFF = CRGB::Black;
const int Configs::SAME_COLOR_CHANCE = 50;
const int Configs::NUMBER_OF_VERTICES_TO_CHOOSE = 3;
const uint8_t Configs::ROWS = 4;
const uint8_t Configs::COLS = 4;

// Setar a variável abaixo como constante ? 
// COMENTAR! (COLOCAR AS 2 VARIÁVEIS ABAIXO DENTRO DA STRUCT DE CONFIGS!)
uint8_t rowPins[Configs::ROWS] = {14, 27, 26, 25}; // GIOP14, GIOP27, GIOP26, GIOP25
uint8_t colPins[Configs::COLS] = {33, 32, 18, 19}; // GIOP33, GIOP32, GIOP18, GIOP19

// Setar a variável abaixo como constante ?
// Cria um mapa de teclas que define o que aparecerá ao se pressionar cada uma das teclas. No caso em questão, o mapa de teclas
// foi criado representando as teclas exatamente como elas aparecem no teclado.
char keys[Configs::ROWS][Configs::COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};


/*        Declaração da assinatura das funções auxiliares       */
void generateGraph();
void simulate();
uint8_t choice(char);
void setChoosenVerticesNumber();
void setColorVertices(); 
void updatePlayersVertexCount();
// Colocar a função hasColor e outras funções que existirem aqui
// Alterar a ordem das funções aqui e na definição delas no fim do código (por na ordem em que elas são executadas)

/*              Classes que serão usadas no código              */
class Node{
    /*
        Descrição:
            Essa classe é responsável por implementar uma abstração específica de um nó que representa o vértice de um grafo.
        
        Atributos:
            public:
                led_number (uint8_t): Número inteiro que representa o número do led do nó em questão.
                current_color (CRGB): Struct do tipo 'CRGB' que representa a cor atual que o nó possui.
                next_color (CRGB): Struct do tipo 'CRGB' que representa a próxima cor que o nó possuirá.
                neighbors (vector<uint8_t>): Array de inteiros que indica quais são os vizinhos do nó, isto é, que indica quais nós estão 
                                             conectados ao nó em questão. Para tal, esse array armazena os números dos vértices que representam
                                             cada um dos vizinhos do nó em questão.
            private:
                neighbors_count (uint8_t): Valor inteiro que representa a quantidade de vizinhos do nó.

        Métodos:
            public:
                void set_current_color(CRGB color_to_set): Seta a cor 'color_to_set' no atributo 'current_color' do nó.
                void set_next_color(CRGB color_to_set): Seta a cor 'color_to_set' no atributo 'next_color' do nó.
                void set_neighbor(uint8_t neighbor_number): Adiciona um novo vizinho ao nó.
                boolean is_neighbor(uint8_t neighbor_number): Verifica se um dado nó é vizinho do nó em questão.
                std::ostream& operator<<(std::ostream& os, const Node& node): Cria uma visualização para objetos do tipo 'Node'.

                OBS: Não serão adicionados métodos get para os atributos, a ideia é que eles sejam acessados diretamente.
                
            private:
                std::string get_neighbors_as_string(): Retorna uma string que mostra os vizinhos do nó seguindo o formato '[vizinho_1,...,vizinho_N]'.
                                                       Esse método auxilia na construção do método que sobrecarrega o operador '<<'.
    */

    public:
        int led_number;
        CRGB current_color;
        CRGB next_color;
        std::vector<int> neighbors;

        // Cria um construtor personalizado. Repare que, caso não seja passado nenhum parâmetro para o construtor da classe 'Node',
        // o construtor padrão criará um objeto do tipo 'Node' que possui o atributo 'number' == 255.
        // Repare que os atributos públicos led_number, current_color e next_color, além do atributo privado 'neighbors_count' são inicializados 
        // no array de inicialização do construtor da classe 'Node', enquanto o atributo público neighbors é inicializado dentro 
        // do construtor em questão.
        Node(int number = 255) : led_number(number), current_color(Configs::OFF), next_color(Configs::OFF),neighbors_count(0){}

        // Método responsável por setar o atributo 'led_number'
        void set_led_number(int number){
            this->led_number = number;
        }

        // Método responsável por setar uma nova cor no atributo 'current_color' de um objeto do tipo 'Node'.
        void set_current_color(CRGB color_to_set){
            this->current_color = color_to_set;
        }

        // Método responsável por setar uma nova cor no atributo 'next_color' de um objeto do tipo 'Node'.
        void set_next_color(CRGB color_to_set){
            this->next_color = color_to_set;
        }

        // Método responsável por setar um novo vizinho a um objeto do tipo 'Node'.
        void set_neighbor(int neighbor_number){
            this->neighbors.push_back(neighbor_number);
            this->neighbors_count ++;
        }

        // Método responsável por checar se um dado nó é vizinho do nó em questão.
        bool is_neighbor(int neighbor_number){
            for(int i = 0; i < this->neighbors.size(); i++){
                if(this->neighbors[i] == neighbor_number){
                    return true;
                }
            }
            return false;
        }

        // Método responsável por gerar uma visualização de objetos do tipo 'Node'.
        friend std::ostream& operator<<(std::ostream& os, const Node& node) {
            // Imprime o número do nó.
            os << "Led Number: " << static_cast<int>(node.led_number) << std::endl; 
            // Imprime a cor RGB do nó.
            os << "Node Color (R,G,B): " << static_cast<int>(node.current_color.r) << "," 
                                        << static_cast<int>(node.current_color.g) << "," 
                                        << static_cast<int>(node.current_color.b) << std::endl; 
            // Imprime os vizinhos do nó.
            os << "Node neighbors: " << node.get_neighbors_as_string() << std::endl;

            return os;
        }

        // Função para imprimir informações do nó via comunicação serial
        void serialPrintNode() {
            Serial.print("Led Number: ");
            Serial.println(led_number);
            
            Serial.print("Node Current Color (R,G,B): ");
            Serial.printf("R:%d, G:%d, B:%d\n", (current_color.r),(current_color.g),(current_color.b));
            Serial.print("Node Next Color (R,G,B): ");
            Serial.printf("R:%d, G:%d, B:%d\n", (next_color.r),(next_color.g),(next_color.b));
            
            // Suponha que get_neighbors_as_string() seja uma função membro de Node
            // que retorna uma string com os vizinhos
            Serial.print("Node neighbors: ");
            Serial.println(get_neighbors_as_string().c_str());
        }
    
    private:
        int neighbors_count;

        // Repare que a função abaixo é setada como 'const', isso quer dizer que essa função não modificará o estado do objeto em que ela é chamada.
        // Método responsável por retornar uma string que contém uma representação que exibe os atributos de um objeto do tipo 'Node'.
        std::string get_neighbors_as_string() const {
            std::string neighbors_representation = "[";
            for(int i = 0; i < this->neighbors.size(); i++){
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
            Essa classe é responsável por implementar a abstração de um grafo. Para tal, consideraremos um grafo como sendo um array
            fixo de nós, sendo que, a posição i do array citado representa o vértice de número i.
        Atributos:
            public:
                NODES_NUMBER (const uint8_t): Variável que guardará a quantidade de nós que o grafo terá, isto é, a quantidade de elementos
                do array que representa o grafo.
                nodes (Node[]): Array de objetos do tipo 'Node' que conterá todos os nós do grafo. 
                nodes_color (CRGB[]): Array de objetos do tipo 'CRGB' que conterá as cores que cada led assumirá no estado atual do grafo.
            private:
                current_nodes_number (uint8_t): Variável que guardará a quantidade atual de nós que o grafo possui, isto é, a quantidade de 
                elementos que o array que representa o grafo possui atualmente.
        Métodos:
            public:
                void set_node(Node node): Método responsável por adicionar um nó ao grafo, isto é, adicionar um objeto do tipo 'Node' ao array
                que representa o grafo.
                friend std::ostream& operator<<(std::ostream& os, const Graph& graph): Método responsável por gerar uma representação 
                para o grafo.

            private:
                N/A
    */          

    public:

        static const int NODES_NUMBER = Configs::NODES_NUMBER;
        Node nodes[NODES_NUMBER];
        CRGB nodes_color[NODES_NUMBER];

        // Cria um construtor para a classe 'Graph'.
        Graph() : current_nodes_number(0){}

        // Método responsável por adicionar um nó ao grafo e também por adicionar a cor atual desse nó ao array de objetos do tipo 'CRGB'
        // que guardará as cores que cada led assumirá no estado atual do grafo.
        void set_node(const Node& node){
            if(current_nodes_number < NODES_NUMBER){
                this->nodes[current_nodes_number] = node;
                nodes_color[current_nodes_number] = node.current_color;
                current_nodes_number++;
            }else{
                printf("Erro, o grafo já atingiu a sua quantidade máxima de nós.");
            }   
        }

        // Método responsável por adicionar uma visualização a objetos do tipo 'Graph'.
        // Perceba que o método abaixo utiliza a visualização gerada para objetos do tipo 'Node'.
        friend std::ostream& operator<<(std::ostream& os, const Graph& graph) {
            os << "\nGraph nodes: {\n" << std::endl;
            for (int i = 0; i < graph.current_nodes_number; i++) {
                os << "Vertex Number: " + std::to_string(i) << std::endl;
                os << graph.nodes[i] << std::endl;
            }
            os << "}" << std::endl;
            
            return os;
        }

        void printSerialGraph(){
            for(int i = 0; i < Configs::NODES_NUMBER; i++){
                Serial.printf("%d: \n", i);
                this->nodes[i].serialPrintNode();
                delay(100);
            }
        }

        void printSerialNodesColor(){
            std::string result = "\n[";
            for(int i = 0; i < Configs::NODES_NUMBER; i++){
                result += i + "=> " "(R:" + std::to_string(nodes_color[i].r) + ", G:" + std::to_string(this->nodes_color[i].g) + ", B:" + std::to_string(this->nodes_color[i].b) + "),";
            }
            result.pop_back(); //Remove a última vírgula.
            result += "]\n";

            Serial.print(result.c_str());
        }

    private:
        int current_nodes_number;
};


/*        Variáveis globais usadas no código        */

// Cria um objeto do tipo 'Keypad' para controlar o teclado matricial 4x4.
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, Configs::ROWS, Configs::COLS);

// Cria um objeto do tipo 'LiquidCrystal_I2C' para controlar o display LCD.
LiquidCrystal_I2C lcd(0x27,20,4); //Cria o objeto lcd passando como parâmetros o endereço, o nº de colunas e o nº de linhas

// Variável responsável por armazenar uma abstração do grafo representado na placa física.
Graph G;

// Variável global que armazena a posição atual da linha do cursor do display LCD.
uint8_t row_cursor_current_position = 0;
// Variável global que armazena a posição atual da coluna do cursor do display LCD.
uint8_t col_cursor_current_position = 0;

// Variável global que armazenará a quantidade de vértices que cada jogador irá escolher na simulação atual.
// PS: ESSA VARIÁVEL DEVERÁ SER LIMITADA POR 1 E POR UM VALOR DEFINIDO EM [Configs::]
int choices_number; //usar a variável NUMBER_OF_VERTICES_TO_CHOOSE
// Variável global que indicará se a simulação já foi iniciada ou não.

bool vertices_number_per_player_has_choosen = false; //TROCAR O NOME DESSA VARIÁVEL E CRIAR OUTRA VARIÁVEL DE CONTROLE

// Variável global que indicará se o usuário já escolheu o número de vértices que cada jogador irá poder escolher.
bool change_choosen_vertices_number = false;

// Variável global que indicará se a simulação já começou.
bool in_simulation = false;

// Variável global que indicará se a simulação precisa ser resetada.
bool reset = false;

// Variável global que guardará o número de vértices que o player 1 (COLOR_1) conquistou.
int player_1_current_vertex_count = 0;

// Variável global que guardará o número de vértices que o player 2 (COLOR_2) conquistou.
int player_2_current_vertex_count = 0;


/*        Funções auxiliares        */
void generateGraph(){
    
    // A estrutura de dados abaixo possui 'Configs::NODES_NUMBER' linhas pois cada linha i representa os vizinhos do
    // vértice 1, e, como a constante 'Configs::NODES_NUMBER' guarda o número de vértices, consequentemente, ela será
    // a responsável por setar o número de linhas na estrutura de dados abaixo. Além disso, a estrutura de dados abaixo
    // é setada como tendo 9 colunas pois esse é o tamanho da maior das linhas. 

    std::vector<std::vector<int>> ordered_pairs_connection;

    ordered_pairs_connection.push_back({4, 26}); //Vizinhos do vértice 0
    ordered_pairs_connection.push_back({2, 4}); //Vizinhos do vértice 1
    ordered_pairs_connection.push_back({1, 3}); //Vizinhos do vértice 2
    ordered_pairs_connection.push_back({2, 5, 6}); //Vizinhos do vértice 3
    ordered_pairs_connection.push_back({0, 1, 5, 22, 28}); //Vizinhos do vértice 4
    ordered_pairs_connection.push_back({3, 4, 6, 21, 23}); //Vizinhos do vértice 5
    ordered_pairs_connection.push_back({3, 5, 21}); //Vizinhos do vértice 6
    ordered_pairs_connection.push_back({8}); //Vizinhos do vértice 7
    ordered_pairs_connection.push_back({7, 9, 10, 11, 21}); //Vizinhos do vértice 8
    ordered_pairs_connection.push_back({8}); //Vizinhos do vértice 9
    ordered_pairs_connection.push_back({8, 13, 14}); //Vizinhos do vértice 10
    ordered_pairs_connection.push_back({8, 12, 16, 20, 22}); //Vizinhos do vértice 11
    ordered_pairs_connection.push_back({11, 14, 15, 16}); //Vizinhos do vértice 12
    ordered_pairs_connection.push_back({10, 99}); //Vizinhos do vértice 13
    ordered_pairs_connection.push_back({10, 12, 15, 99}); //Vizinhos do vértice 14
    ordered_pairs_connection.push_back({12, 14, 16, 17, 82}); //Vizinhos do vértice 15
    ordered_pairs_connection.push_back({11, 12, 15, 17, 18}); //Vizinhos do vértice 16
    ordered_pairs_connection.push_back({15, 16, 18, 81}); //Vizinhos do vértice 17
    ordered_pairs_connection.push_back({16, 17, 27}); //Vizinhos do vértice 18
    ordered_pairs_connection.push_back({20, 35}); //Vizinhos do vértice 19
    ordered_pairs_connection.push_back({11, 19, 21, 23}); //Vizinhos do vértice 20
    ordered_pairs_connection.push_back({5, 6, 8, 20, 22, 25}); //Vizinhos do vértice 21
    ordered_pairs_connection.push_back({4, 11, 21, 23, 24}); //Vizinhos do vértice 22
    ordered_pairs_connection.push_back({5, 20, 22, 29, 35}); //Vizinhos do vértice 23
    ordered_pairs_connection.push_back({22, 26}); //Vizinhos do vértice 24
    ordered_pairs_connection.push_back({21, 27}); //Vizinhos do vértice 25
    ordered_pairs_connection.push_back({0, 24}); //Vizinhos do vértice 26
    ordered_pairs_connection.push_back({18, 25}); //Vizinhos do vértice 27
    ordered_pairs_connection.push_back({4, 30}); //Vizinhos do vértice 28
    ordered_pairs_connection.push_back({23, 30, 31, 51}); //Vizinhos do vértice 29
    ordered_pairs_connection.push_back({28, 29, 52}); //Vizinhos do vértice 30
    ordered_pairs_connection.push_back({29, 32, 51}); //Vizinhos do vértice 31
    ordered_pairs_connection.push_back({31, 46}); //Vizinhos do vértice 32
    ordered_pairs_connection.push_back({35, 38, 52}); //Vizinhos do vértice 33
    ordered_pairs_connection.push_back({36, 50, 51}); //Vizinhos do vértice 34
    ordered_pairs_connection.push_back({19, 23, 33, 37, 47}); //Vizinhos do vértice 35
    ordered_pairs_connection.push_back({34, 40, 44, 50}); //Vizinhos do vértice 36
    ordered_pairs_connection.push_back({35, 38, 39}); //Vizinhos do vértice 37
    ordered_pairs_connection.push_back({33, 37, 39, 40}); //Vizinhos do vértice 38
    ordered_pairs_connection.push_back({37, 38, 42}); //Vizinhos do vértice 39
    ordered_pairs_connection.push_back({36, 38, 41, 42, 43}); //Vizinhos do vértice 40
    ordered_pairs_connection.push_back({40, 53, 73, 82}); //Vizinhos do vértice 41
    ordered_pairs_connection.push_back({39, 40, 44}); //Vizinhos do vértice 42
    ordered_pairs_connection.push_back({40, 44, 63}); //Vizinhos do vértice 43
    ordered_pairs_connection.push_back({36, 42, 43, 45, 46, 47}); //Vizinhos do vértice 44
    ordered_pairs_connection.push_back({44}); //Vizinhos do vértice 45
    ordered_pairs_connection.push_back({32, 44}); //Vizinhos do vértice 46
    ordered_pairs_connection.push_back({35, 44, 48}); //Vizinhos do vértice 47
    ordered_pairs_connection.push_back({47, 51}); //Vizinhos do vértice 48
    ordered_pairs_connection.push_back({50}); //Vizinhos do vértice 49
    ordered_pairs_connection.push_back({34, 36, 49}); //Vizinhos do vértice 50
    ordered_pairs_connection.push_back({29, 31, 34, 48}); //Vizinhos do vértice 51
    ordered_pairs_connection.push_back({30, 33}); //Vizinhos do vértice 52
    ordered_pairs_connection.push_back({41, 54, 56}); //Vizinhos do vértice 53
    ordered_pairs_connection.push_back({53, 55, 62, 63}); //Vizinhos do vértice 54
    ordered_pairs_connection.push_back({54, 58, 65}); //Vizinhos do vértice 55
    ordered_pairs_connection.push_back({53, 57, 58, 61}); //Vizinhos do vértice 56
    ordered_pairs_connection.push_back({56, 58, 59, 61, 66, 69}); //Vizinhos do vértice 57
    ordered_pairs_connection.push_back({55, 56, 57, 59, 60, 66}); //Vizinhos do vértice 58
    ordered_pairs_connection.push_back({57, 58, 69, 70}); //Vizinhos do vértice 59
    ordered_pairs_connection.push_back({58, 65, 66, 67, 87}); //Vizinhos do vértice 60
    ordered_pairs_connection.push_back({56, 57, 62, 67}); //Vizinhos do vértice 61
    ordered_pairs_connection.push_back({54, 61, 63, 64, 65, 72}); //Vizinhos do vértice 62
    ordered_pairs_connection.push_back({43, 54, 62, 64}); //Vizinhos do vértice 63
    ordered_pairs_connection.push_back({62, 63, 73, 81}); //Vizinhos do vértice 64
    ordered_pairs_connection.push_back({55, 60, 62, 67, 71, 72, 74}); //Vizinhos do vértice 65
    ordered_pairs_connection.push_back({57, 58, 60, 67, 70}); //Vizinhos do vértice 66
    ordered_pairs_connection.push_back({60, 61, 65, 66, 71, 74, 75}); //Vizinhos do vértice 67
    ordered_pairs_connection.push_back({69, 74}); //Vizinhos do vértice 68
    ordered_pairs_connection.push_back({57, 59, 68, 76, 77}); //Vizinhos do vértice 69
    ordered_pairs_connection.push_back({59, 66, 71, 75, 77}); //Vizinhos do vértice 70
    ordered_pairs_connection.push_back({65, 67, 70, 72, 78, 80, 86}); //Vizinhos do vértice 71
    ordered_pairs_connection.push_back({62, 65, 71, 73, 80}); //Vizinhos do vértice 72
    ordered_pairs_connection.push_back({41, 64, 72, 80, 81, 83}); //Vizinhos do vértice 73
    ordered_pairs_connection.push_back({65, 67, 68, 78, 80, 87}); //Vizinhos do vértice 74
    ordered_pairs_connection.push_back({67, 70, 76, 79}); //Vizinhos do vértice 75
    ordered_pairs_connection.push_back({69, 75, 77, 78, 87, 89}); //Vizinhos do vértice 76
    ordered_pairs_connection.push_back({69, 70, 76, 94}); //Vizinhos do vértice 77
    ordered_pairs_connection.push_back({71, 74, 76, 79, 84, 86, 90}); //Vizinhos do vértice 78
    ordered_pairs_connection.push_back({75, 78, 80, 85}); //Vizinhos do vértice 79
    ordered_pairs_connection.push_back({71, 72, 73, 74, 79, 84, 85}); //Vizinhos do vértice 80
    ordered_pairs_connection.push_back({17, 64, 73, 83}); //Vizinhos do vértice 81
    ordered_pairs_connection.push_back({15, 41, 83, 92}); //Vizinhos do vértice 82
    ordered_pairs_connection.push_back({73, 81, 82, 84, 92}); //Vizinhos do vértice 83
    ordered_pairs_connection.push_back({78, 80, 83, 91, 93}); //Vizinhos do vértice 84
    ordered_pairs_connection.push_back({79, 80, 90, 93, 96}); //Vizinhos do vértice 85
    ordered_pairs_connection.push_back({71, 78, 87, 88, 93}); //Vizinhos do vértice 86
    ordered_pairs_connection.push_back({60, 74, 76, 86, 88}); //Vizinhos do vértice 87
    ordered_pairs_connection.push_back({86, 87, 89, 90}); //Vizinhos do vértice 88
    ordered_pairs_connection.push_back({76, 88, 90}); //Vizinhos do vértice 89
    ordered_pairs_connection.push_back({78, 85, 88, 89, 91, 93, 94, 96}); //Vizinhos do vértice 90
    ordered_pairs_connection.push_back({84, 90, 92, 93}); //Vizinhos do vértice 91
    ordered_pairs_connection.push_back({82, 83, 91, 99}); //Vizinhos do vértice 92
    ordered_pairs_connection.push_back({84, 85, 86, 90, 91, 95, 97, 98, 99}); //Vizinhos do vértice 93
    ordered_pairs_connection.push_back({77, 90, 95}); //Vizinhos do vértice 94
    ordered_pairs_connection.push_back({93, 94, 96, 97}); //Vizinhos do vértice 95
    ordered_pairs_connection.push_back({85, 90, 95}); //Vizinhos do vértice 96
    ordered_pairs_connection.push_back({93, 95, 98}); //Vizinhos do vértice 97
    ordered_pairs_connection.push_back({93, 97, 99}); //Vizinhos do vértice 98
    ordered_pairs_connection.push_back({13, 14, 92, 93, 98}); //Vizinhos do vértice 99

    for (int i = 0; i < ordered_pairs_connection.size(); i++) {
        Node node;
        for (int j = 0; j < ordered_pairs_connection[i].size(); j++) {
            node.set_neighbor(ordered_pairs_connection[i][j]);
        }
        G.set_node(node);
    }

    // A matriz abaixo possui 100 linhas pois cada linha representa um par (led,vértice) e o grafo que iremos usar 
    // nesse código possui 100 vértices. Além disso, como cada linha representa um par (led, vértice), o número de
    // colunas será igual à 2.
    int led_vertex_relation[Configs::NODES_NUMBER][2] = {
        {1, 30}, {2, 28}, {3, 0}, {4, 1}, {5, 2}, {6, 4}, {7, 26}, {8, 27}, {9, 29}, {10, 31}, {11, 52}, 
        {12, 51}, {13, 33}, {14, 32}, {15, 24}, {16, 25}, {17, 5}, {18, 3}, {19, 6}, {20, 21}, {21, 22}, 
        {22, 23}, {23, 34}, {24, 50}, {25, 49}, {26, 48}, {27, 36}, {28, 35}, {29, 20}, {30, 8}, {31, 7}, 
        {32, 9}, {33, 11}, {34, 18}, {35, 19}, {36, 37}, {37, 38}, {38, 47}, {39, 46}, {40, 45}, {41, 44},
        {42, 40}, {43, 39}, {44, 17}, {45, 16}, {46, 12}, {47, 10}, {48, 13}, {49, 14}, {50, 15}, {51, 41}, 
        {52, 42}, {53, 43}, {54, 63}, {55, 81}, {56, 82}, {57, 99}, {58, 92}, {59, 83}, {60, 73}, {61, 64}, 
        {62, 54}, {63, 53}, {64, 55}, {65, 62}, {66, 72}, {67, 80}, {68, 84}, {69, 91}, {70, 98}, {71, 97}, 
        {72, 93}, {73, 85}, {74, 79}, {75, 71}, {76, 65}, {77, 61}, {78, 56}, {79, 60}, {80, 67}, {81, 74}, 
        {82, 78}, {83, 86}, {84, 95}, {85, 96}, {86, 90}, {87, 87}, {88, 75}, {89, 68}, {90, 66}, {91, 58}, 
        {92, 57}, {93, 59}, {94, 70}, {95, 76}, {96, 88}, {97, 94}, {98, 89}, {99, 77}, {100, 69}
    };

    // Itera sobre as sublistas da lista 'led_vertex_relation'.
    for(int i = 0; i < Configs::NODES_NUMBER; i++){
        // (IMPORTANTE!)
        // Repare que o LED começa a contar do 1 e vai até o 100.

        // Salvo em duas variáveis o par (número do led, número do vértice).
        int led_number = led_vertex_relation[i][0] - 1;
        int vertex_number = led_vertex_relation[i][1];

        // Atribuo ao vértice 'vertex_number' o número do seu respectivo led na placa, isto é 'led_number'.
        G.nodes[vertex_number].set_led_number(led_number);
    }
}

bool hasColor(CRGB& color){
    if(color.r == Configs::OFF.r && color.g == Configs::OFF.g && color.b == Configs::OFF.b){
        return false;
    }
    return true;
}

void simulate(){
    /*
        Description: 
        Essa função é responsável por executar o algoritmo de consenso no grafo G (declarado globalmente). Dito isso, vale relembrar 
        alguns pontos, primeiramente, é importante lembrar que cada posição 'i' de 'G.nodes' representa o vértice 'i' do grafo G. Por exemplo,
        o objeto do tipo 'Node' que está na posição 1, representa o vértice 1 do grafo G. Vale dizer também que, o número do led associado a um 
        dado 'G.nodes[i]' está guardado no atributo 'led_number' do objeto do tipo 'Node' em questão. Ou seja, para o objeto do tipo 'Node' 
        que ocupa a posição 1 de 'G.nodes', o led que representa esse vértice será o led 'G.nodes[1].led_color'.
    */    

    //
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();

    // Cria um gerador de números aleatórios usando uma semente gerada pelo sistema.
    std::mt19937 rng(seed);

    for(Node& node: G.nodes){
        //Serial.printf("led_number %d\n", node.led_number);
        //Serial.printf("=> %d\n", node.led_number);
        // Cria uma distribuição uniforme que gera números inteiros entre 0 e 100. Tal distribuição será usada para gerar um número 
        // pseudo-aleatórios entre 0 e 100
        std::uniform_int_distribution<int> dist(0, 100);

        // Gera um número pseudo-aleatório entre 0 e 100 usando a semente e a distribuição setadas logo acima.
        int random_int = dist(rng);
        //Serial.printf("random_int: %d\n\n", random_int);
        // Cria uma variável booleana que representa se a cor do nó em questão deverá ser alterada ou não, com base em SAME_COLOR_CHANCE.
        bool change_color = random_int >= Configs::SAME_COLOR_CHANCE; 

        if(change_color){
            // Se o fluxo de execução chegar até aqui, a cor do nó em questão deverá ser alterada.
            if(node.neighbors.empty()){
                //Serial.printf("neighbors: %d\n", node.led_number);
                // Se o fluxo de execução chegar até aqui, significa que o nó em questão não possui vizinhos. Logo, a sua cor não será alterada.
                // Faz com que a próxima cor do nó em questão seja igual a sua cor atual.
                node.set_next_color(node.current_color);
            }else{
                // Se o fluxo de execução chegar até aqui, significa que o nó em questão possui vizinhos. Logo, a sua cor será alterada. 
                int neighbors_number = node.neighbors.size();
                // Cria uma distribuição uniforme que gera números inteiros entre 0 e 'neighbors_number-1'. Tal distribuição será usada para 
                // gerar um número pseudo-aleatórios entre 0 e 'neighbors_number-1'.
                // PS: O fator de correção '-1' é usado pois estamos sorteando um índice de um array (0 <= array_index <= array.size() - 1).
                std::uniform_int_distribution<int> dist(0, neighbors_number-1);
                // Gera um número pseudo-aleatório entre 0 e 'neighbors_number-1'. Tal número indicará qual índice do array de vizinhos foi 
                // sorteado, isto é, qual dos vizinhos foi sorteado.
                int random_number = dist(rng);
                int choosen_neighbor_index = random_number;
                // Guarda em uma variável o número que representa o vértice do vizinho sorteado.
                int choosen_neighbor = node.neighbors[choosen_neighbor_index];
                // Faz com que a próxima cor do nó em questão seja igual a cor atual do seu vizinho sorteado.
                if(hasColor(G.nodes[choosen_neighbor].current_color)) node.set_next_color(G.nodes[choosen_neighbor].current_color);
                else node.set_next_color(node.current_color);
            }
        }else{
            //Serial.printf("change_color: %d\n", node.led_number);
            // Se o fluxo de execução chegar até aqui, o nó em questão permanecerá com a sua cor inalterada.
            // Faz com que a próxima cor do nó em questão seja igual a sua cor atual.
            node.set_next_color(node.current_color);
        }
    }
   
    for(int i = 0; i < Configs::NODES_NUMBER; i++){
        int led_number = G.nodes[i].led_number; //Ok!
        //std::cout << led_number << std::endl; //Ok!
        //Color current_color = G.nodes[i].current_color; //Ok!
        //std::cout << "R: " << current_color.R << " G: " << current_color.G << " B: " << current_color.B << std::endl; //Ok!
        CRGB next_color = G.nodes[i].next_color; //Ok!
        //std::cout << "R: " << next_color.R << " G: " << next_color.G << " B: " << next_color.B << std::endl; //Ok!

        G.nodes_color[led_number] = next_color;
        //std::cout << "R: " << G.nodes_color[led_number].R << " G: " << G.nodes_color[led_number].G << " B: " << G.nodes_color[led_number].B << std::endl;
        G.nodes[i].current_color = G.nodes[i].next_color;
        //std::cout << "R: " << G.nodes[i].current_color.R << " G: " << G.nodes[i].current_color.G << " B: " << G.nodes[i].current_color.B << std::endl;
    }

    updatePlayersVertexCount();
}


void setChoosenVerticesNumber(){
    // Limpo a mensagem inicial, isto é, limpo a mensagem 'Press # to start!'.
    lcd.clear();

    // Seta a posição atual do cursor do display LCD para a posição (coluna = 0, linha = 0).
    col_cursor_current_position = 0;
    row_cursor_current_position = 0;
    lcd.setCursor(col_cursor_current_position, row_cursor_current_position);

    // Exibe a mensagem 'Choices number:'.
    lcd.print("Number of starting");
    col_cursor_current_position = 0;
    row_cursor_current_position = 1;
    lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
    lcd.print("vertices for each");
    col_cursor_current_position = 0;
    row_cursor_current_position = 2;
    lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
    lcd.print("player: ");

    // Guarda na variável 'inserted_choices_number' uma eventual tecla digitada.
    char inserted_choices_number = keypad.getKey();
    // Enquanto uma tecla não for digitada, o código fica em loop.
    while(inserted_choices_number == NO_KEY || inserted_choices_number == 'A' || inserted_choices_number == 'B' 
        || inserted_choices_number == 'C' || inserted_choices_number == 'D' || inserted_choices_number == '0' 
        || inserted_choices_number == '#'|| inserted_choices_number == '*'){
        // Guarda na variável 'inserted_choices_number' uma eventual tecla digitada.
        // Perceba que, como apenas 1 caractere é guardado, não se pode digitar um número maior do que 9.
        inserted_choices_number = keypad.getKey();
    }

    // Seta a posição atual do cursor do display LCD para a posição (coluna = 0, linha = 1).
    col_cursor_current_position = 8;
    row_cursor_current_position = 2;
    lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
    // Exibo abaixo da mensagem 'Choices number:', o valor escolhido para o número de vértices que cada jogador irá selecionar, isto é,
    // o valor da variável inserted_choices_number.
    lcd.print(String(inserted_choices_number));

    // Atribuo o número de vértices que cada usuário irá escolher à variável 'choices_number'. Repare que é feita uma conversão do
    // tipo 'char' para o tipo 'int' através da subtração do '0'.
    choices_number = inserted_choices_number - '0';

    // CÓDIGO DE TESTE.
    Serial.println(choices_number);

    // Delay de 1 segundo para o usuário conferir o valor que ele digitou.
    delay(1000);

    // Limpa a tela do display LCD.
    lcd.clear(); 

    // Seta a posição atual do cursor do display LCD para a posição (coluna = 0, linha = 0).
    col_cursor_current_position = 0;
    row_cursor_current_position = 0;
    lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
    
    // Pede para o usuário digitar '#' ou '*'. Sendo que, digitar '#' significa que o jogo pode prosseguir para a escolha dos vértices pelos
    // jogadores, e, digitar '*' significa que o usuário escolheu o número errado de vértices por jogador e quer digitar novamente esse número.
    lcd.print(String("# - Confirm"));

    col_cursor_current_position = 0;
    row_cursor_current_position = 2;
    lcd.setCursor(col_cursor_current_position, row_cursor_current_position);

    // Pede para o usuário digitar '#' ou '*'. Sendo que, digitar '#' significa que o jogo pode prosseguir para a escolha dos vértices pelos
    // jogadores, e, digitar '*' significa que o usuário escolheu o número errado de vértices por jogador e quer digitar novamente esse número.
    lcd.print(String("* - Choose again"));

    // Guarda em uma variável uma eventual tecla digitada pelo usuário.
    char key_pressed = keypad.getKey();
    Serial.println(key_pressed);
    // Enquanto uma tecla não for digitada pelo usuário, o código fica em loop.
    while(key_pressed == NO_KEY || key_pressed == 'A' || key_pressed == 'B' || key_pressed == 'C' || key_pressed == 'D' || isDigit(key_pressed) ){
        // Verifica se uma tecla foi digitada pelo usuário.
        key_pressed = keypad.getKey();
    }

    if(key_pressed == '#'){
        // Caso o fluxo de execução chegue até aqui, a simulação poderá ser iniciada.

        // Digo a variável de controle 'vertices_number_per_player_has_choosen' que o usuário já escolhe o número de vértices que cada
        // jogador irá escolher.
        vertices_number_per_player_has_choosen = true;

        // Para controlar o fluxo do código, a variável 'change_choosen_vertices_number' é setada como false, caso ela seja true nesse ponto do
        // código. Isto é feito pois, caso tal variável seja true e a tecla '#' seja pressionada e guardada na variável 'key_pressed', isso 
        // indicará que o usuário inicialmente digitou o número errado de vértices que cada jogador poderá escolher e teclou '*' após digitar 
        // tal número. Ou seja, o fluxo foi reiniciado no caso em questão para que o usuário pudesse refazer a sua escolha, e, como consequência
        // disso, 'change_choosen_vertices_number' foi setada como true. Contudo, caso isso tenha ocorrido e o fluxo tenha chegado até aqui, 
        // significa que o usuário corrigiu a sua escolha. Logo, não há mais necessidade de reiniciar o fluxo de execução. Por conta disso, 
        // no caso citado, a variável 'change_choosen_vertices_number' é setada como false nesse ponto do código.
        if(change_choosen_vertices_number) change_choosen_vertices_number = false;
    }else{
        // Caso o fluxo de execução chegue aqui o usuário terá digitado um número errado para o número de escolhas de vértices por jogador, logo
        // o fluxo de execução deverá voltar ao ponto onde esse número é pedido para o usuário. Por conta disso, a variável 
        // 'change_choosen_vertices_number' é setada como true, indicando que a ação citada aqui deverá ocorrer.
        change_choosen_vertices_number = true;
    }
}

void setColorVertices(){
    byte chosen_vertices[2*choices_number];
    int number_of_chosen_vertices = 0;
    // Limpa a tela do display LCD.
    lcd.clear();

    // Cria um contador para controlar a quantidade de vértices que será escolhida pelos jogadores.
    byte count = 1;
    // Itera 2*'choices_number' vezes.
    while(count <= 2*choices_number){
        if(count % 2 != 0){
            // Quando count for um número ímpar, o jogador 1 escolherá um vértice.

            // Pede para o jogador 1 escolher um vértice e guarda essa escolha na variável 'player_1_choice'.
            bool was_choose = false;
            byte player_1_choice = choice('1');
            for(int i = 0; i < number_of_chosen_vertices; i++){
                if(chosen_vertices[i] == player_1_choice){
                    was_choose = true;
                }
            }
            if(was_choose){
                while(was_choose){
                    player_1_choice = choice('1');
                    was_choose = false;
                    for(int i = 0; i < number_of_chosen_vertices; i++){
                        if(chosen_vertices[i] == player_1_choice){
                            was_choose = true;
                        }
                    }
                }
            }

            chosen_vertices[number_of_chosen_vertices] = player_1_choice;
            number_of_chosen_vertices ++;
            // Altero a cor do 'player_1_choice'
            G.nodes[player_1_choice].set_current_color(Configs::COLOR_1); 
            // Seto no led referente ao vértice 'player_1_choice' a cor do vértice 'player_1_choice'
            G.nodes_color[G.nodes[player_1_choice].led_number] = Configs::COLOR_1; 
            // Exibo na placa a escolha feita pelo usuário
            FastLED.show();

            // CÓDIGO DE TESTE.
            Serial.println(player_1_choice);
            
            // Delay de 1s.
            delay(1000);
            // Incrementa o contador.
            count ++;
        }else{
            // Quando count for um número par, o jogador 2 escolheá um vértice.
            
            bool was_choose = false;
            // Pede para o jogador 2 escolher um vértice e guarda essa escolha na variável 'player_2_choice'.
            byte player_2_choice = choice('2');

            for(int i = 0; i < number_of_chosen_vertices; i++){
                if(chosen_vertices[i] == player_2_choice){
                    was_choose = true;
                }
            }
            if(was_choose){
                while(was_choose){
                    player_2_choice = choice('2');
                    was_choose = false;
                    for(int i = 0; i < number_of_chosen_vertices; i++){
                        if(chosen_vertices[i] == player_2_choice){
                            was_choose = true;
                        }
                    }
                }
            }

            chosen_vertices[number_of_chosen_vertices] = player_2_choice;
            number_of_chosen_vertices ++;
            // SETAR A COR DO VÉRTICE ESCOLHIDO AQUI! (E EXIBIR ESSA COR NO LED!) [A FAZER ]
            // Altero a cor do 'player_1_choice'
            G.nodes[player_2_choice].set_current_color(Configs::COLOR_2); 
            // Seto no led referente ao vértice 'player_1_choice' a cor do vértice 'player_1_choice'
            G.nodes_color[G.nodes[player_2_choice].led_number] = Configs::COLOR_2; 
            // Exibo na placa a escolha feita pelo usuário
            FastLED.show();

            // CÓDIGO DE TESTE.
            Serial.println(player_2_choice);

            // Delay de 1s.
            delay(1000);
            // Incrementa o contador.
            count ++;
        }
    } 

    // Seta a posição atual do cursor do display LCD para a posição (coluna = 0, linha = 0).
    col_cursor_current_position = 0;
    row_cursor_current_position = 0;
    lcd.setCursor(col_cursor_current_position, row_cursor_current_position);

    // Exibe a mensagem 'Press # to start'
    lcd.print("Press # to start");
    // Seta a posição atual do cursor do display LCD para a posição (coluna = 0, linha = 0).
    col_cursor_current_position = 0;
    row_cursor_current_position = 1;
    lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
    lcd.print("simulation");

    // Guarda na variável 'key_pressed' uma eventual tecla digitada pelo usuário.
    char key_pressed = keypad.getKey();
    // Enquanto a tecla '#' não for digitada pelo usuário, o código permanecerá em loop.
    while(key_pressed != '#'){
        // Verifica se uma tecla foi digitada pelo usuário.
        key_pressed = keypad.getKey();
    }
    delay(500);
    lcd.clear();

    updatePlayersVertexCount();

    // Seta a variável 'in_simulation' como true, indicando que a simulação irá começar a partir desse ponto do código.
    in_simulation = true;
}

byte choice(char player_number){
    // Seta a posição atual do cursor do display LCD para a posição (coluna = 0, linha = 0).
    row_cursor_current_position = 0;
    col_cursor_current_position = 0;
    lcd.setCursor(col_cursor_current_position,row_cursor_current_position);

    // Exibe uma mensagem informando qual player deverá realizar a escolha do vértice. 
    lcd.print("Player " + String(player_number) + " choose");

    row_cursor_current_position = 1;
    col_cursor_current_position = 0;
    lcd.setCursor(col_cursor_current_position,row_cursor_current_position);
    //
    lcd.print("a vertex: ");
    
    // Seta a posição atual do display como sendo a origem da segunda linha.
    col_cursor_current_position = 10;
    lcd.setCursor(col_cursor_current_position,row_cursor_current_position);

    // Cria uma variável que guardará a última tecla pressionada.
    char key_pressed = keypad.getKey();
    
    // Cria uma variável que guardará todas as teclas pressionadas até que se pressione a tecla '#'.
    String pressed_keys;

    // Loop para capturar as teclas pressionadas até que o caractere '#' seja pressionado.
    while (key_pressed != '#') {
        // Confere se alguma tecla foi pressionada.
        if (key_pressed != NO_KEY) {
            Serial.println(key_pressed);
            // Se a última tecla pressionada for igual à '*', um backspace será realizado.
            if ((key_pressed == '*') && (pressed_keys.length() > 0)) {
                // Remove o último caractere adicionado.
                pressed_keys.remove(pressed_keys.length() - 1);
                // Volta em uma coluna a posição atual do display.
                lcd.setCursor(col_cursor_current_position-1, row_cursor_current_position);
                // Substitui. no display, o caractere removido por um espaço em branco.
                lcd.print(" ");
                // Volta em uma coluna a posição atual do display. Isso é feito para que o espaço em branco seja substituido na próxima iteração
                // em que a tecla pressionada for diferente de '*'.
                col_cursor_current_position -= 1;

            // Se a última tecla pressionada for diferente de '*', a última tecla pressionada é adicionado a string "pressed_keys".
            }else {
                // pressed_keys.length() < 2 é usado pois só podem ser digitados números entre 0 e 99, já que temos 100 vértices no grafo 
                // em questão e esses vértices são numerados de 0 a 99.
                if(isDigit(key_pressed) && pressed_keys.length() < 2){
                    // Adiciona a última tecla pressionada a string "pressed_keys".
                    pressed_keys += key_pressed;
                    // Atualiza a posição atual do display com base na quantidade de caracteres em "pressed_keys".
                    lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
                    // Imprime a string 'pressed_keys' no display LCD.
                    lcd.print(key_pressed); 
                    // Avança em uma coluna a posição atual do display.
                    col_cursor_current_position += 1;
                }
            }    
        }
        // Atualiza a última tecla pressionada.
        key_pressed = keypad.getKey();      
    }

    // Limpa a tela do display LCD.
    lcd.clear();

    // Retorna o número do vértice escolhido pelo usuário.
    return pressed_keys.toInt();
}

// MUDAR O NOME DESSA FUNÇÃO
void updatePlayersVertexCount(){
    player_1_current_vertex_count = 0;
    player_2_current_vertex_count = 0;
    for(CRGB color : G.nodes_color){
        if(color == Configs::COLOR_1) player_1_current_vertex_count ++;
        else if(color == Configs::COLOR_2) player_2_current_vertex_count ++;
    }
}

// DECLARAR ESSA FUNÇÃO NA DECLARAÇÃO DE ASSINATURA DE FUNÇÕES
bool checkConsensus(){

}

/*        Setup da placa        */

void setup() {
    // Inicia a comunicação serial com uma taxa de transmissão de 9600 bits por segundo.
    Serial.begin(9600);
    //Configs iniciais do display
    lcd.init(); //Inicializa a comunicação com o display já conectado
    lcd.clear(); //Limpa a tela do display
    lcd.backlight(); //Aciona a luz de fundo do display
    lcd.print("Press # to start");

    // Gera o grafo
    generateGraph();

    // Com base na configuração abaixo, sempre que usarmos 'FastLED.show()' as cores que estiverem em 'G.nodes_color' serão as cores que irão ser exibidas
    // nos leds. Por conta disso, convencionaremos na hora de implementarmos a simulação que o array 'G.nodes' será o array que guardará o estado antigo dos
    // leds, enquanto o array 'G.nodes_color' será o array que guardará o estado novo dos leds. Dito isso, É IMPORTANTE RESSALTAR QUE 'G_nodes_color' e 'G.nodes'
    // são estruturas de dados diferentes, já que, 'G_nodes' é um array de objetos do tipo 'Node', enquanto 'G_nodes_color' é um array de objetos do tipo 'CRGB'.
    // Contudo, como todo 'G_nodes[i]' possui o atributo color que é do tipo 'CRGB', podemos operar com os dois arrays citados como sendo os vetores de estado.
    FastLED.addLeds<NEOPIXEL, Configs::LED_PIN>(G.nodes_color, Configs::NODES_NUMBER); // Inicializa a biblioteca FastLED
    
   
    // Exibe o estado inicial do grafo
    FastLED.show();
    delay(5000);
}

void loop() {
    
    // Cria uma variável que guardará uma eventual tecla pressionada nesse ponto da execução do código.
    char key_pressed = keypad.getKey();

    if(!in_simulation){
        if(((key_pressed != NO_KEY) && (key_pressed == '#')) || (change_choosen_vertices_number )){
            if((!vertices_number_per_player_has_choosen) || (change_choosen_vertices_number )){
                // Obtem do usuário o número de vértices que cada jogador poderá escolher.
                setChoosenVerticesNumber();
            }
            //
            if(!change_choosen_vertices_number){ 
                // Pede para cada jogador escolher 'choices_number' vértices e, após isso, colore esses vértices com a cor que representa cada
                // um dos jogadores.
                setColorVertices();
            }
        }else if((key_pressed != NO_KEY) && (key_pressed == '*')){
            // Exibe a mensagem 'Choices number:'.
            col_cursor_current_position = 0;
            row_cursor_current_position = 0;
            lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
            lcd.print("Enter the value ");
            col_cursor_current_position = 0;
            row_cursor_current_position = 1;
            lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
            lcd.print("of p: ");
        }
    }else{
        if(player_1_current_vertex_count == 100){
            delay(1000);
            col_cursor_current_position = 0;
            row_cursor_current_position = 0;
            lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
            lcd.print("The winner is");
            row_cursor_current_position = 1;
            col_cursor_current_position = 0;
            lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
            lcd.print("player 1!");
            delay(2000);
            lcd.clear();
            col_cursor_current_position = 0;
            row_cursor_current_position = 0;
            lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
            lcd.print("Press # to restart");
            char key = keypad.getKey();
            while(key != '#'){
                key = keypad.getKey();
                delay(50);
                Serial.println(key);
            }
            for(int i = 0; i < Configs::NODES_NUMBER; i++){
                G.nodes[i].current_color = Configs::OFF;
                G.nodes[i].next_color = Configs::OFF;
                G.nodes_color[i] = Configs::OFF;
            }
            updatePlayersVertexCount();
            FastLED.show();
            in_simulation = false;
            change_choosen_vertices_number = true;
        }else if(player_2_current_vertex_count == 100){
            delay(1000);
            col_cursor_current_position = 0;
            row_cursor_current_position = 0;
            lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
            lcd.print("The winner is");
            row_cursor_current_position = 1;
            col_cursor_current_position = 0;
            lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
            lcd.print("player 2!");
            delay(2000);
            lcd.clear();
            col_cursor_current_position = 0;
            row_cursor_current_position = 0;
            lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
            lcd.print("Press # to restart");
            char key = keypad.getKey();
            while(key != '#'){
                key = keypad.getKey();
            }
            for(int i = 0; i < Configs::NODES_NUMBER; i++){
                G.nodes[i].current_color = Configs::OFF;
                G.nodes[i].next_color = Configs::OFF;
                G.nodes_color[i] = Configs::OFF;
            }
            updatePlayersVertexCount();
            FastLED.show();
            in_simulation = false;
            change_choosen_vertices_number = true;
        }else{
            delay(200);
            simulate();
            Serial.println(player_1_current_vertex_count);
            Serial.println(player_2_current_vertex_count);
            FastLED.show();
        }
        
    }
}