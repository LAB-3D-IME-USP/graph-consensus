//TO - DO AND ALERTS!
// Trocar 'byte' por 'uint8_t'
// Fazer as alterações necessárias para adaptar esse código ao código principal
// Implementar o reset após o fim da simulação.
// conferir comentários e o que deve ser setado como constante

// Biblioteca responsável por controlar o teclado matricial 4x4.
#include <Keypad.h>
// Biblioteca responsável por controlar o display LCD 16x2.
#include <Adafruit_LiquidCrystal.h>

// [DECLARAÇÃO DE FUNÇÕES AUXILIARES]
byte choice(char);
void setChoosenVerticesNumber();
void setColorVertices(); // Essa função precisa ser completada no código principal

// [CONSTANTES!]
// Define o número de linhas do teclado matricial. [Configs::]
const byte keypad_num_rows= 4;
// Define o número de colunas do teclado matricial. [Configs::]
const byte keypad_num_cols= 4;
// Define os pinos em que estarão conectados as linhas do teclado matricial. [Configs::]
byte keypad_row_pins[keypad_num_rows] = {9,8,7,6}; // Linhas 0 a 3.
// Define os pinos em que estarão conectados as colunas do teclado matricial. [Configs::]
byte keypad_col_pins[keypad_num_cols]= {5,4,3,2}; // Colunas 0 a 3.

// [VARIÁVEIS GLOBAIS!]
// Variável global que armazena a posição atual da linha do cursor do display LCD.
byte row_cursor_current_position = 0;
// Variável global que armazena a posição atual da coluna do cursor do display LCD.
byte col_cursor_current_position = 0;
// Variável global que armazenará a quantidade de vértices que cada jogador irá escolher na simulação atual.
// PS: ESSA VARIÁVEL DEVERÁ SER LIMITADA POR 1 E POR UM VALOR DEFINIDO EM [Configs::]
int choices_number = 2;
// Variável global que indicará se a simulação já foi iniciada ou não.
bool vertices_number_per_player_has_choosen = false; //TROCAR O NOME DESSA VARIÁVEL E CRIAR OUTRA VARIÁVEL DE CONTROLE
// Variável global que indicará se o usuário já escolheu o número de vértices que cada jogador irá poder escolher.
bool change_choosen_vertices_number = false;
// Variável global que indicará se a simulação já começou.
bool in_simulation = false;
// Cria um mapa de teclas que define o que aparecerá ao se pressionar cada uma das teclas. No caso em questão, o mapa de teclas
// foi criado representando as teclas exatamente como elas aparecem no teclado.
char keymap[keypad_num_rows][keypad_num_cols]= 
{
    {'1', '2', '3', 'A'}, 
    {'4', '5', '6', 'B'}, 
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// [INICIALIZAÇÕES DE CLASSES!]
// Inicializa uma instância da classe 'Keypad', isto é, cria um objeto do tipo 'Keypad'. Tal ação é feita para que se possa usar os métodos da
// biblioteca 'Keypad' para controlar o teclado matricial 4x4.
Keypad myKeypad= Keypad(makeKeymap(keymap), keypad_row_pins, keypad_col_pins, keypad_num_rows, keypad_num_cols);
// Inicializa uma instância da classe 'Adafruit_LiquidCrystal', isto é, cria um objeto do tipo 'Adafruit_LiquidCrystal'. Tal ação é feita para
// que se possa usar os métodos da biblioteca 'Adafruit_LiquidCrystal' para controlar o display LCD 16x2. Vale dizer que, o argumento
// passado para o construtor da classe 'Adafruit_LiquidCrystal' é o número do endereço I2C do dispositivo. [Configs::]
Adafruit_LiquidCrystal lcd(0);

// Seta as configurações iniciais para que o código entre em loop.
void setup()
{
    // Inicia a comunicação serial com uma taxa de transmissão de 9600 bits por segundo.
    Serial.begin(9600);
    // Inicia o display LCD 16x2 passando como argumento para o método 'begin' as dimensões do display em questão.
    lcd.begin(16, 2);
    // Seta a posição atual do cursor do display LCD para a posição (coluna = 0, linha = 0).
    lcd.setCursor(col_cursor_current_position, row_cursor_current_position);
    // Exibe a mensagem inicial.
    lcd.print("Press #");
}

void loop()
{
    // Cria uma variável que guardará uma eventual tecla pressionada nesse ponto da execução do código.
    char key_pressed = myKeypad.getKey();

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
        }
    }else{
        // AQUI DEVERÁ SER RODADA A SIMULAÇÃO! [ TO-DO ]
        Serial.println("AQUI DEVERÁ SER RODADA A SIMULAÇÃO!");
        delay(2000);
    }
    //
    
}

