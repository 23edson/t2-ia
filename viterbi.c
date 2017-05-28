#include "viterbi.h"

//table_t *tbl = NULL;
//considera o indice para os valores de 0 a 3
//cada valor possui duas posições consecutivas para 0 e 1.
// no vetor:
//		00 :v [0] v[1]
//    01 : v[2] v[3]
//    10 : v[4] v[5]
//    11 : v[6] v[7]
//int *v = (int[8]){00,11,11,00,10,01,01,10}; //val emitido
//int *v2 = (int[8]){00,10,00,10,01,11,01,11};	//val proximo


int v[8] = {0,3,3,0,2,1,1,2}; //val emitido
int v2[8] = {0,2,0,2,1,3,1,3};

tab_t *tbb = NULL;
int maxSize;
/*void genTableValues(){

	tbl = (table_t *)malloc(sizeof(table_t));
	
	tbl->valEmit = v;
	tbl->valProx = v2;

}*/

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
decoder_t  *fillData(int state, int val){
//	puts("dmmjmj");
	decoder_t *new = (decoder_t *)malloc(sizeof(decoder_t));
	//printf("%d %d\n", state,val);
	
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
	/*if(state == 0){
		new->init = 0;
		if(val == 0){
			
			new->ent = 0;
			new->emit = 0;
			new->last = 00;		
		}
		else{
			
			new->ent = 1;
			new->emit = 3;
			new->last = 2;		
		
		}	
	}
	else if(state == 1){
		new->init = 1;
		if(val == 0){
			new->ent = 0;
			new->emit = 3;
			new->last = 0;		
		}
		else{
			new->ent = 1;
			new->emit = 0;
			new->last = 2;
		
		}
	}
	else if(state == 2){
		new->init=2;	
		if(val == 0){
			new->ent = 0;
			new->emit = 2;
			new->last = 1;		
		}
		else{
			new->ent=1;
			new->emit = 1;
			new->last = 3;		
		
		}
	}
	else{
		new->init=3;
		if(val == 0){
			new->ent = 0;
			new->emit=1;
			new->last = 1;
		}	
		else{
			new->ent =1;
			new->emit = 2;		
			new->last = 3;
		}
	
	
	
	}	*/
	
	
	
	
	}


void fillMatrix(){
	int i,j;
	int st = 0;
	
	//printf("%d", tam/2);
	//puts("oigeerg");
	for(i = 0; i < (tam/2);i++){//puts("12");
		for(j = 0; j < 8;j++){
			
			if(j>0 && j%2==0)st+=1;
			//puts("popop");
			tbb[i].dec[j] = fillData(st,j%2);
			//printf("%d %d %d %d\n", tbb[i].dec[j]->init,tbb[i].dec[j]->ent,tbb[i].dec[j]->emit,tbb[i].dec[j]->last);
		//	puts("th");
		}
	}

}
void initStructs(){
	int i,j;
	for(i = 0;i < (tam/2);i++){
		tbb[i].dec = (decoder_t **)malloc(sizeof(decoder_t)*8);	
		tbb[i].qtd = 0;
		for(j = 0; j < 4;j++)
			tbb[i].atv[j] =0;
	}


}
void decoder(){

	int i;
	tam = 2;
	tbb = (tab_t *)malloc(sizeof(tab_t)*(tam/2));
	initStructs();
	fillMatrix();
	
	//for(i = 0; i < 8;i++){
	//	printf("%d %d %d %d \n",tbb[0].dec[i]->init,tbb[0].dec[i]->ent,tbb[0].dec[i]->emit,
	//	tbb[0].dec[i]->last);	
	
	//}/
	return;
}
int main(){
	int i = 0;
	//printf("fwfjjrh");
	//genTableValues();
	decoder();	
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