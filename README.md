	Universidade Federal da Fronteira Sul.
 
	TRABALHO II
   
	Disciplina: Inteligência Artificial;
	Professor: José Carlos Bins Filho;
    
	Aplicação de Viterbi para codificação/decodificação (com ruído);
 	

	Alunos : Edson Lemes da Silva && Lucas Cezar Parnoff.
	
**Descrição Geral**

O algoritmo recebe uma sequência de 0's e/ou 1's que representa a entrada,
que é passada para a função de codificação. Nela, é preenchido os valores
da codificação ( com tamanho do dobro + 2 em relação à entrada) de acordo 
com a tabela de próximos estados e emissões descritas no trabalho.

O próximo processo é a inclusão de ruídos nos valores codificados, a função
responsável por isso acrescenta de forma uniforme os valores alternados de 
acordo com o parâmetro da função de ruído, para simulação de eventuais ruídos
oriundos de transmissões.

O último passo é a decodificação, nela é construído a tabela de estados da
decodificação, esta tabela possui o tamanho da entrada original mais dois. Após
a construção desta tabela, é analisado o caminho inverso com menor erro respeitando
os estados indicados pelas transições.

Por último a comparação é feita bit-a-bit com a entrada original e o valor oriundo
da função de decodificação.

**1.Função de codificação**

A função recebe um vetor de inteiros (int input) prenchido com 0's ou 1's, estes valores representam
a entrada em binário. 
O primeiro passo da função é acrescentar dois 0's no final do vetor de entrada, para exercer
toda a influência dois bits de entrada. Obviamente o tamanho do vetor é acrescido em dois.

Posteriormente é alocado um vetor de saída que contém o dobro do tamanho de entrada. Deste modo,
para cada valor do vetor de entrada, é gerado dois valores para o vetor de saída de acordo com a
tabela de emissões e próximos estados. Para a implementação, o estado inicial é o 00.

No fim, a função devolve um vetor de inteiros (int output) com os valores codificados.

**2.Função de ruído**

A função recebe um vetor de inteiros (int output) que representa uma entrada codificada e 
um inteiro (int lvl) que indica o nível de avanço(passo) entre os bits. Este nível de avanço
possui um intervalo de 0 a 9, caso a função receba um valor acima ou abaixo destes
limites, a função é modificada para alterar o mínimo de 2bits da entrada codificada.

Os intervalos aqui definidos correspondem do maior para o menor, isto é, nível zero
representa um passo muito pequeno, e consequentemente uma maior modificação dos bits. E
em contra partida, um nível maior representa um passo muito longo, e por consequência
uma menor modificação dos bits. 

Por ser avanço de casas e não porcentagem, pode acontecer de alguns ruídos maiores 
mesmo sendo numero maior, por isso para minimizar esse problema 
foi multiplicado o valor aleatório gerado pelos passos, embora agora pode ser observado 
muitas igualdades entre passos de números consecutivos.


**3.Função de decodificação**

A função recebe um vetor de inteiros (int output) que representa uma entrada codificada com ruído.
A construção dos estados de decodificação é considerada como metade da entrada codificada.

Estrutura dos estados:

 struct decoder_t : Representa um estado da lista de decodificação. Ela contém 6 atributos, nomeados
da seguinte maneira:
  init : estado inicial (decimal);
  ent : entrada ( 0 ou 1);
  emit : valor emitido (em decimal);
  recv : valor recebido (decimal);
  last : estado de transição (decimal);
  erro : Diferença entre emit e recv (decimal).

   
struct table_t: Contém dois atributos, o primeiro deles é um vetor de struct decoder_t. O segundo é um
vetor (int atv[4]) de quatro posições, que representa flags para cada estado, ou seja, 00,01,10 e 11.

Desta maneira, são alocados metade do tamanho da entrada de codificação da struct table_t para variável tbb
(cada posição representa um estado da decodificação), para cada posição da variável tbb é alocado oito 
posições da struct decoder_t para a variável dec, pois como são quatro estados e duas possibilidades cada, 
então serão no máximo oito situações de transições.

A próxima etapa é a construção da tabela de decodificação, partindo do estado inicial (00), são definidas de forma
separada até o terceiro estado desta tabela, a primeira ( tbb[0]) que é definição do estado 0, e inicialização dos erros, a
segunda (tbb[1]) que é a definição dos estados 0 e 2(de acordo com a tabela de transição) com os erros já acumulados, e a
terceira (tbb[2]) parte que define os quatro estados com erros acumulados. Para saber qual estado está ativo em cada iteração,
usa-se as flags do vetor atv; é indicado pela posição que contém 1 qual estado está ativa para cada estado da decodificação.
Assim, até esta parte foi analisado três pares de bits.