void setChoosenVerticesNumber(){
    // Limpo a mensagem inicial, isto é, limpo a mensagem 'Press # to start!'.
    lcd.clear();

    // Seta a posição atual do cursor do display LCD para a posição (coluna = 0, linha = 0).
    col_cursor_current_position = 0;
    row_cursor_current_position = 0;
    lcd.setCursor(col_cursor_current_position, row_cursor_current_position);

    // Exibe a mensagem 'Choices number:'.
    lcd.print("Choices number:");

    // Guarda na variável 'inserted_choices_number' uma eventual tecla digitada.
    char inserted_choices_number = myKeypad.getKey();
    // Enquanto uma tecla não for digitada, o código fica em loop.
    while(inserted_choices_number == NO_KEY || inserted_choices_number == 'A' || inserted_choices_number == 'B' 
        || inserted_choices_number == 'C' || inserted_choices_number == 'D' || inserted_choices_number == '0' 
        || inserted_choices_number == '#'|| inserted_choices_number == '*'){
        // Guarda na variável 'inserted_choices_number' uma eventual tecla digitada.
        // Perceba que, como apenas 1 caractere é guardado, não se pode digitar um número maior do que 9.
        inserted_choices_number = myKeypad.getKey();
    }

    // Seta a posição atual do cursor do display LCD para a posição (coluna = 0, linha = 1).
    col_cursor_current_position = 0;
    row_cursor_current_position = 1;
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
    
    // Pede para o usuário digitar '#' ou 'D'. Sendo que, digitar '#' significa que o jogo pode prosseguir para a escolha dos vértices pelos
    // jogadores, e, digitar 'D' significa que o usuário escolheu o número errado de vértices por jogador e quer digitar novamente esse número.
    lcd.print(String("Press # or D"));

    // Guarda em uma variável uma eventual tecla digitada pelo usuário.
    char key_pressed = myKeypad.getKey();
    // Enquanto uma tecla não for digitada pelo usuário, o código fica em loop.
    while(key_pressed == NO_KEY || key_pressed == 'A' || key_pressed == 'B' || key_pressed == 'C' || key_pressed == '*' ||isDigit(key_pressed) ){
        // Verifica se uma tecla foi digitada pelo usuário.
        key_pressed = myKeypad.getKey();
    }

    if(key_pressed == '#'){
        // Caso o fluxo de execução chegue até aqui, a simulação poderá ser iniciada.

        // Digo a variável de controle 'vertices_number_per_player_has_choosen' que o usuário já escolhe o número de vértices que cada
        // jogador irá escolher.
        vertices_number_per_player_has_choosen = true;

        // Para controlar o fluxo do código, a variável 'change_choosen_vertices_number' é setada como false, caso ela seja true nesse ponto do
        // código. Isto é feito pois, caso tal variável seja true e a tecla '#' seja pressionada e guardada na variável 'key_pressed', isso 
        // indicará que o usuário inicialmente digitou o número errado de vértices que cada jogador poderá escolher e teclou 'D' após digitar 
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
    // Limpa a tela do display LCD.
    lcd.clear();

    // Cria um contador para controlar a quantidade de vértices que será escolhida pelos jogadores.
    byte count = 1;
    // Itera 2*'choices_number' vezes.
    while(count <= 2*choices_number){
        if(count % 2 != 0){
            // Quando count for um número ímpar, o jogador 1 escolherá um vértice.

            // Pede para o jogador 1 escolher um vértice e guarda essa escolha na variável 'player_1_choice'.
            byte player_1_choice = choice('1');
            
            // SETAR A COR DO VÉRTICE ESCOLHIDO AQUI! (E EXIBIR ESSA COR NO LED!) [ A FAZER ]

            // CÓDIGO DE TESTE.
            Serial.println(player_1_choice);
            
            // Delay de 1s.
            delay(1000);
            // Incrementa o contador.
            count ++;
        }else{
            // Quando count for um número par, o jogador 2 escolheá um vértice.
            
            // Pede para o jogador 2 escolher um vértice e guarda essa escolha na variável 'player_2_choice'.
            byte player_2_choice = choice('2');
            
            // SETAR A COR DO VÉRTICE ESCOLHIDO AQUI! (E EXIBIR ESSA COR NO LED!) [A FAZER ]

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

    // Guarda na variável 'key_pressed' uma eventual tecla digitada pelo usuário.
    char key_pressed = myKeypad.getKey();
    // Enquanto a tecla '#' não for digitada pelo usuário, o código permanecerá em loop.
    while(key_pressed != '#'){
        // Verifica se uma tecla foi digitada pelo usuário.
        key_pressed = myKeypad.getKey();
    }

    // Seta a variável 'in_simulation' como true, indicando que a simulação irá começar a partir desse ponto do código.
    in_simulation = true;
}

byte choice(char player_number){
    // Seta a posição atual do cursor do display LCD para a posição (coluna = 0, linha = 0).
    row_cursor_current_position = 0;
    col_cursor_current_position = 0;
    lcd.setCursor(col_cursor_current_position,row_cursor_current_position);

    // Exibe uma mensagem informando qual player deverá realizar a escolha do vértice. 
    lcd.print("Player " + String(player_number) + " choose:");
    
    // Seta a posição atual do display como sendo a origem da segunda linha.
    row_cursor_current_position = 1;
    lcd.setCursor(col_cursor_current_position,row_cursor_current_position);

    // Cria uma variável que guardará a última tecla pressionada.
    char key_pressed = myKeypad.getKey();
    // Cria uma variável que guardará todas as teclas pressionadas até que se pressione a tecla '#'.
    String pressed_keys;

    // Loop para capturar as teclas pressionadas até que o caractere '#' seja pressionado.
    while (key_pressed != '#') {
        // Confere se alguma tecla foi pressionada.
        if (key_pressed != NO_KEY) {
            // Se a última tecla pressionada for igual à 'D', um backspace será realizado.
            if ((key_pressed == 'D') && (pressed_keys.length() > 0)) {
                // Remove o último caractere adicionado.
                pressed_keys.remove(pressed_keys.length() - 1);
                // Volta em uma coluna a posição atual do display.
                lcd.setCursor(col_cursor_current_position-1, row_cursor_current_position);
                // Substitui. no display, o caractere removido por um espaço em branco.
                lcd.print(" ");
                // Volta em uma coluna a posição atual do display. Isso é feito para que o espaço em branco seja substituido na próxima iteração
                // em que a tecla pressionada for diferente de 'D'.
                col_cursor_current_position -= 1;

            // Se a última tecla pressionada for diferente de 'D', a última tecla pressionada é adicionado a string "pressed_keys".
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
        key_pressed = myKeypad.getKey();      
    }

    // Limpa a tela do display LCD.
    lcd.clear();

    // Retorna o número do vértice escolhido pelo usuário.
    return pressed_keys.toInt();
}