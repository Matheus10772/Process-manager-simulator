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

PCB* getLastElementOfPCBTable(PCB* elementoPCB) {
	if (elementoPCB)
		return tabelaPCB.lastElement;
	else
		return NULL;
}

void processReadyQueUE(PCB* pcbElement) { //Coloca o indice do elemento desejado na lista de processo prontos
	if (_ReadyProcess) {
		ReadyProcess* temp = _ReadyProcess;
		while (temp->after)
		{
			temp = temp->after;
		}
		temp->atual = pcbElement;
		temp->after = (ReadyProcess*)malloc(sizeof(ReadyProcess) * 1);
		temp->after->after = NULL;
		temp->after->before = temp;
	}
	else {
		_ReadyProcess = (ReadyProcess*)malloc(sizeof(ReadyProcess) * 1);
		_ReadyProcess->atual = pcbElement;
		_ReadyProcess->after = NULL;
		_ReadyProcess->before = NULL;
	}
}

void processReadyRemoveQueUE(PCB* pcbElement) {//Remove o indice do elemento desejado da lista de processos prontos
	ReadyProcess* temp = _ReadyProcess;
	if (temp->atual == pcbElement) {
		_ReadyProcess = temp->after;
		//temp->after->before = NULL;
		_ReadyProcess->before = NULL;
		free(temp);
	}
	else
	{
		while (temp->after)
		{
			if (temp->atual == pcbElement) {
				temp->before->after = temp->after;
				temp->after->before = temp->before;
				free(temp);
				break;
			}
		}
	}
	
}

void processBlokedQueUE(PCB* pcbElement) { // Coloca o indice do elemento desejado na lista de elementos bloqueados
	if (_BlokedProcess) {
		BlokedProcess* temp = _BlokedProcess;
		while (temp->after)
		{
			temp = temp->after;
		}
		temp->atual = pcbElement;
		temp->after = (BlokedProcess*)malloc(sizeof(BlokedProcess) * 1);
		temp->after->after = NULL;
		temp->after->before = temp;
	}
	else {
		_BlokedProcess = (BlokedProcess*)malloc(sizeof(BlokedProcess) * 1);
		_BlokedProcess->atual = pcbElement;
		_BlokedProcess->after = NULL;
		_BlokedProcess->before = NULL;
	}
}

void processBlokedRemoveQueUE(PCB* pcbElement) { //remove o indice do elemento desejado da lista de processos bloqueados
	BlokedProcess* temp = _BlokedProcess;
	if (temp->atual == pcbElement) {
		_BlokedProcess = temp->after;
		//temp->after->before = NULL;
		_BlokedProcess->before = NULL;
		free(temp);
	}

	else
	{
		while (temp->after)
		{
			if (temp->atual == pcbElement) {
				temp->before->after = temp->after;
				temp->after->before = temp->before;
				free(temp);
				break;
			}
		}
	}
	
}

void updateLastElementOfPCBTable(PCB* elementoPCB) { //Atualiza o ponteiro que aponta para o último elemento da tabela PCB. Ponteiro útil na hora de criar um novo processo simulado
	if(elementoPCB)
		tabelaPCB.lastElement = elementoPCB;
}

void createNewProcess(PCB* pcbCalled, processoSimulado* processCalled,int priority) { //cria um novo processo e coloca o indice na lista de processos prontos
	processoSimulado* newProcess = (processoSimulado*)malloc(sizeof(processoSimulado) * 1); //Cria uma nova estrutura que representa um novo processo

	PCB* LastElementOfPCBTable = getLastElementOfPCBTable(pcbCalled);
	
	if (pcbCalled && LastElementOfPCBTable->ID) //Se passar nesse teste, significa que não é o primeiro elemento da tabela PCB
	{
		/*Adiciona mais uma entrada na tabela PCB*/
		LastElementOfPCBTable->after = (PCB*)malloc(sizeof(PCB) * 1);
		LastElementOfPCBTable->after->before = LastElementOfPCBTable;
		LastElementOfPCBTable = LastElementOfPCBTable->after;
		updateLastElementOfPCBTable(LastElementOfPCBTable);
		/*Fim da adição*/
		
		/*Defini o ID do processo pai, caso o novo processo simulado criado não seja o primeiro processo*/
		LastElementOfPCBTable->parentID = pcbCalled->ID;
		newProcess->VariavelManipulada = processCalled->VariavelManipulada;
		/*Fim da atribuição do ID do processo pai*/

		/*Copiando dados do processo pai para o processo filho*/
		//_CPU.programInstructionsList = *(_CPU.programInstructionsList);
		/*Fim da cópia*/
	}
	else //Significa que é o primeiro elemento da tabela PCB
	{
		/*Adiciona mais uma entrada na tabela PCB*/
		LastElementOfPCBTable = (PCB*)malloc(sizeof(PCB) * 1);
		LastElementOfPCBTable->after = NULL;
		LastElementOfPCBTable->before = NULL;
		tabelaPCB.firstElement = LastElementOfPCBTable;
		updateLastElementOfPCBTable(LastElementOfPCBTable);
		/*Fim da adição*/

		/*Defini o ID do processo pai, caso o novo processo simulado criado não seja o primeiro processo*/
		LastElementOfPCBTable->parentID = '\0';
		newProcess->VariavelManipulada = 0;
		/*Fim da atribuição do ID do processo pai*/

		/*Copiando dados do processo pai para o processo filho*/
		//_CPU.programInstructionsList = ReadInit(); //criar a afunção ReadInit?
		/*Fim da cópia*/
	}
	LastElementOfPCBTable->ID = getNewID(); //Obtem um ID para o novo processo simulado criado
	LastElementOfPCBTable->ProgramCounterTotal = 0;

	/*Definindo informações remanascentes*/
	LastElementOfPCBTable->priority = priority;
	LastElementOfPCBTable->startTime = tempo;
	LastElementOfPCBTable->status = READY_STATUS;
	LastElementOfPCBTable->_ProcessoSimulado = newProcess;
	/*Fim da definição remanescente*/
	 
	processReadyQueUE(LastElementOfPCBTable);
}

int main() {
	char teste;
	while (1)
	{

		//printf("Process manager");
		scanf("%c", &teste);
		printf("\nTeste: %c", teste);
	}
}