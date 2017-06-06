/**
*   Universidade Federal da Fronteira Sul
*
*   TRABALHO II
*   Disciplina: Inteligência Artificial
*   Professor: José Carlos Bins Filho
*
*
* 	 Alunos : Edson Lemes da Silva & Lucas Cezar Parnoff
*  
*   
*  O trabalho consiste em fazer codificação e decodificação de
*  uma sequência de bits dado como entrada. A codificação é feita
*  através da tabela de transições definida na descrição do trabalho.
*  Adicionando um ruído a partir da entrada codificada, com o uso do
*  algoritmo de Viterbi, é feito a decodificação levando em conta a tabela
*  de transições. O objetivo é recuperar a entrada original, mesmo que haja
*  divergência em alguns bits da codificação.
*
*
*
**/


#include "viterbi.h"


//transições
int v[8] = {0,3,3,0,2,1,1,2}; //Valores emitidos 
int v2[8] = {0,2,0,2,1,3,1,3}; //Próximo estado
int qtdNoise;

table_t *tbb = NULL;

int *output;
int test[MAXNOISE];
/**
 * @function encoder
 *
 *
 * A função é responsável por fazer a codificação dado uma entrada.
 * A codificação possui o dobro mais dois em relação ao tamanho da
 * sequência original. Deste modo, para cada bit é gerado dois de acordo
 * com as definições na tabela de transição; isto levando em conta também
 * que é adicionado dois pares de zeros's no final, para permitir que seja
 * usado a influência de todos os bits da entrada.	   	 
 * Este processo ocorre levando em conta que o estado inicial é o 00.
 *
 * O valor é colocado no variável global int *output
 **/ 
void encoder(){

	int i,j,esc;
	
	int state = 0;
	
	output = (int *)malloc(sizeof(int)*((tam+2)*2));
	
	//Coloca dois zeros's no final da entrada
	for(i = tam;i < tam+2;i++)
		input[i] = 0;
	
	tam+=2;
	j = 0;
	for(i = 0; i < (tam*2);i+=2){
		//se esta no estado 0
		if(!state){
			esc = input[j];
			if(esc == 0){
				//Emite 00; prox. estado : 0
				output[i] = output[i+1] = 0;			
				state= 0;
			}
			else{
				//Emite 11; prox. estado : 2
				output[i] = output[i+1] = 1;
				state = 2;		
			}
			j++;			
		}
		//se esta no estado 1
		else if(state == 1){
			esc = input[j];		
			if(esc == 0){
				//Emite 11; prox. estado : 0
				output[i] = output[i+1] = 1;			
				state = 0;
			}
			else{
				//Emite 00; prox. estado : 2
				output[i] = output[i+1] = 0;		
				state = 2;			
			}
			j++;
		}
		//se esta no estado 2
		else if(state == 2){
			esc = input[j];
			if(esc == 0){
				//Emite 10; prox. estado : 1
				output[i] = 1;
				output[i+1] = 0;			
				state = 1;
			}
			else{
				//Emite 01; prox. estado : 3
				output[i] = 0;			
				output[i+1] = 1;
				state = 3;
			}
			j++;		
		}
		//se esta no estado 3
		else{
			esc = input[j];
			if(esc == 0){
				//Emite 01; prox. estado : 1
				output[i] = 0;
				output[i+1] = 1;			
				state = 1;
			}
			else{
				//Emite 10; prox. estado : 3
				output[i] = 1;
				output[i+1] = 0;
				state = 3;			
			
			}
			j++;
		}	
	
	}
	tam = tam*2;
	
}

/**
 * @function fillData
 *
 * @param int state - Número do estado corrente
 * @param int val - Posicao no vetor v/v2
 *
 * A função preenche a struct decoder_t *new com os valores da tabela de
 * transição. Estes valores são usados posteriormente para calcular os erros
 * de cada estado da decodificação.
 * Após preechimento a struct é retornada.
 * 
 **/ 
decoder_t  *fillData(int state, int val){

	decoder_t *new = (decoder_t *)malloc(sizeof(decoder_t));
	
	int valFn;
	int valEmit;
	
	if(state == 0){
		valFn = v2[0+val];	
		valEmit = v[0+val];
	}
	else if(state == 1){
		valFn = v2[2+val];
		valEmit = v[2+val]; 	
	
	}
	else if(state == 2){
		valFn = v2[4+val];
		valEmit = v[4+val];
	}
	else{
		valFn = v2[6+val];
		valEmit = v[6+val];
	
	}
	new->init = state;
	new->ent = val;
	new->emit = valEmit;
	new->last = valFn;
	
	
	return new;
}

