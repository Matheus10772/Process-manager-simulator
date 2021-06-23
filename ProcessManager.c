#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

const short int LOW_PRIORITY = -1;
const short int NORMAL_PRIORITY = 0;
const short int HIGH_PRIORITY = 1;

const short int READY_STATUS = 1;
const short int EXECUTING_STATUS = 0;
const short int BLOKED_STATUS = -1;


unsigned long int tempo;

typedef struct CPU {
	char* programInstructionsList;
	int ProgramCounter;
	int VariavelManipulada;
	unsigned long int tempoTotal;
	unsigned long int tempoAtual;
}CPU;

typedef struct PCB {
	int ID;
	int parentID;
	int* ProgramCounter;
	int VariavelManipulada;
	short int prioridade;
	short int status;
	unsigned long int startTime;
	unsigned long int usedCPU;
}PCB;

PCB* tabelaPCB; //array(din�mico) que vai conter as entradas para os processos que n�o terminaram a execu��o
int* ReadyProcess;//array(din�mico) que vai conter os indices do array tabela PCB, e esses indices levam para os endere�os de mem�ria que contem processos prontos para executar
int ExecutingProcess; //cont�m um indice tabela PCB. O processo acessado por esse �ndice, � o processo sendo executado no momento
int* BlokedProcess; //array(din�mico) que vai conter os �ndices da tabela PCB, que armazena os procesos que est�o com execu��o pausada

typedef struct processosSimulado {
	int VariavelManipulada;
	CPU _CPU;
}processoSimulado;

int main() {
	int processID;
	scanf("%d", &processID);
}