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
int *encoder(){

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
	tam = tam*2;
	
	return output;

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
	
	//printf("T:%d\n", tam/2);
	//puts("oigeerg");
	for(i = 0; i < (tam/2);i++){//puts("12");
		st = 0;
		for(j = 0; j < 8;j++){
			
			if(j>0 && j%2==0)st+=1;
			//puts("popop");
			tbb[i].dec[j] = fillData(st,j%2);
		//	printf("%d %d %d  %d\n", tbb[i].dec[j]->init,tbb[i].dec[j]->ent,tbb[i].dec[j]->emit,tbb[i].dec[j]->last);
		//	puts("th");
		}
	//	printf("\n");
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
int getState(int *vet,int pos){
	//printf("%d %d ", pos,vet[pos+1]);
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
int getDiffError(int a, int b){

	int c;
	if(a==b)return 0;
	
	c = (a^b);
	if(c == 1)return c;
	else return (c-1);

}
int count(tab_t *tl, int state){

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
int *ativa(int state,tab_t *tl){
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
int find(tab_t *tt,int state){

	int i,k;
	
	
	for(i =0 ;i < 4; i++){
		if(tt->atv[i] == 1){
			for(k = 0; k < 2;k++){
				if(tt->dec[(2*i)+k]->last == state)
					return (2*i)+k;
			}		
		
		}
	}

}
void decoder(int *output){

	int i,j,k;
	int flag[4] = {0,0,0,0};
	//tam = 2;
	int current = 0;
	tbb = (tab_t *)malloc(sizeof(tab_t)*(tam/2));
	initStructs();
//	printf("\nttttt: %d", tam/2);
	fillMatrix();
	

	//Inicializa a decodificacao assumindo estado 00
	for(j = 0; j < 2;j++){
		tbb[0].dec[j]->recv = getState(output,0);
		tbb[0].dec[j]->erro = getDiffError(tbb[0].dec[j]->emit,tbb[0].dec[j]->recv); 
	}	
	tbb[0].qtd+=2;
	//marca estado 00 como ativo
	tbb[0].atv[0]=1;
	//printf("\n");
	//for(j =0 ; j < 2;j++){
	//	printf("%d %d %d %d %d %d \n", tbb[0].dec[j]->init,tbb[0].dec[j]->ent,tbb[0].dec[j]->emit,tbb[0].dec[j]->recv,tbb[0].dec[j]->last,tbb[0].dec[j]->erro);	
	
	//}
	
	
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
	tbb[1].atv[0] = 1;
	tbb[1].atv[2] = 1;
	//current+=2;
	//printf("\n");
	for(j = 0; j < 8;j++){
		
		tbb[2].dec[j]->recv = getState(output,4);
		int d = find(&tbb[1],tbb[2].dec[j]->init);	
		tbb[2].dec[j]->erro = tbb[1].dec[d]->erro + getDiffError(tbb[2].dec[j]->emit,tbb[2].dec[j]->recv);
		//printf("%d %d\n",getDiffError(tbb[2].dec[j]->emit,tbb[2].dec[j]->recv),d);
	
	}
	printf("\n");
	
	
	for(k = 0; k < 4;k++)tbb[2].atv[k]=1;
	current+=3;
	
	
	
	
	
	
	
	for(j = 6; j < tam;j+=2){
		/*for(k = 0; k < 4;k++){
			if(tbb[current-1].atv[k] == 1){
				int *v=ativa(&tbb[current]);
				tbb[current].atv[v[0]]= flag[v[0]] =1;
				tbb[current].atv[v[1]]=flag[v[0]] =1;
				tbb[current].atv[k] = 1;			
			}
			else if(flag[k] == 0){
					tbb[current].atv[k] = tbb[current-1].atv[k];		
			}
		}*/
		//for(k = 0; k < 4; k++)
		
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
	
	debugDecoder();	
	
	
	
	//}
	
	return;
}

int main(int argc, char* argv[]){

	int i = 0;
	int *output = NULL;
	//printf("fwfjjrh");
	//genTableValues();
		
	//lê tamanho da entrada
	scanf("%d", &tam);
	
	//le entrada
	input = (int *)malloc(sizeof(int)*(tam+2));
	while(i < tam){	
		scanf("%d", &input[i++]);
	}
	output = encoder();
	decoder(output);
	//for(i = 0; i < tam;i++)
	//	printf("%d ", input[i]);
	//for(i = 0; i < 8;i+=2){
	//	printf("%d %d \n", tbl->valEmit[i],tbl->valEmit[i+1]);
	//	printf("%d %d \n", tbl->valProx[i],tbl->valProx[i+1]);
	//}
	
	return 0;
	
	
}
