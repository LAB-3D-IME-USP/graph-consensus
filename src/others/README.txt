Segue abaixo uma descrição de como cada arquivo deve ser interpretado. Vale dizer que, 
os arquivos que serão interpretados são os arquivos 'led_vertex_relation_converted' e 
'ordered_pairs_connection_converted.txt', já que, os demais arquivos foram usados apenas para gerar
os dois arquivos ditos como sendo os arquivos de interesse.

'led_vertex_relation_converted':
    O arquivo 'led_vertex_relation_converted.txt' é um arquivo que contém a relação que indica qual o número
    do led cada vértice do grafo recebera. Dito isso, o arquivo deve ser interpretado da seguinte forma:
    Suponha que na posição i da lista que é guardada pelo arquivo em questão, temos a sublista [a,b],
    essa lista indica que o led 'a' estará no vértice 'b' do grafo.

'ordered_pairs_connection_converted.txt':
    O arquivo 'ordered_pairs_connection_converted.txt' é um arquivo que contém a informação de quais são os
    os vizinhos de cada vértice. Dito isso, o arquivo deve ser interpretado da seguinte forma:
    Suponha que na posição i da lista que é guardada pelo arquivo em questão, temos a sublista [a,b],
    essa lista indica que o vértice i possui os vizinhos [a,b].