/**
 * @function fillMatrix
 *
 *  Preenche o vetor de structs com os valores da tabela
 *  de transição. O tamanho de estados de decodificação é
 *  metade do tamanho da entrada.
 * 
 **/ 

void fillMatrix(){
	int i,j;
	int st = 0;
	
	
	for(i = 0; i < (tam/2);i++){
		st = 0;
		for(j = 0; j < 8;j++){
			
			if(j>0 && j%2==0)st+=1;
			tbb[i].dec[j] = fillData(st,j%2);
		}

	}

}

/**
 * @function initStructs
 *
 * A função inicializa os parâmetros da struct tab_t tbb (global). A
 * quantidade de estados é a metade da entrada codificada.
 * 
 **/ 
void initStructs(){
	int i,j;
	for(i = 0;i < (tam/2);i++){
		tbb[i].dec = (decoder_t **)malloc(sizeof(decoder_t)*8);	
		for(j = 0; j < 4;j++)
			tbb[i].atv[j] =0;
	}


}

/**
 * @function getState
 *
 * @param int vet - Recebe o vetor codificado.
 * @param int pos - posição no vetor
 * @param int horario - Horário para o teste.
 *
 * Verifica dentro do vetor vet a pos e pos+1 para reconhecer
 * dois bits como entrada.
 * A função retorna o valor correspondente em decimal
 * 
 **/ 
int getState(int *vet,int pos){
	
	if(!vet[pos] && !vet[pos+1]){
		return 0;
	}
	else if(!vet[pos] && vet[pos+1]){
		return 1;
	}
	else if(vet[pos] && !vet[pos+1]){
		return 2;	
	}
	else{
		return 3;
	}

}

/**
 * @function getDiffError
 *
 * @param int a
 * @param int b
 *
 * A função compara os inteiro a e b, aplica xor bit-a-bit 
 * para verificar a diferença de bits entre a e b.
 * Retorna a quantidade de bits diferentes.
 * 
 **/ 
int getDiffError(int a, int b){

	int c;
	if(a==b)return 0;
	
	c = (a^b);
	if(c == 1)return c;
	else return (c-1);

}

/**
 * @function count
 *
 * @param tab_t tl - Tabela de decodificação do estado corrente
 * @param int state - número do estado a ser procurado
 *
 * Procura o menor erro em relação ao estado state passado como parâmetro.
 * A função devolve o índice desse valor.
 * 
 **/ 
int count(table_t *tl, int state){

	int i,idx;
	int menor = 999;
	for(i = 0;i<8;i++){
		if(tl->dec[i]->last == state && tl->dec[i]->erro < menor){
			menor = tl->dec[i]->erro;
			idx = i;		
		}
	
	}
	return idx;
}

/**
 * @function ativa
 *
 * @param int state- Estado a ser analisado
 * @param tab_t tl - Tabela de decodificação corrente
 *
 * A função procura qual estado corresponde ao anterior nos
 * estados da tabela de decodificação. 
 * É retornado os valores dos estados finais do estado anterior.
 * 
 **/ 
int *ativa(int state,table_t *tl){
	int *v = (int *)malloc(sizeof(int)*2);
	if(state == 0){
		v[0] = tl->dec[0]->last;
		v[1] = tl->dec[1]->last;	
	}
	else if(state == 1){
		v[0] = tl->dec[2]->last;
		v[1] = tl->dec[3]->last;	
	}
	else if(state == 2){
		v[0] = tl->dec[4]->last;
		v[1] = tl->dec[5]->last;	
	
	}
	else if(state == 3){
		v[0] = tl->dec[6]->last;
		v[1] = tl->dec[7]->last;	
	
	}
	return v;
}

/**
 * @function debugEncoder
 *
 * A função imprime na tela o resultado da codificação.
 * 
 **/ 
void debugEncoder(int *saida){

	int i;

	for(i = 0; i < tam;i+=2){
		printf("%d%d ", saida[i],saida[i+1]);	
	
	}


}

/**
 * @function debugDecoder
 *
 * A tabela imprime na tela todos os estados 
 * e as transições correspondente a cada um deles
 * conforme a decodificação é processada. 
 *
 * 
 **/ 
