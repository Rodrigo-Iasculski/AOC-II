# AOC-II
Trabalhos feitos na matéria de Arquitetura e Organização de Computadores II na UFPEL
# Cache Simulator
Trabalho feito em C com o intuito de simular o funcionamento de uma cache, para utilizar é preciso passar 7 argumentos por linha de comando.<br>
`nome <nsets> <bsize> <assoc> <substituição> <flag_saída> arquivo_de_entrada`
- nome - nome do arquivo de execução do simulador
- nsets - número de conjuntos da cache
- bsize - tamanho do bloco em bytes
- assoc - grau da associatividade (quantidade de vias)
- flag_saída - flag que ativa o modo padrão de saída de dados:
    - flag de saída em 0:<br>
      ![image](https://github.com/user-attachments/assets/bf8ce5d0-7b1e-4a71-af00-ac6a2ada17c8)
    - flag de saída em 1:<br>
      ![image](https://github.com/user-attachments/assets/fb88ce19-f1f4-400e-a451-74dab465a38c)
- arquivo_entrada - arquivo que contem os endereços da cache
