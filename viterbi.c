#include "viterbi.h"

table_t *tbl = NULL;
//considera o indice para os valores de 0 a 3
//cada valor possui duas posições consecutivas para 0 e 1.
// no vetor:
//		00 :v [0] v[1]
//    01 : v[2] v[3]
//    10 : v[4] v[5]
//    11 : v[6] v[7]
int *v = (int[8]){00,11,11,00,10,01,01,10}; //val emitido
int *v2 = (int[8]){00,10,00,10,01,11,01,11};	//val proximo


void genTableValues(){

	tbl = (table_t *)malloc(sizeof(table_t));
	
	tbl->valEmit = v;
	tbl->valProx = v2;

}

int encoderChoice(int i){
	if(!i){
		return 0; 	
	}else return 1;

}
void encoder(){

	int i,j,esc;
	int *output;
	int state = 0;
	
	output = (int *)malloc(sizeof(int)*((tam+2)*2));
	//Coloca dois zeros's na final da entrada
	for(i = tam;i < tam+2;i++)
		input[i] = 0;
		
	tam+=2;
	j = 0;
	for(i = 0; i < (tam*2);i+=2){
		if(!state){
			esc = encoderChoice(input[j]);
			if(esc == 0){
				output[i] = output[i+1] = 0;			
				state= 0;
			}
			else{
				output[i] = output[i+1] = 1;
				state = 2;		
			}
			j++;			
		}
		else if(state == 1){
			esc = encoderChoice(input[j]);		
			if(esc == 0){
				output[i] = output[i+1] = 1;			
				state = 0;
			}
			else{
				output[i] = output[i+1] = 0	;		
				state = 2;			
			}
			j++;
		}
		else if(state == 2){
			esc = encoderChoice(input[j]);
			if(esc == 0){
				output[i] = 1;
				output[i+1] = 0;			
				state = 1;
			}
			else{
				output[i] = 0;			
				output[i+1] = 1;
				state = 3;
			}
			j++;		
		}
		else{
			esc = encoderChoice(input[j]);
			if(esc == 0){
				output[i] = 0;
				output[i+1] = 1;			
				state = 1;
			}
			else{
				output[i] = 1;
				output[i+1] = 0;
				state = 3;			
			
			}
			j++;
		}	
	
	}
	for(i = 0; i < (tam*2);i+=2){
		printf("%d%d ", output[i],output[i+1]);	
	
	}
	
	
	return;

}
int main(){
	int i = 0;
	
	genTableValues();
	//lê tamanho da entrada
	scanf("%d", &tam);
	
	//le entrada
	input = (int *)malloc(sizeof(int)*(tam+2));
	while(i < tam){	
		scanf("%d", &input[i++]);
	}
	encoder();
	//for(i = 0; i < tam;i++)
	//	printf("%d ", input[i]);
	//for(i = 0; i < 8;i+=2){
	//	printf("%d %d \n", tbl->valEmit[i],tbl->valEmit[i+1]);
	//	printf("%d %d \n", tbl->valProx[i],tbl->valProx[i+1]);
	//}
	
	return 0;
}