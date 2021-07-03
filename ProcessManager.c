#include "ProcessManager.h"

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

void processReadyQueUE(int indice) { //Coloca o indice do elemento desejado na lista de processo prontos
	int priority = tabelaPCB[indice].priority;
	if (priority == LOW_PRIORITY) {
		ReadyProcessLowPriority = reallocVector(ReadyProcessLowPriority, &indiceVetorReadyProcessLowPriority, &sizeOfVetorReadyProcessLowPriority, 'i');
		indiceVetorReadyProcessLowPriority++;
		ReadyProcessLowPriority[indiceVetorReadyProcessLowPriority] = indice;
	}
	else if (priority == NORMAL_PRIORITY) {
		ReadyProcessNormalPriority = reallocVector(ReadyProcessNormalPriority, &indiceVetorReadyProcessNormalPriority, &sizeOfVetorReadyProcessNormalPriority, 'i');
		indiceVetorReadyProcessNormalPriority++;
		printf("indice: %d", indice);
		ReadyProcessNormalPriority[indiceVetorReadyProcessNormalPriority] = indice;
	}
	else if (priority == HIGH_PRIORITY) {
		ReadyProcessHighPriority = reallocVector(ReadyProcessHighPriority, &indiceVetorReadyProcessHighPriority, &sizeOfVetorReadyProcessHighPriority, 'i');
		indiceVetorReadyProcessHighPriority++;
		ReadyProcessHighPriority[indiceVetorReadyProcessHighPriority] = indice;
	}
}

void processReadyRemoveQueUE(int indice) {//Remove o indice do elemento desejado da lista de processos prontos
	int priority = tabelaPCB[indice].priority;
	if (priority == LOW_PRIORITY) {
		popBackVector(ReadyProcessLowPriority, &indice, indiceVetorReadyProcessLowPriority, 'i');
		indiceVetorReadyProcessLowPriority--;
	}
	else if (priority == NORMAL_PRIORITY) {
		popBackVector(ReadyProcessNormalPriority, &indice, indiceVetorReadyProcessNormalPriority, 'i');
		indiceVetorReadyProcessNormalPriority--;
	}
	else if (priority == HIGH_PRIORITY) {
		popBackVector(ReadyProcessHighPriority, &indice, indiceVetorReadyProcessHighPriority, 'i');
		indiceVetorReadyProcessHighPriority--;
	}
}

void processBlokedQueUE(int indice) { // Coloca o indice do elemento desejado na lista de elementos bloqueados
	int priority = tabelaPCB[indice].priority;
	if (priority == LOW_PRIORITY) {
		BlokedProcessLowPriority = reallocVector(BlokedProcessLowPriority, &indiceVetorBlokedProcessLowPriority, &sizeOfVetorBlokedProcessLowPriority, 'i');
		indiceVetorBlokedProcessLowPriority++;
		BlokedProcessLowPriority[indiceVetorBlokedProcessLowPriority] = indice;
	}
	else if (priority == NORMAL_PRIORITY) {
		BlokedProcessNormalPriority = reallocVector(BlokedProcessNormalPriority, &indiceVetorBlokedProcessNormalPriority, &sizeOfVetorBlokedProcessNormalPriority, 'i');
		indiceVetorBlokedProcessNormalPriority++;
		BlokedProcessNormalPriority[indiceVetorBlokedProcessNormalPriority] = indice;
	}
	else if (priority == HIGH_PRIORITY) {
		BlokedProcessHighPriority = reallocVector(BlokedProcessHighPriority, &indiceVetorBlokedProcessHighPriority, &sizeOfVetorBlokedProcessHighPriority, 'i');
		indiceVetorBlokedProcessHighPriority++;
		BlokedProcessHighPriority[indiceVetorBlokedProcessHighPriority] = indice;
	}
}

void processBlokedRemoveQueUE(int indice) { //remove o indice do elemento desejado da lista de processos bloqueados
	int priority = tabelaPCB[indice].priority;
	if (priority == LOW_PRIORITY) {
		popBackVector(BlokedProcessLowPriority, &indice, indiceVetorBlokedProcessLowPriority, 'i');
		indiceVetorBlokedProcessLowPriority--;
	}
	else if (priority == NORMAL_PRIORITY) {
		popBackVector(BlokedProcessNormalPriority, &indice, indiceVetorBlokedProcessNormalPriority, 'i');
		indiceVetorBlokedProcessNormalPriority--;
	}
	else if (priority == HIGH_PRIORITY) {
		popBackVector(BlokedProcessHighPriority, &indice, indiceVetorBlokedProcessHighPriority, 'i');
		indiceVetorBlokedProcessHighPriority--;
	}
}

