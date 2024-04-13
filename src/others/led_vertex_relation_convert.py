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
            e, após ler o arquivo em questão, essa função transforma cada tupla em uma lista e guarda todas
            as listas geradas em uma outra lista de nome 'formated_data'.
            
        Params:
            data (io.TextIOWrapper): Arquivo que contém as tuplas no formato especificado.
        Return:
            formated_data (list): Lista de listas, onde cada lista interna representa uma tupla convertida.
    '''
    
    # Lista que conterá todas as listas geradas pela conversão das tuplas em listas.
    formated_data = []
    
    # Itero sobre cada tupla
    for pair in data.split('\n'):
        # Para cada par lido (repare que os pares são lidos como objetos do tipo 'str') é realizado uma conversão de 'str' para 'tuple' 
        # do par em questão.
        pair = eval(pair)
        # O par convertido em um objeto do tipo 'tuple' é agora convertido em um objeto do tipo 'list' e guardado na lista 'formated_data'.
        formated_data.append(list(pair))
    
    # Retorno a lista que contém todas as listas geradas pela conversão das tuplas em listas.
    return formated_data

def main():
    
    # Leio o arquivo 'led_vertex_relation.txt' no modo apenas leitura e salvo esse arquivo na variável data.
    with open('led_vertex_relation.txt','r') as initial_file:
        unformatted_data = initial_file.read()
    
    # Converto o arquivo representado pela variável data para o formato desejado.
    formated_data = convert(unformatted_data)
    # Transformo o arquivo convertido em uma string
    formated_data = str(formated_data)
    # Troco os colchetes do arquivo por chaves
    formated_data = formated_data.replace('[','{').replace(']','}')
    # Salvo os dados convertidos no arquivo 'led_vertex_relation_converted.txt'.
    with open('led_vertex_relation_converted.txt','w') as final_file:
        final_file.write(str(formated_data))
    
main()