#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct tb{

	int *valEmit;
	int *valProx;

}table_t;

int tam;
int *input;

typedef struct dc{

	int init;
	int ent;
	int emit;
	int recv;
	int last;
	int erro;

}decoder_t;

typedef struct tbl{

	int atv[4];	
	decoder_t **dec;
	int qtd;

}tab_t;