void debugDecoder(){


	int size = tam/2;
	int i,j,k;
	
	for(i = 0; i < size;i++){
		printf("Init Ent Emit Recv Last Error\n");
		for(j = 0; j < 4; j++){
			for(k = 0; k < 2;k++){
				if(tbb[i].atv[j] == 1){
					printf("  %d   %d    %d    %d    %d    %d\n",tbb[i].dec[2*j+k]->init,
							 tbb[i].dec[2*j+k]->ent,tbb[i].dec[2*j+k]->emit,
							 tbb[i].dec[2*j+k]->recv,tbb[i].dec[2*j+k]->last,
							 tbb[i].dec[2*j+k]->erro);			
				}
			
			}		
		}	
	}



}

/**
 * @function find
 *
 * @param tab_t tt - Vetor de estados da decodificação
 * @param int state - Estado a ser pesquisado
 *
 * A função procura o estado final correspondente ao início do
 * estado seguinte.
 * Deste modo, é retornado o índice desse estado.
 * 
 **/ 
int find(table_t *tt,int state){

	int i,k;
	
	
	for(i =0 ;i < 4; i++){
		if(tt->atv[i] == 1){
			for(k = 0; k < 2;k++){
				if(tt->dec[(2*i)+k]->last == state)
					return (2*i)+k;
			}		
		
		}
	}
	return -1;
}

/**
 * @function emitOriginal
 *
 * @param int path - Recebe um vetor contendo índices
 *
 * A função constrói um vetor com as emições originais
 * de acordo com os índices passados no vetor path.
 * No final, é retornado este vetor.s
 * 
 **/ 

int *emitOriginal(int *path){

	int size = tam/2;
	int i;
	int *original = (int *)malloc(sizeof(int)*(tam/2));
	for(i = 0; i < size;i++){
		original[i] = tbb[i].dec[path[i]]->ent;
			
	}
	return original;

}

/**
 * @function inversePath
 *
 * A função analisa o vetor de decodificação de trás para frente,
 * e reconstrói o caminho, colocando no vetor path os índices de
 * cada estado com o menor erro, respeitando as transições indicadas.
 * No final, é retornado um vetor contendo o caminho.
 * A função devolve o vetor de valores decodificados. 
 * 
 **/ 
int  *inversePath(){

	int size = tam/2;
	int i,val;
	int j = size-1;
	int atual = 0;
	
	int *path = (int *)malloc(sizeof(int)*size);
	for(i = 0 ;i < 4;i++){
		val = count(&tbb[size-1],i);	
		if(tbb[size-1].dec[val]->erro < tbb[size-1].dec[atual]->erro)
			atual = val;
	}
	
	size-=2;
	path[j--] = atual;
	for(;size >= 0; size--){
		val = count(&tbb[size],tbb[size+1].dec[path[j+1]]->init);
		path[j--] = val;
			
	}
	
	int *original = emitOriginal(path);
	return original;

}

/**
 * @function decoder
 *
 * @param int output - recebe o vetor codificado (com ruído	)
 *
 * A função é responsável por gerenciar a decodificação da entrada.
 * Inicia assumindo que está no estado 00. Constrói a tabela para 
 * os primeiros 3 pares de bits. A partir daí controla a construção
 * considerando que há mais de um estado final, neste caso usa-se o 
 * estado que possui o menor erro entre o conjunto corrente.
 *
 * Aloca um vetor de struct table_t da metade do tamanho da entrada.
 * Assim cada posição é um estado da decodificação, ou seja, cada
 * posição contém um vetor de decoder_t com oito posições.
 * O índice 0 de table_t tbb usa-se apenas 2 índices de decoder_t,
 * índice 1 de table_t usa 4 índices de decoder_t,
 * os demais usam os oito índices. 
 * A função retorna um vetor contendo os valores decodificados.
 **/ 
