#include "ProcessManager.h"
const int PIPE_FALIED_VALUE = -1;
const int FORK_FALIED_VALUE = -1;
pid_t  childpid;

int* criarpipe() {
	int* pipefd = (int*)malloc(sizeof(int) * 2);
	int* statusvalue = (int*)malloc(sizeof(int) * 1);
	statusvalue[0] = pipe(pipefd);
	if (*(statusvalue) == PIPE_FALIED_VALUE) { //a chamada pipe retorna '-1' se a cria��o do pipe n�o for bem sucedida
		perror("Erro ao criar um novo pipe");
		return statusvalue;
	}
	return pipefd;
}

pid_t criarFork() {
	pid_t pid;
	pid = fork();
	if (pid == FORK_FALIED_VALUE) {
		perror("Erro ao criar uma c�pia do processo (fork)");
	}
	return pid;
}

void redirecionarEntrada(int pipefd[], int entrada, int destino) {
	dup2(pipefd[destino], entrada);
	close(pipefd[destino]);
}


int getNewID() {
	if (recycleIDcount > 0) {
		recycleIDcount--;
		return recycleID[recycleIDcount+1];
	}
	else {
		generatyID++;
		return generatyID;
	}
}

void removeID(int ID) {
	recycleIDcount++;
	recycleID[recycleIDcount] = ID;
}

void* reallocVector(void* vetor, int* indice, int* valorMaximo, char tipo) {
	if (strcmp(&tipo, "i") == 0) {
		if ((*indice) == (*valorMaximo) - 1) {
			(*valorMaximo) = (*valorMaximo) + 128;
			vetor = realloc((int*)vetor, (*valorMaximo)*sizeof(int));
		}
	}
	else if (strcmp(&tipo, "p") == 0) {
		if ((*indice) == (*valorMaximo) - 1) {
			(*valorMaximo) = (*valorMaximo) + 128;
			vetor = realloc((PCB*)vetor, (*valorMaximo)*sizeof(PCB));
		}
	}
	return vetor;
}

void removeFromVector(const void* vetor, void* valueOfremove, int indiceMaximo, char tipo)
{
	if (strcmp(&tipo, "i") == 0) {
		int* _vetor = (int*)vetor;
		int i;
		for (i = 0; i < indiceMaximo; i++) {
			if (_vetor[i] == *((int*)valueOfremove)) {
				int j;
				for (j = i; j < indiceMaximo - 1; j++) {
					_vetor[j] = _vetor[j + 1];
				}
			}
		}
	}
	else if (strcmp(&tipo, "p") == 0) {
		PCB* _vetor = (PCB*)vetor;
		int i;
		for (i = 0; i < indiceMaximo; i++) {
			if (_vetor[i].ID == ((PCB*)valueOfremove)->ID) {
				int j;
				for (j = i; j < indiceMaximo - 1; j++) {
					memcpy(&(_vetor[i]), &(_vetor[i + 1]), sizeof(PCB));
				}
				indiceOfVetorTabelaPCB--;
			}
		}
	}
}


void processReadyQueUE(int indice) { //Coloca o indice do elemento desejado na lista de processo prontos
	int priority = tabelaPCB[indice].priority;
	if (priority == LOW_PRIORITY) {
		ReadyProcessLowPriority = (int*)reallocVector(ReadyProcessLowPriority, &indiceVetorReadyProcessLowPriority, &sizeOfVetorReadyProcessLowPriority, 'i');
		indiceVetorReadyProcessLowPriority++;
		ReadyProcessLowPriority[indiceVetorReadyProcessLowPriority] = indice;
	}
	else if (priority == NORMAL_PRIORITY) {
		ReadyProcessNormalPriority = (int*)reallocVector(ReadyProcessNormalPriority, &indiceVetorReadyProcessNormalPriority, &sizeOfVetorReadyProcessNormalPriority, 'i');
		indiceVetorReadyProcessNormalPriority++;
		ReadyProcessNormalPriority[indiceVetorReadyProcessNormalPriority] = indice;
	}
	else if (priority == HIGH_PRIORITY) {
		ReadyProcessHighPriority = (int*)reallocVector(ReadyProcessHighPriority, &indiceVetorReadyProcessHighPriority, &sizeOfVetorReadyProcessHighPriority, 'i');
		indiceVetorReadyProcessHighPriority++;
		ReadyProcessHighPriority[indiceVetorReadyProcessHighPriority] = indice;
	}
}