void* popBackVector(const void* vetor, void* valueOfremove, int indiceMaximo, char tipo)
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
			if ((_vetor+(sizeof(PCB)*i))->ID == ((PCB*)valueOfremove)->ID) {
				indiceOfVetorTabelaPCB--;
				int j;
				for (j = i; j < indiceMaximo - 1; j++) {
					memcpy((_vetor + (sizeof(PCB) * (j + 1))), (_vetor + (sizeof(PCB) * j)), sizeof(*(_vetor + (sizeof(PCB) * j))));
				}
			}
		}
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

int scheduler(int priorityEspecific) {
	if (indiceVetorReadyProcessHighPriority >= 0) {
		int indice = ReadyProcessHighPriority[indiceVetorReadyProcessHighPriority];
		indiceVetorReadyProcessHighPriority--;
		tabelaPCB[indice].priority = NORMAL_PRIORITY;
		return indice;
	}
	else if(indiceVetorReadyProcessNormalPriority >= 0){
		int indice = ReadyProcessNormalPriority[indiceVetorReadyProcessNormalPriority];
		indiceVetorReadyProcessNormalPriority--;
		tabelaPCB[indice].priority = LOW_PRIORITY;
		return indice;
	}
	else if (indiceVetorReadyProcessLowPriority >= 0) {
		int indice = ReadyProcessLowPriority[indiceVetorReadyProcessLowPriority];
		indiceVetorReadyProcessLowPriority--;
		return indice;
	}
	else {
		return NULL;
	}
}

void createNewProcess(PCB* pcbCalled, int priority) { //cria um novo processo e coloca o indice na lista de processos prontos
	processoSimulado* newProcess = (processoSimulado*)malloc(sizeof(processoSimulado) * 1); //Cria uma nova estrutura que representa um novo processo

	reallocVector(tabelaPCB, &indiceOfVetorTabelaPCB, &sizeOfVetorTabelaPCB, 'p');
	PCB* LastElementOfPCBTable = getLastElementOfPCBTable();
	printf("\nIndice antes incremento:%d ", indiceOfVetorTabelaPCB);
	indiceOfVetorTabelaPCB++;
	if (pcbCalled->ID != -1) //Se passar nesse teste, significa que não é o primeiro elemento da tabela PCB
	{
		newProcess->VariavelManipulada = (pcbCalled->_ProcessoSimulado)->VariavelManipulada;
		*(newProcess->programInstructionsList) = *(pcbCalled->_ProcessoSimulado->programInstructionsList); // conferir isso

		/*Defini o ID do processo pai como sendo o ID do processo que chamou a função*/
		LastElementOfPCBTable->parentID = pcbCalled->ID;
		/*A execução do processo filho continua de onde o processo pai parou*/
		LastElementOfPCBTable->ProgramCounter = pcbCalled->ProgramCounter;
	}
	else //Significa que é o primeiro elemento da tabela PCB
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

	return tabelaPCB;
}

int main() {
	char teste;
	scanf("%c", &teste);
	PCB* _PCB = initiManager(); // retorna o vetor de structs PCB
	createNewProcess(_PCB, NORMAL_PRIORITY);
	printf("\nIndice do vetor de prontos: %d\n", ReadyProcessNormalPriority[indiceVetorReadyProcessNormalPriority]);
	printf("\nId do elemento PCB: %d\n", _PCB[indiceOfVetorTabelaPCB].ID);
	processReadyRemoveQueUE(ReadyProcessNormalPriority[indiceVetorReadyProcessNormalPriority]);
	popBackVector(_PCB, &_PCB[1], indiceOfVetorTabelaPCB, 'p');
	printf("indice da tabela PCB apos decremento: %d", indiceOfVetorTabelaPCB);
	printf("\nID: %d", _PCB[indiceOfVetorTabelaPCB].ID);
	//printf("\nTeste bem sucedido: %c\n", teste);
	//getc(stdin);
	return 0;
}