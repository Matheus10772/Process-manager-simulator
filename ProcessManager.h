#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

const short int LOW_PRIORITY = 50;
const short int NORMAL_PRIORITY = 70;
const short int HIGH_PRIORITY = 100;

const short int READY_STATUS = 1;
const short int EXECUTING_STATUS = 0;
const short int BLOKED_STATUS = -1;

/*Variáveis usadas para controlar a obtenção de ID's pelos processos*/
int generatyID = -1;
int recycleID[1024];
int recycleIDcount = 0;
//********************************************************************

unsigned long int tempo = 0;//Usado para contabilizar o tempo 

typedef struct CPU { //Struct que representa a CPU
	char** programInstructionsList; //Lista de instruções do processo
	int ProgramCounter; //Contador de programa do processo simulado atualmente em execução
	int* VariavelManipulada; //Ponteiro para acessar a variável do processo em execução
	unsigned long int currentTime; //Tempo gasto atualmente pelo processo em execução
}CPU;

typedef struct processosSimulado { //Struct que representa o processo simulado
	int VariavelManipulada; //Variavel inteira do processo
	char** programInstructionsList; //Lista de instruções do processo
}processoSimulado;

typedef struct PCB {
	int ID; //ID do processo 
	int parentID;//ID do processo pai
	int ProgramCounter;//Variavel para salvar o contador de programa do processo, caso o processo seja bloqueado
	unsigned long int startTime;//Variavel para guaradar o tempo de início do processo
	unsigned long int totalElapsedTime;//Variavel usada para guardar o tempo total de CPU gasto
	short int priority;//Variavel usada para definir a prioridade do processo
	short int status;//Variavel usada para guardar o estado do processo
	processoSimulado* _ProcessoSimulado;//Ponteiro para acessar o valo inteiro do processo simulado e sua lista de instruções
}PCB; 

CPU _CPU;
PCB* tabelaPCB;
int sizeOfVetorTabelaPCB = 128;
int indiceOfVetorTabelaPCB = -1;

/*vetor que contem os índices dos processos bloqueados com baixa prioridade e suas variáveis de controle*/
int* BlokedProcessLowPriority;
int indiceVetorBlokedProcessLowPriority = -1;
int sizeOfVetorBlokedProcessLowPriority = 128;
//*********************************************************************************************************

/*vetor que contem os índices dos processos bloqueados com prioridade normal e suas variáveis de controle*/
int* BlokedProcessNormalPriority;
int indiceVetorBlokedProcessNormalPriority = -1;
int sizeOfVetorBlokedProcessNormalPriority = 128;
//**********************************************************************************************************

/*vetor que contem os índices dos processos bloqueados com prioridade alta e suas variáveis de controle*/
int* BlokedProcessHighPriority;
int indiceVetorBlokedProcessHighPriority = -1;
int sizeOfVetorBlokedProcessHighPriority = 128;
//*******************************************************************************************************

/*vetor que contem os índices dos processos prontos com prioridade baixa e suas variáveis de controle*/
int* ReadyProcessLowPriority;
int indiceVetorReadyProcessLowPriority = -1;
int sizeOfVetorReadyProcessLowPriority = 128;
//*****************************************************************************************************

/*vetor que contem os índices dos processos prontos com prioridade normal e suas variáveis de controle*/
int* ReadyProcessNormalPriority;
int indiceVetorReadyProcessNormalPriority = -1;
int sizeOfVetorReadyProcessNormalPriority = 128;
//******************************************************************************************************

/*vetor que contem os índices dos processos prontos com prioridade alta e suas variáveis de controle*/
int* ReadyProcessHighPriority;
int indiceVetorReadyProcessHighPriority = -1;
int sizeOfVetorReadyProcessHighPriority = 128;
//*******************************************************************************************************

int ExecutingProcess; //Contem o índice do processo que está atualmente em execução

int* criarpipe();//Função usada para criar um pipe com o processo filho
pid_t criarFork();//Executa o fork para criar o processo filho
void redirecionarEntrada(int pipefd[], int entrada, int destino);//Faz a entrada do processo filho ser a saída do pipe do processo pai
int getNewID();//Entrega um novo ID para um novo processo sempre que for criado
void removeID(int ID);//Adiciona o ID de um processo que foi finalizado ao vetor de ID's que podem ser reutilizados
void* reallocVector(void* vetor, int* indice, int* valorMaximo, char tipo);//Ralocar o tamanho do vetor, caso seja necessário
void removeFromVector(const void* vetor, void* valueOfremove, int indiceMaximo, char tipo);//Remove um determinado elemento de um vetor e puxa os itens remanescente para as primeiras posições
																							//De modo a não ficar "buracos" no vetor
void processReadyQueUE(int indice);//Coloca o indice do elemento desejado na lista de processo prontos
int processReadyRemoveQueUE(int indice, int priority);//Remove o indice do elemento desejado da lista de processos prontos
void processBlokedQueUE(int indice);// Coloca o indice do elemento desejado na lista de elementos bloqueados
int processBlokedRemoveQueUE(int indice, int priority);//remove o indice do elemento desejado da lista de processos bloqueados
PCB* getLastElementOfPCBTable();//Retorna o último elemento do vetor de tabela PCB
void createNewProcess(PCB* pcbCalled, int priority);//cria um novo processo e coloca o indice na lista de processos prontos
int scheduler();//Retorna o indice de algum processo que deve ser escalado, ou seja, que entrará em execução
void contextChange(int indice);//Muda qual processo ficará em execução na CPU
char** splitString(char string[]);//divide uma string em várias sub-strings mediante ao delimitador fornecido
char** readFile(char arqName[]);//Lê um arquivo de instruções de um processo, mediante ao fornecimento do nome do arquivo
void replaceProgramList(CPU* __CPU, char arqName[]);//Substituí a lista de instruções do processo atualmente em execução
void updateValue(CPU* __CPU, int n);//Seta o valor da variavel para instrução do tipo S, atualizando o valor para N
void sumValue(CPU* __CPU, int n);// Instrução do tipo A, realiza a soma da variavel inteira mais N (Variavel  + N)
void subtractValue(CPU* __CPU, int n);// Instrução do tipo D, subtrai N da variavel inteira (Variavel - N)
void execInstruction(CPU* __CPU);//Executa a função correspondente da Lista de instruções do processo em execução
void finishUnitTime(CPU* __CPU);//Incrementa o contador de programa, para executar a próxima instrução, incrementa o tempo
								  //e o tempo de CPU pelo processo atualmente em execução
void blockExecutingProcess(CPU* __CPU, int indice);//Bloqueia o processo atualmente em execução
void finishExecutingProcess(int indice);//Finaliza o processo atualemente em execução
void sendForReporter(int indice, int pipefd[]);//Envia uma estrutura PCB através do pipe para o processReporter
PCB* initiManager();//Função que cria uma nova estrutura de dados que representa uma CPU, aloca memória para os vetores de índices dos processos e 
					  //inicializa o vetor de tabelas PCB