A partir deste ponto, é verificado o restante da entrada (int output), assim para cada um deles é verificado o estado anterior 
para saber qual deles possui o menor erro ( de acordo com cada transição). Partindo dessa ideia, as demais posições da variável
tbb são preenchidas.

Como os estados e os error são tratados como inteiro, a função que verifica a diferença de bits é feita através de 
uma operação Xor bit-a-bit com alguns controles para retornar a diferença útil para a aplicação.

Após a construção da tabela de estados de decodificação, é chamado uma função que procura o caminho de menor erro(Seleciona um
se possui erros iguais).
Começando pela última posição da variável tbb, é procurado a posição do vetor dec que possui o menor erro, assim é
verificado no estado anterior indicado pelo estado dec->init, pegando novamente o menor erro ( desta vez locamente no
estado indicado por dec->init) e constrói o caminho guardando a posição de cada estado em um vetor auxiliar ( int path). Este
processo é repetido até que se chegue no estado de decodificação inicial, assim se obtém o caminho inverso de menor erro.

Posteriormente, utilizando o vetor que contém os índices do caminho inverso, é propagado na variável tbb obtendo através 
do índice no vetor path o valor emitido, deste modo armazena em uma outra estrutura, neste caso para o dec->ent. Recorrendo
toda a variável tbb, obtem-se o valor decodificado. Este é retornado como resultado da função.

**4.Resultado**

A partir da função de decodificação, é obtido o vetor resultante. Então é comparado bit-a-bit com a entrada original
para averiguar se o ruído de fato pode ser contornado pelo processo anterior. A rigor, só é possível obter a entrada
original novamente se o ruído for relativamente baixo, ou seja, se apenas alguns bits foram alterados.

Estão implementados duas funções para debug nesta aplicação, uma para codificação e outra para decodificação. A primeira
chamada de 'debugEncoder', ela imprime na tela a codificação da entrada, onde a cada dois bits estão separados por espaços.

A outra chama-se 'debugDecoder', esta função imprime a tabela de estado da decodificação, neste caso são imprimidos na 
tela todos os estados (Processo passo a passo da montagem da tabela). Na aplicação ficou definido que apenas o último
teste é impresso na tela, uma vez que é uma função apenas de 'debug' e visualização do processo da decodificação.

Os resultados são exibidos conforme a quantidade de ruído passado como parâmetro. Por exemplo, se tiver quatro ruídos
passados, então serão realizados quatro testes, um para cada ruído. Para cada teste, é imprimido na tela, a sequência
original da entrada, seguida pela sequência obtida na decodificação. Se não houve divergências de bits, então é exibido
a mensagem "Nenhum Erro. Sequencia Esta Integra", porém se houver diferenças, então é impresso as posições onde aconteceu
a divergência (começando com índice a partir do zero), e também é impresso a porcentagem de bits diferentes, ela
representa unicamente a diferença bit-a-bit, nada além disso.

**4.1.Exemplos de computações**

*1.
Parâmetros de entrada (arquivo data.in):
5
1 1 0 1 1
9

Entrada: 1 1 0 1 1

Codificação sem ruído : 11 01 01 00 01 01 11
Codificação com ruído : 01 01 01 00 01 01 11
Saída esperada do decoder: 1 1 0 1 1
Saída alcançada:           1 1 0 1 1 

*2.
Parâmetros de entrada (arquivo data.in):
11
0 0 0 1 1 1 0 0 1 1 0
9 0

Entrada: 0 0 0 1 1 1 0 0 1 1 0

Codificação sem ruído    : 00 00 00 11 01 10 01 11 11 01 01 11 00
Codificação com ruído(9) : 00 00 00 11 01 11 01 11 11 01 01 01 00
Saída esperada do decoder: 0 0 0 1 1 1 0 0 1 1 0
Saída alcançada          : 0 0 0 1 1 1 0 0 1 1 0

Codificação com ruído(0) : 01 11 01 10 11 10 01 01 10 00 10 10 10
Saída esperada do decoder: 0 0 0 1 1 1 0 0 1 1 0 
Saída alcançada          : 0 1 1 1 1 1 0 1 0 1 0 
Os bits 1,2,7 e 8 são diferentes entre saída esperada e alcançada ( a partir do índice zero). 36.36% de diferença.

*3.
Parâmetros de entrada (arquivo data.in):
30
1 0 0 1 0 1 0 0 1 1 1 0 0 0 1 1 0 0 1 0 1 1 0 1 1 0 0 0 1 1
4 3 2 1 0

Entrada : 1 0 0 1 0 1 0 0 1 1 1 0 0 0 1 1 0 0 1 0 1 1 0 1 1 0 0 0 1 1