void processReadyRemoveQueUE(int indice) {//Remove o indice do elemento desejado da lista de processos prontos
	int priority = tabelaPCB[indice].priority;
	if (priority == LOW_PRIORITY) {
		removeFromVector(ReadyProcessLowPriority, &indice, indiceVetorReadyProcessLowPriority, 'i');
		indiceVetorReadyProcessLowPriority--;
	}
	else if (priority == NORMAL_PRIORITY) {
		removeFromVector(ReadyProcessNormalPriority, &indice, indiceVetorReadyProcessNormalPriority, 'i');
		indiceVetorReadyProcessNormalPriority--;
	}
	else if (priority == HIGH_PRIORITY) {
		removeFromVector(ReadyProcessHighPriority, &indice, indiceVetorReadyProcessHighPriority, 'i');
		indiceVetorReadyProcessHighPriority--;
	}
}

void processBlokedQueUE(int indice) { // Coloca o indice do elemento desejado na lista de elementos bloqueados
	int priority = tabelaPCB[indice].priority;
	if (priority == LOW_PRIORITY) {
		BlokedProcessLowPriority = (int*)reallocVector(BlokedProcessLowPriority, &indiceVetorBlokedProcessLowPriority, &sizeOfVetorBlokedProcessLowPriority, 'i');
		indiceVetorBlokedProcessLowPriority++;
		BlokedProcessLowPriority[indiceVetorBlokedProcessLowPriority] = indice;
	}
	else if (priority == NORMAL_PRIORITY) {
		BlokedProcessNormalPriority = (int*)reallocVector(BlokedProcessNormalPriority, &indiceVetorBlokedProcessNormalPriority, &sizeOfVetorBlokedProcessNormalPriority, 'i');
		indiceVetorBlokedProcessNormalPriority++;
		BlokedProcessNormalPriority[indiceVetorBlokedProcessNormalPriority] = indice;
	}
	else if (priority == HIGH_PRIORITY) {
		BlokedProcessHighPriority = (int*)reallocVector(BlokedProcessHighPriority, &indiceVetorBlokedProcessHighPriority, &sizeOfVetorBlokedProcessHighPriority, 'i');
		indiceVetorBlokedProcessHighPriority++;
		BlokedProcessHighPriority[indiceVetorBlokedProcessHighPriority] = indice;
	}
}

void processBlokedRemoveQueUE(int indice) { //remove o indice do elemento desejado da lista de processos bloqueados
	int priority = tabelaPCB[indice].priority;
	if (priority == LOW_PRIORITY) {
		removeFromVector(BlokedProcessLowPriority, &indice, indiceVetorBlokedProcessLowPriority, 'i');
		indiceVetorBlokedProcessLowPriority--;
	}
	else if (priority == NORMAL_PRIORITY) {
		removeFromVector(BlokedProcessNormalPriority, &indice, indiceVetorBlokedProcessNormalPriority, 'i');
		indiceVetorBlokedProcessNormalPriority--;
	}
	else if (priority == HIGH_PRIORITY) {
		removeFromVector(BlokedProcessHighPriority, &indice, indiceVetorBlokedProcessHighPriority, 'i');
		indiceVetorBlokedProcessHighPriority--;
	}
}

PCB* getLastElementOfPCBTable()
{
	return &tabelaPCB[indiceOfVetorTabelaPCB+1];
}

void contextChange(int indice) {
	_CPU.currentTime = 0;
	_CPU.ProgramCounter = tabelaPCB[indice].ProgramCounter;
	_CPU.programInstructionsList = *(tabelaPCB[indice]._ProcessoSimulado->programInstructionsList);
	_CPU.VariavelManipulada = &(tabelaPCB[indice]._ProcessoSimulado->VariavelManipulada);
}

int scheduler() {
	if (indiceVetorReadyProcessHighPriority >= 0) {
		int indice = ReadyProcessHighPriority[0];
		indiceVetorReadyProcessHighPriority--;
		tabelaPCB[indice].priority = NORMAL_PRIORITY;
		return indice;
	}
	else if(indiceVetorReadyProcessNormalPriority >= 0){
		int indice = ReadyProcessNormalPriority[0];
		indiceVetorReadyProcessNormalPriority--;
		tabelaPCB[indice].priority = LOW_PRIORITY;
		return indice;
	}
	else if (indiceVetorReadyProcessLowPriority >= 0) {
		int indice = ReadyProcessLowPriority[0];
		indiceVetorReadyProcessLowPriority--;
		return indice;
	}
	else {
		return NULL;
	}
}

