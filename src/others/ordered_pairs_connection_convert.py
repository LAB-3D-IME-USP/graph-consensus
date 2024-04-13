def convert(data):
    '''
        Description:
            Essa função é responsável por receber um arquivo que contém tuplas seguindo o formato:
            '
                (x_{1}, y_{1})
                (x_{2}, y_{2})
                  .      .
                  .      .
                  .      .
                (x_{n}, y_{n})
            '
            e, após ler o arquivo em questão, essa função identifica todas as tuplas (i, n_{1}), (i, n_{2}),
            ..., (i, n_{m}), e agrupa os elementos n_{m} dessas tuplas em uma lista seguindo o formato
            [n_{1}, n_{2}, ..., n_{m}] e, por fim, guarda essa lista no índice i de uma outra lista de nome
            'formated_data' que, por sua vez, conterá todas as listas de tuplas agrupadas.
        Params:
            data (io.TextIOWrapper): Arquivo que contém as tuplas no formato especificado.
        Return:
            formated_data (list): Lista de listas, onde cada lista interna representa uma tupla convertida.
    '''
    
    # Lista auxiliar que conterá todas as listas geradas pela conversão das tuplas em listas.
    aux_list = []
    
    # Itero sobre cada tupla.
    for pair in data.split('\n'):
        # Para cada par lido (repare que os pares são lidos como objetos do tipo 'str') é realizado uma conversão de 'str' para 'tuple' 
        # do par em questão, e, logo após, o objeto do tipo 'tuple' é convertido em um objeto do tipo 'list'.
        pair = list(eval(pair))

        # O par convertido é guardado na lista 'formated_data'.
        aux_list.append(pair)
    
    # Lista que conterá todas as listas geradas pela conversão das tuplas em listas.
    formated_data = []

    # Itero sobre a lista que foi gerada através da conversão das tuplas.
    i, j = 0, 0
    while(i < len(aux_list)):
        # Lista que conterá o 'grupo' dos vizinhos do vértice i
        groups = []
        # Guardo o número do vértice atual
        first_number = aux_list[j][0]
        # Uso a variável j para iterar sobre todos os vizinhos do vértice atual
        while((j < len(aux_list)) and (aux_list[j][0] == first_number)):
            # Adiciono cada vizinho do vértice atual na lista 'groups'
            groups.append(aux_list[j][1])
            # Incremento j
            j += 1
        # Adiciono o grupo do vértice em questão na posição de mesmo número desse vértice da lista formated_data.
        formated_data.append(groups)
        # Incremento i em um valor igual à j
        i = j
        
    # Retorno a lista que contém todas as listas geradas pela conversão das tuplas em listas.
    return formated_data

def main():
    
    # Leio o arquivo 'ordered_pairs_connection.txt' no modo apenas leitura e salvo esse arquivo na variável data.
    with open('ordered_pairs_connection.txt','r') as initial_file:
        unformatted_data = initial_file.read()

    # Converto o arquivo representado pela variável data para o formato desejado.
    formated_data = convert(unformatted_data)
    # Transformo o arquivo convertido em uma string
    formated_data = str(formated_data)
    # Troco os colchetes do arquivo por chaves
    formated_data = formated_data.replace('[','{').replace(']','}')
    # Salvo os dados convertidos no arquivo 'ordered_pairs_connection_converted.txt'.
    with open('ordered_pairs_connection_converted.txt','w') as final_file:
        final_file.write(formated_data)
    
main()