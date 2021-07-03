#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

const short int LOW_PRIORITY = 50;
const short int NORMAL_PRIORITY = 70;
const short int HIGH_PRIORITY = 100;

const short int READY_STATUS = 1;
const short int EXECUTING_STATUS = 0;
const short int BLOKED_STATUS = -1;

int generatyID = -1;
int recycleID[1024];
int recycleIDcount = 0;

unsigned long int tempo = 0;

typedef struct CPU {
	char*** programInstructionsList; //Lista de instru��es do processo
	int ProgramCounter; //(Tempo total)Carrega da tabela PCB o tempo do processo
	int* VariavelManipulada; //Carrega o valor do processo simulado
	unsigned long int currentTime; //Inicia do zero sempre que um novo processo for colocado em execu��o
}CPU;

typedef struct processosSimulado { //Struct que representa o processo simulado
	int VariavelManipulada;
	char*** programInstructionsList;
}processoSimulado;

typedef struct PCB {
	int ID;
	int parentID;
	int ProgramCounter;//Ponteiro que permite acessar o contador da CPU para armazenar o valo caso o proceso seja bloqueado
	unsigned long int startTime;
	unsigned long int totalElapsedTime;
	short int priority;
	short int status;
	processoSimulado* _ProcessoSimulado;//Ponteiro para acessar o valo inteiro do processo simulado
}PCB; 

	

CPU _CPU;
PCB* tabelaPCB;
int sizeOfVetorTabelaPCB = 128;
int indiceOfVetorTabelaPCB = -1;

int* BlokedProcessLowPriority;
int indiceVetorBlokedProcessLowPriority = -1;
int sizeOfVetorBlokedProcessLowPriority = 128;

int* BlokedProcessNormalPriority;
int indiceVetorBlokedProcessNormalPriority = -1;
int sizeOfVetorBlokedProcessNormalPriority = 128;

int* BlokedProcessHighPriority;
int indiceVetorBlokedProcessHighPriority = -1;
int sizeOfVetorBlokedProcessHighPriority = 128;

int* ReadyProcessLowPriority;
int indiceVetorReadyProcessLowPriority = -1;
int sizeOfVetorReadyProcessLowPriority = 128;

int* ReadyProcessNormalPriority;
int indiceVetorReadyProcessNormalPriority = -1;
int sizeOfVetorReadyProcessNormalPriority = 128;

int* ReadyProcessHighPriority;
int indiceVetorReadyProcessHighPriority = -1;
int sizeOfVetorReadyProcessHighPriority = 128;

int ExecutingProcess;

int getNewID();
void removeID(int ID);
void* reallocVector(void* vetor, int* indice, int* valorMaximo, char tipo);
void removeFromVector(const void* vetor, void* valueOfremove, int indiceMaximo, char tipo);
void processReadyQueUE(int indice);
void processReadyRemoveQueUE(int indice);
void processBlokedQueUE(int indice);
void processBlokedRemoveQueUE(int indice);
PCB* getLastElementOfPCBTable();
void contextChange(int indice);
int scheduler();
void createNewProcess(PCB* pcbCalled, int priority);
PCB* initiManager();

//Processo Simulado

void updateValue(CPU* __CPU, int n);
void sumValue(CPU* __CPU, int n);
void subtractValue(CPU* __CPU, int n);
char** splitString(char* string);
void execInstruction(CPU* __CPU);
void finishUnitTime(CPU* __CPU);
void blockExecutingProcess(CPU* __CPU, int indice);
void finishExecutingProcess(int indice);
char** readFile(char arqName[]);
void replaceProgramList(CPU* __CPU, char arqName[]);