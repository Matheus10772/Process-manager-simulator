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

int generatyID = -1;
int recycleID[1024];
int recycleIDcount = 0;

unsigned long int tempo = 0;

typedef struct CPU {
	char* programInstructionsList; //Lista de instruções do processo
	int ProgramCounter; //(Tempo total)Carrega da tabela PCB o tempo do processo
	int* VariavelManipulada; //Carrega o valor do processo simulado
	//unsigned long int tempoTotal;
	unsigned long int tempoAtual; //Inicia do zero sempre que um novo processo for colocado em execução
}CPU;

typedef struct processosSimulado { //Struct que representa o processo simulado
	int VariavelManipulada;
}processoSimulado;

typedef struct PCB {
	int ID;
	int parentID;
	int ProgramCounter;//Ponteiro que permite acessar o contador da CPU para armazenar o valo caso o proceso seja bloqueado
	processoSimulado* _ProcessoSimulado;//Ponteiro para acessar o valo inteiro do processo simulado
	short int priority;
	short int status;
	unsigned long int startTime;
	struct PCB* before; //ponteiro utilizado para apontar para o processo anterior, caso o processo esteja na lista de bloqueados ou de prontos
	struct PCB* after; //ponteiro utilizado para apontar para o processo, caso o processo esteja na lista de bloqueados ou de prontos
}PCB;

typedef struct PCBDescritor {
	PCB* firstElement;//ponteiro para o primeiro elemento da tabela PCB
	PCB* lastElement;//ponteiro para o ultimo elemento da tabela PCB
}PCBDescritor;

typedef struct ReadyProcess {
	PCB* atual;
	struct ReadyProcess* before;
	struct ReadyProcess* after;
}ReadyProcess;

typedef struct BlokedProcess {
	PCB* atual;
	struct BlokedProcess* before;
	struct BlokedProcess* after;
}BlokedProcess;


PCBDescritor tabelaPCB;//lista encadeada que vai conter as entradas para os processos que não terminaram a execução
CPU _CPU;
ReadyProcess* _ReadyProcess; //array(dinâmico) que vai conter os indices do array tabela PCB, e esses indices levam para os endereços de memória que contem processos prontos para executar
PCB* ExecutingProcess = NULL; //contém um indice tabela PCB. O processo acessado por esse índice, é o processo sendo executado no momento
BlokedProcess* _BlokedProcess; //array(dinâmico) que vai conter os índices da tabela PCB, que armazena os procesos que estão com execução pausada

int getNewID();
void removeID(int ID);
PCB* getLastElementOfPCBTable(PCB* elementoPCB);
void processReadyQueUE(PCB* pcbElement);
void processReadyRemoveQueUE(PCB* pcbElement);
void processBlokedQueUE(PCB* pcbElement);
void processBlokedRemoveQueUE(PCB* pcbElement);
void updateLastElementOfPCBTable(PCB* elementoPCB);
void createNewProcess(PCB* pcbCalled, processoSimulado* processCalled, int priority);