Codificação sem ruído   : 11 10 11 11 10 00 10 11 11 01 10 01 11 00 11 01 01 11 11 10 00 01 01 00 01 01 11 00 11 01 01 11
Codificação com ruído(4): 01 10 10 11 10 00 10 10 11 01 10 01 10 00 11 11 01 10 11 10 00 01 01 00 01 01 11 01 11 01 01 11
Saída esperada do decoder: 1 0 0 1 0 1 0 0 1 1 1 0 0 0 1 1 0 0 1 0 1 1 0 1 1 0 0 0 1 1
Saída alcançada 	 : 1 0 0 1 0 1 0 0 1 1 1 0 0 0 1 1 0 0 1 0 1 1 0 1 1 0 0 0 1 1 

Codificação com ruído(3): 01 10 10 11 10 00 10 10 11 01 10 01 10 00 11 11 01 10 11 10 00 01 01 00 01 01 11 01 11 01 01 11
Saída esperada do decoder: 1 0 0 1 0 1 0 0 1 1 1 0 0 0 1 1 0 0 1 0 1 1 0 1 1 0 0 0 1 1
Saída alcançada 	 : 1 0 0 1 0 1 0 0 1 1 1 0 0 0 1 1 0 0 1 0 1 1 0 1 1 0 0 0 1 1

Codificação com ruído(2): 01 11 11 11 11 00 10 10 11 11 11 01 11 00 11 01 00 11 11 10 00 01 00 00 01 00 11 10 10 01 11 11
Saída esperada do decoder: 1 0 0 1 0 1 0 0 1 1 1 0 0 0 1 1 0 0 1 0 1 1 0 1 1 0 0 0 1 1
Saída alcançada		 : 0 1 1 0 0 0 1 0 0 1 1 0 0 0 1 1 0 0 1 0 1 1 1 1 0 1 1 1 1 0
Os bits 0 1 2 3 5 6 8 22 24 25 26 27 29 são diferentes entre saída esperada e alcançada , 43.33% de diferença.

Codificação com ruído(1): 01 11 11 11 11 00 10 10 11 11 11 01 11 00 11 01 00 11 11 10 00 01 00 00 01 00 11 10 10 01 11 11
Saída esperada do decoder: 1 0 0 1 0 1 0 0 1 1 1 0 0 0 1 1 0 0 1 0 1 1 0 1 1 0 0 0 1 1
Saída alcançada		 : 0 1 1 0 0 0 1 0 0 1 1 0 0 0 1 1 0 0 1 0 1 1 1 1 0 1 1 1 1 0
Os bits 0 1 2 3 5 6 8 22 24 25 26 27 29 são diferentes entre saída esperada e alcançada , 43.33% de diferença.

Codificação com ruído(0): 00 11 10 00 10 01 10 10 10 10 00 10 01 10 01 00 01 11 00 11 10 11 11 00 11 01 10 01 01 00 11 10
Saída esperada do decoder: 1 0 0 1 0 1 0 0 1 1 1 0 0 0 1 1 0 0 1 0 1 1 0 1 1 0 0 0 1 1
Saída alcançada		 : 0 1 0 1 0 1 0 1 0 0 0 1 1 1 0 1 1 0 1 1 1 0 0 0 1 1 1 0 0 0
Os bits 0 1 7 8 9 10 11 12 13 14 16 19 21 23 25 26 28 29 são diferentes entre saída esperada e alcançada , 60.00% de diferença.

**Compilação**

Para compilar é necessário digitar na linha de comando "make" no terminal.

Compilado e executado no LINUX.

Alternativamente, para remover o executável gerado basta digitar no terminal: make clean.

O arquivo executável gerado pela compilação chama-se "main".

**Execução**

A aplicação lê os dados de entrada do arquivo 'data.in' que precisa estar no mesmo diretório do executável. O programa
recebe 3 conjuntos de números inteiros como parâmetros. No arquivo de texto 'data.in', o primeiro número inteiro representa
o tamanho da entrada, os próximos inteiros são os valores que representam os bits, ou seja, 0 e/ou 1 ( A quantidade descrita
pelo primeiro parâmetro). E a última sequência de inteiros representam os ruídos que serão aplicados sobre a codificação. Este
não possui quantidade definida, pode ter vários elementos ou nem um. A função de leitura se encarrega de contar este último 
parâmetro. Entretanto, é definido uma quantidade máxima de ruídos, descrito pela constante MAXNOISE no arquivo 
"viterbi.h"; por default este valor ficou inicializado como 20.


Para facilitar a visualização do funcionamento da função de decodificação, a aplicação permite imprimir na tela a execução
passo a passo do processo de decodificação, para isso basta passar como parâmetro na linha de comando o valor '1', por 
default este valor esta definido como 0, ou seja, inativo. Contudo, já que esta função é apenas para visualização, é 
impresso na tela apenas o teste para o último ruído passado no arquivo de entrada.

Exemplo de execução:
	./main
	./main 1  
	
