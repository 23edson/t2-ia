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
*   Compilação pelo arquivo Makefile
*   ou gcc viterbi.c viterbi.h -o m && ./m < data.in
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

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


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