int *decoder(int *output){

	int j,k;
	
	
	int current = 0;
	tbb = (table_t *)malloc(sizeof(table_t)*(tam/2));
	//Inicializa e preenche as estruturas
	initStructs();
	fillMatrix();
	
	//Inicializa a decodificacao assumindo estado 00
	for(j = 0; j < 2;j++){
		tbb[0].dec[j]->recv = getState(output,0);
		tbb[0].dec[j]->erro = getDiffError(tbb[0].dec[j]->emit,tbb[0].dec[j]->recv); 
	}	
	//marca estado 00 como ativo
	tbb[0].atv[0]=1;
	
	//current++;
	//marca estado seguinte
	for(j = 0; j < 2; j ++){
		tbb[1].dec[j]->recv = getState(output,2);	
		tbb[1].dec[j]->erro = tbb[0].dec[0]->erro + getDiffError(tbb[0].dec[j]->emit,tbb[1].dec[j]->recv);
	}
	for(j =4 ; j < 6;j++){
		tbb[1].dec[j]->recv = getState(output,2);	
		tbb[1].dec[j]->erro = tbb[0].dec[1]->erro + getDiffError(tbb[1].dec[j]->emit,tbb[1].dec[j]->recv);
	}	
	//Marca os estados 00 e 10 como ativos
	tbb[1].atv[0] = 1;
	tbb[1].atv[2] = 1;
	
	//Completa os estados iniciais
	for(j = 0; j < 8;j++){
		
		tbb[2].dec[j]->recv = getState(output,4);
		int d = find(&tbb[1],tbb[2].dec[j]->init);	
		tbb[2].dec[j]->erro = tbb[1].dec[d]->erro + getDiffError(tbb[2].dec[j]->emit,tbb[2].dec[j]->recv);
	}
	//Marca os quatro estados como ativos
	for(k = 0; k < 4;k++)tbb[2].atv[k]=1;
	current+=3;
	
	//Faz as outras transições até completar os estados de decodificação
	for(j = 6; j < tam;j+=2){
		for(k =0; k < 4;k++){
			if(tbb[current-1].atv[k] == 1){
				tbb[current].atv[k] = tbb[current-1].atv[k];
				int minor = count(&tbb[current-1],k);			
						
				tbb[current].dec[2*k]->recv = tbb[current].dec[(2*k)+1]->recv = getState(output,j);
				tbb[current].dec[2*k]->erro = tbb[current-1].dec[minor]->erro+ getDiffError(tbb[current].dec[2*k]->emit,tbb[current].dec[2*k]->recv);
				tbb[current].dec[(2*k)+1]->erro = tbb[current-1].dec[minor]->erro+ getDiffError(tbb[current].dec[(2*k)+1]->emit,tbb[current].dec[(2*k)+1]->recv);
			}			
			
		}
	   current++;
	}
	
	//Obtem o menor caminho inverso
	int *original = inversePath();
	
	return original;
}

/**
 * @function ruido
 * 
 * @param int alvo - recebe o vetor que será aplicado o ruido.
 * @param int lvl  - indica o passo do ruido que será aplicado em cima do alvo.
 *  
 * Função que aplica um ruido uniforme para o alvo simulando os possíveis 
 * erros de transmições, que podem ocorrer quando é passado pela rede.
 * sendo '0' o mais alterado e 'tamanho * 2' o maximo, valores diferentes, seram
 * considerados como 'tamanho * 2 - 3' para no minimo alterar dois valores.
 * 
 **/ 

void ruido(int *alvo,int lvl){
    int i, n;
    time_t t;
    
    n = (3 + lvl * 2) / 2;
   // printf("l:%d %d |", lvl,n);
    srand((unsigned) time(&t));
    if(n < 1 || n > tam)
        n = tam - 3;
    if(n%2 == 0)
        n++;
    for(i = 0 ; i < tam ; i += n ){
        alvo[i] = (alvo[i] + rand() % 100 * n)%2;
    };
    return;
}

/**
 * @function readFile
 * 
 * A função faz a leitura do arquivo 'data.in', onde
 * encontram-se  os parametros de entrada para o algoritmo.
 * São três parametros: tamanho da entrada, a entrada 
 * e alguns inteiros que representam o ruido
 **/
void readFile(){

	int i=0;
	FILE *arq;
	arq = fopen("data.in","r");
	
	fscanf(arq,"%d", &tam);
	input = (int *)malloc(sizeof(int)*(tam+2));
	
	while(i < tam){	
		fscanf(arq,"%d ", &input[i++]);
	}
	i = 0;
	while(!feof(arq)){
		fscanf(arq,"%d", &test[i++]);
		qtdNoise++;
	}

}

/**
 * @function printData
 * 
 * @param int data - Vetor a ser imprimido
 *
 * A função imprime o vetor na tela
 *
 **/
void printData(int *data){

	int i;
	for(i = 0; i < tam/2 - 2;i++)
		printf("%d ",data[i]);
	printf("\n");	

}

/**
 * @function copyData
 * 
 * @param int dest
 * @param int source
 *
 * A função copia os dados de source para dest
 *
 **/
void copyData(int *dest, int *source){
	int i;
	for(i = 0; i < tam;i++){
		dest[i] = source[i];	
	
	}

}