void createNewProcess(PCB* pcbCalled, int priority) { //cria um novo processo e coloca o indice na lista de processos prontos
	processoSimulado* newProcess = (processoSimulado*)malloc(sizeof(processoSimulado) * 1); //Cria uma nova estrutura que representa um novo processo

	tabelaPCB = (PCB*)reallocVector(tabelaPCB, &indiceOfVetorTabelaPCB, &sizeOfVetorTabelaPCB, 'p');
	PCB* LastElementOfPCBTable = getLastElementOfPCBTable();
	printf("\nIndice antes incremento:%d ", indiceOfVetorTabelaPCB);
	indiceOfVetorTabelaPCB++;
	if (pcbCalled->ID != -1) //Se passar nesse teste, significa que n�o � o primeiro elemento da tabela PCB
	{
		newProcess->VariavelManipulada = (pcbCalled->_ProcessoSimulado)->VariavelManipulada;
		*(newProcess->programInstructionsList) = *(pcbCalled->_ProcessoSimulado->programInstructionsList); // conferir isso

		/*Defini o ID do processo pai como sendo o ID do processo que chamou a fun��o*/
		LastElementOfPCBTable->parentID = pcbCalled->ID;
		/*A execu��o do processo filho continua de onde o processo pai parou*/
		LastElementOfPCBTable->ProgramCounter = pcbCalled->ProgramCounter;
	}
	else //Significa que � o primeiro elemento da tabela PCB
	{
		LastElementOfPCBTable = pcbCalled;
		printf("Aqui");
		(newProcess->programInstructionsList) = readFile("init"); //conferir isso
		newProcess->VariavelManipulada = 0;
		LastElementOfPCBTable->parentID = '\0';
		LastElementOfPCBTable->ProgramCounter = 1;
	}


	LastElementOfPCBTable->totalElapsedTime = 0;
	LastElementOfPCBTable->_ProcessoSimulado = newProcess;
	LastElementOfPCBTable->status = READY_STATUS;
	LastElementOfPCBTable->startTime = tempo;
	LastElementOfPCBTable->ID = getNewID();
	LastElementOfPCBTable->priority = priority;
	processReadyQueUE(indiceOfVetorTabelaPCB);
}

PCB* initiManager() {
	_CPU.currentTime = 0;
	tabelaPCB = (PCB*)malloc(sizeof(PCB) * sizeOfVetorTabelaPCB);

	BlokedProcessLowPriority = (int*)malloc(sizeof(int) * sizeOfVetorBlokedProcessLowPriority);
	BlokedProcessNormalPriority = (int*)malloc(sizeof(int) * sizeOfVetorBlokedProcessNormalPriority);
	BlokedProcessHighPriority = (int*)malloc(sizeof(int) * sizeOfVetorBlokedProcessHighPriority);

	ReadyProcessLowPriority = (int*)malloc(sizeof(int) * sizeOfVetorReadyProcessLowPriority);
	ReadyProcessNormalPriority = (int*)malloc(sizeof(int) * sizeOfVetorReadyProcessNormalPriority);
	ReadyProcessHighPriority = (int*)malloc(sizeof(int) * sizeOfVetorReadyProcessHighPriority);

	tabelaPCB[indiceOfVetorTabelaPCB+1].ID = -1;
	createNewProcess(&tabelaPCB[indiceOfVetorTabelaPCB+1], NORMAL_PRIORITY);
	*(tabelaPCB[indiceOfVetorTabelaPCB+1]._ProcessoSimulado->programInstructionsList) = readFile("init.txt");
	contextChange(scheduler());
	return tabelaPCB;
}

int main() {
	int* pipefd;
	if (childpid == 0) {
		close(pipefd[1]);
		redirecionarEntrada(pipefd, STDIN_FILENO, 0);
		execlp("./processReporter", "./processReporter", NULL);
	}
	PCB* _tabelaPCB = initiManager(); // Array das entradas na tabela PCB
	char command;
	scanf("%c", &command);
	while (strcmp(&command, "T") != 0)
	{
		switch (command)
		{
		case 'Q':
			execInstruction(&_CPU);
			break;
		case 'U':
			processBlokedQueUE(0);
			break;
		case 'P':
			pipefd = criarpipe();
			childpid = criarFork();
			close(pipefd[0]);
			write(pipefd[1], sizeof(tabelaPCB[ExecutingProcess]), sizeof(int));
			write(pipefd[1], &tabelaPCB[ExecutingProcess], sizeof(tabelaPCB[ExecutingProcess]));
			close(pipefd[1]);
			break;
		case 'T':
			//cria reporter
			exit(0);
		default:
			printf("\nComando n�o econtrado\n");
			break;
		}
	}
	return 0;
}