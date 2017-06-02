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
A função recebe um vetor de inteiros (int output) que representa uma entrada codificada.


**3.Função de decodificação**
A função recebe um vetor de inteiros (int output) que representa uma entrada codificada com ruído.
A construção dos estados de decodificação é considerada como metade da entrada codificada.

Estrutura dos estados:

struct decoder_t : Representa um estado da lista de decodificação. Ela contém 6 atributos, nomeados
da seguinte maneira:
  init : estado inicial (decimal)
  ent : entrada ( 0 ou 1)
  emit : valor emitido (em decimal)
  recv : valor recebido (decimal)
  last : estado de transição (decimal)
  erro : Diferença entre emit e recv (decimal)

   
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

Após a construção da tabela de estados de decodificação, é chamado uma função que procura o caminho de menor erro.
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
tela todos os estados.

Ambas as funções são chamadas dentro da função main, podendo ser desativadas caso necessário. Porque para entradas
muito grandes, pode ser que a tela fique bastante poluída.

**Compilação**

Para compilar é necessário digitar na linha de comando "make" no terminal.

Compilado e executado no LINUX.

O arquivo Makefile se encarrega de iniciar a execução do arquivo compilado.
Alternativamente, para remover o executável gerado basta digitar no terminal: make clean.


**Execução**


	