/**
 * @function ruido
 * 
 * @param int saida - Recebe um vetor de inteiros
 *  
 * Verifica os valores com a entrada original, analisando
 * se as posições são iguais. No final imprime o feedback,
 * a porcentagem de erro representa apenas a diferença 
 * de valores nos dois vetores.
 **/
void compara( int *saida){
	
	int i,j=0;
	int limit = tam/2-2;
	int *v;
	v = (int *)malloc(sizeof(int)*tam/2-2);
	for(i = 0; i < limit;i++){
		if(input[i]!=saida[i]){
			v[j++] = i;		
		
		}
	
	}
	if(j == 0){
		printf("Nenhum Erro. A Sequencia Esta Integra\n");	
	
	}
	else{
		printf("Bits diferentes: ");
		for(i = 0; i < j;i++){
			printf("%d ", v[i]);
		}
		printf(", Erro de: %.2f%% \n", (j*100.0)/(tam/2-2));	
	
	}

}
//Função principal
int main(int argc, char* argv[]){

	int i = 0,lvl,j;
   int *cpy;
   int ativo;
	readFile();
	//if(argc > 1){
		//ativo = argv[1][0];	
	
	//}else/ ativo = 0;	
        
    // printf("t : %d \n", lvl);
	//lê tamanho da entrada
	//scanf("%d", &tam);
	
	//le entrada

	//while(i < tam){	
	//	scanf("%d", &input[i++]);
	//}
	//scanf("%d", &lvl);
	if(argc > 1){ativo = argv[1][0] - '0';}
	else ativo = 0;
	encoder();
	
    //    printf("\n");
	//debugEncoder();
	
     //   if(lvl < 0 || lvl > tam)
       //     lvl = tam - 3; 
  // printf("aq\n");
//  printf("\n");
	//ruido(output,lvl);
      //  for(i=0;i < tam;i++) //verificar output
       //     printf(" %d",output[i]);
        //printf("\n%d\n",tam); // tamanho esta correto
     //   printf("\n");
   //debugEncoder();
  // printf("\n");
	//modifica a codificacao manualmente para testar
	//int vt[64] = {1,1,1,0,1,1,1,1,1,0,0,0,1,0,1,1,1,1,0,1,1,0,0,1,1,1,0,0,1,1,0,1,0,1,1,1,1,0,1,0,0,0,0,1,0,1,0,0,0,1,0,1,1,1,0,0,1,0,0,1,0,1,1,1};
	//for(
	//int *original = decoder(output);//OK!
	//printf("compara");
	//	for(i=0;i < tam/2 - 2;i++) //verificar output
  //          printf("%d ",original[i]);
            
       // printf("\n%d\n",tam /2 - 2); // tamanho esta correto
//	printf("\n");
	//decoder(vt);
//for(i=0;i < tam/2-2;i++) //verificar output
  //          printf("%d ",input[i]);
     //   printf("\n%d\n",tam /2 - 2); // tamanho esta correto
	//for(i = 0; i < tam;i++)
	//	printf("%d ", input[i]);
	//for(i = 0; i < 8;i+=2){
	//	printf("%d %d \n", tbl->valEmit[i],tbl->valEmit[i+1]);
	//	printf("%d %d \n", tbl->valProx[i],tbl->valProx[i+1]);
	//}
	printf("Codificacao sem ruido\n");
	debugEncoder(output);
	//printf("%d", qtdNoise);
	printf("\n\nDecodificao\n");
	for(i = 0; i < qtdNoise;i++){
		printf("Teste %d , ruido : %d \n",i+1,test[i]);
		
		
		//memcpy(cpy,output,tam);
		copyData(cpy,output);
		//
		//printData(cpy);
		lvl = test[i];
		if(lvl < 0 || lvl > tam)lvl = tam - 3; 
		ruido(cpy,lvl);
		printf("\nCodificacao com ruido\n");
		debugEncoder(cpy);
		printf("\nOriginal: ");
		printData(input);
		//for(j = 0; j < tam;j+=2)printf("%d%d ", cpy[j],cpy[j+1]);
		
		//printf("\n");
		int *original = decoder(cpy);
		//free(cpy);
		printf("Saida:    ");
		printData(original);
		
		compara(original);		
		//debugEncoder();		
		printf("\n");
		
	}
	if(ativo == 1){
		//imprime apenas o ultimo 		
		debugDecoder();	
	
	}   
   free(input);
	free(output);
	return 0;
	
	
}
