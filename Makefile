#   Universidade Federal da Fronteira Sul
# 
#                TRABALHO II
#   
#   Disciplina: Inteligência Artificial
#   Professor: José Carlos Bins Filho
#    
#   Aplicação do algoritmo de Viterbi para codificação/decodificação de bits. 
# 	
#
#   Alunos : Edson Lemes da Silva && Lucas Cezar Parnoff
#
#
#  --- MAKEFILE DE COMPILAÇÃO E EXECUÇAO ---

all: prog

prog: viterbi.c viterbi.h
	gcc viterbi.c viterbi.h -o  main
