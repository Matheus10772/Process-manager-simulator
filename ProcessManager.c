#include "ProcessManager.h"
const int PIPE_FALIED_VALUE = -1;
const int FORK_FALIED_VALUE = -1;
const char arqName[] = { "Exemplo1.txt" };
pid_t  childpid = -1;

int* criarpipe() {
	int* pipefd = (int*)malloc(sizeof(int) * 2);
	int* statusvalue = (int*)malloc(sizeof(int) * 1);
	statusvalue[0] = pipe(pipefd);
	if (*(statusvalue) == PIPE_FALIED_VALUE) { //a chamada pipe retorna '-1' se a criação do pipe não for bem sucedida
		perror("Erro ao criar um novo pipe");
		return statusvalue;
	}
	return pipefd;
}

pid_t criarFork() {
	pid_t pid;
	pid = fork();
	if (pid == FORK_FALIED_VALUE) {
		perror("Erro ao criar uma cópia do processo (fork)");
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

int processReadyRemoveQueUE(int indice, int priority) {//Remove o indice do elemento desejado da lista de processos prontos
	int processo;
	if (priority == LOW_PRIORITY) {
		processo = ReadyProcessLowPriority[indice];
		removeFromVector(ReadyProcessLowPriority, &indice, indiceVetorReadyProcessLowPriority, 'i');
		indiceVetorReadyProcessLowPriority--;
		return processo;
	}
	else if (priority == NORMAL_PRIORITY) {
		processo = ReadyProcessNormalPriority[indice];
		removeFromVector(ReadyProcessNormalPriority, &indice, indiceVetorReadyProcessNormalPriority, 'i');
		indiceVetorReadyProcessNormalPriority--;
		return processo;
	}
	else if (priority == HIGH_PRIORITY) {
		processo = ReadyProcessHighPriority[indice];
		removeFromVector(ReadyProcessHighPriority, &indice, indiceVetorReadyProcessHighPriority, 'i');
		indiceVetorReadyProcessHighPriority--;
		return processo;
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

int processBlokedRemoveQueUE(int indice, int priority) { //remove o indice do elemento desejado da lista de processos bloqueados
	int processo;
	if (priority == LOW_PRIORITY) {
		processo = BlokedProcessLowPriority[indice];
		removeFromVector(BlokedProcessLowPriority, &indice, indiceVetorBlokedProcessLowPriority, 'i');
		indiceVetorBlokedProcessLowPriority--;
		return processo;
	}
	else if (priority == NORMAL_PRIORITY) {
		processo = BlokedProcessNormalPriority[indice];
		removeFromVector(BlokedProcessNormalPriority, &indice, indiceVetorBlokedProcessNormalPriority, 'i');
		indiceVetorBlokedProcessNormalPriority--;
		return processo;
	}
	else if (priority == HIGH_PRIORITY) {
		processo = BlokedProcessHighPriority[indice];
		removeFromVector(BlokedProcessHighPriority, &indice, indiceVetorBlokedProcessHighPriority, 'i');
		indiceVetorBlokedProcessHighPriority--;
		return processo;
	}
}

PCB* getLastElementOfPCBTable()
{
	return &tabelaPCB[indiceOfVetorTabelaPCB+1];
}

void contextChange(int indice) {
	_CPU.currentTime = 0;
	_CPU.ProgramCounter = tabelaPCB[indice].ProgramCounter;
	_CPU.programInstructionsList = tabelaPCB[indice]._ProcessoSimulado->programInstructionsList;
	_CPU.VariavelManipulada = &(tabelaPCB[indice]._ProcessoSimulado->VariavelManipulada);
}

int scheduler() {
	if (indiceVetorReadyProcessHighPriority >= 0) {
		int indice = processReadyRemoveQueUE(0, HIGH_PRIORITY);
		indiceVetorReadyProcessHighPriority--;
		tabelaPCB[indice].priority = NORMAL_PRIORITY;
		return indice;
	}
	else if(indiceVetorReadyProcessNormalPriority >= 0){
		int indice = processReadyRemoveQueUE(0, NORMAL_PRIORITY);
		indiceVetorReadyProcessNormalPriority--;
		tabelaPCB[indice].priority = LOW_PRIORITY;
		return indice;
	}
	else if (indiceVetorReadyProcessLowPriority >= 0) {
		int indice = processReadyRemoveQueUE(0, LOW_PRIORITY);
		indiceVetorReadyProcessLowPriority--;
		return indice;
	}
	else {
		return NULL;
	}
}

void createNewProcess(PCB* pcbCalled, int priority) { //cria um novo processo e coloca o indice na lista de processos prontos
	processoSimulado* newProcess = (processoSimulado*)malloc(sizeof(processoSimulado) * 1); //Cria uma nova estrutura que representa um novo processo
	printf("\nAqui");
	tabelaPCB = (PCB*)reallocVector(tabelaPCB, &indiceOfVetorTabelaPCB, &sizeOfVetorTabelaPCB, 'p');
	printf("\nDepois");
	PCB* LastElementOfPCBTable = getLastElementOfPCBTable();
	printf("\nDps de pegar o ultimo item");
	//printf("\nIndice antes incremento:%d ", indiceOfVetorTabelaPCB);
	printf("asdada  ");
	indiceOfVetorTabelaPCB++;
	if (pcbCalled->ID != -1) //Se passar nesse teste, significa que não é o primeiro elemento da tabela PCB
	{
		newProcess->VariavelManipulada = (pcbCalled->_ProcessoSimulado)->VariavelManipulada;
		newProcess->programInstructionsList = pcbCalled->_ProcessoSimulado->programInstructionsList; // conferir isso

		/*Defini o ID do processo pai como sendo o ID do processo que chamou a função*/
		LastElementOfPCBTable->parentID = pcbCalled->ID;
		/*A execução do processo filho continua de onde o processo pai parou*/
		LastElementOfPCBTable->ProgramCounter = pcbCalled->ProgramCounter;
	}
	else //Significa que é o primeiro elemento da tabela PCB
	{
		printf("\nEntrei no else\n");
		//LastElementOfPCBTable = pcbCalled;
		newProcess->programInstructionsList = readFile("init"); //conferir isso
		printf("\nAqui\n");
		newProcess->VariavelManipulada = 0;
		LastElementOfPCBTable->parentID = -1;
		LastElementOfPCBTable->ProgramCounter = 0;
	}
	printf("\nSai do else\n");

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
	printf("Indice da tabela: %d", indiceOfVetorTabelaPCB);
	createNewProcess(&tabelaPCB[indiceOfVetorTabelaPCB+1], NORMAL_PRIORITY);
	printf("\nNovo processo criado\n");
	//*(tabelaPCB[indiceOfVetorTabelaPCB+1]._ProcessoSimulado->programInstructionsList) = readFile("init.txt");
	printf("Lista de instruções lida\n");
	contextChange(scheduler());
	printf("\ncontexto trocado\n");
	return tabelaPCB;
}

//Seta o valor da variavel para instrução do tipo S, atualizando o valor para N
void updateValue(CPU* __CPU, int n) {
	*(__CPU->VariavelManipulada) = n;
}

// Instrução do tipo A, realiza a soma da variavel inteira mais N (Variavel  + N)
void sumValue(CPU* __CPU, int n) {
	*(__CPU->VariavelManipulada) += n;
}
// Instrução do tipo D, subtrai N da variavel inteira (Variavel - N)
void subtractValue(CPU* __CPU, int n) {
	*(__CPU->VariavelManipulada) -= n;
}

char** splitString(char* string) {
	int quantidade = 0;
	int i = 0;
	char** strings = NULL, * pch = strtok(string, " ");
	while (pch != NULL) {
		strings = realloc(strings, sizeof(char*) * (quantidade + 1));
		strings[i] = malloc(strlen(pch) + 1); // + 1 para o terminador
		strcpy(strings[i++], pch);
		pch = strtok(NULL, " ");
	}

	return strings;
}

void execInstruction(CPU* __CPU) {
	char** executar = splitString(__CPU->programInstructionsList[__CPU->ProgramCounter]);
	executar[0][0] = (char)toupper(executar[0][0]);
	printf("\nProgram counter: %d", __CPU->ProgramCounter);
	printf("\nExecutar 00: %c", executar[0][0]);
	if (executar[0][0] == 'S') {
		printf("\nExecutando update\n");
		printf("\nvalor do update: %d\n", atoi(executar[1]));
		updateValue(__CPU, atoi(executar[1]));
		printf("\nValor da variável manipualda: %d\n", *(__CPU->VariavelManipulada));
	}
	else if (executar[0][0] == 'A') {
		sumValue(__CPU, atoi(executar[1]));
	}
	else if (executar[0][0] == 'D') {
		subtractValue(__CPU, atoi(executar[1]));
	}
	else if (executar[0][0] == 'B') {
		blockExecutingProcess(__CPU, ExecutingProcess);
	}
	else if (executar[0][0] == 'E') {
		finishExecutingProcess(ExecutingProcess);
	}
	else if (executar[0][0] == 'F') {
		createNewProcess(&(tabelaPCB[ExecutingProcess]), NORMAL_PRIORITY);
	}
	else if (executar[0][0] == 'R') {
		replaceProgramList(__CPU, executar[1]);
	}
	else {
		printf("\nComando não encontrado2\n");
	}
	
}

void finishUnitTime(CPU* __CPU) {
	execInstruction(__CPU);
	__CPU->ProgramCounter++;
	tempo++;
	__CPU->currentTime++;
	//Realiza o escalonamento?
}

void blockExecutingProcess(CPU* __CPU, int indice) {
	processBlokedQueUE(indice);
	tabelaPCB[indice].ProgramCounter = __CPU->ProgramCounter;
	tabelaPCB[indice].priority = HIGH_PRIORITY;
	tabelaPCB[indice].totalElapsedTime += __CPU->currentTime;
	tabelaPCB[indice].status += BLOKED_STATUS;
	ExecutingProcess = scheduler();
	contextChange(ExecutingProcess);
}

void finishExecutingProcess(int indice) {
	removeID(tabelaPCB[indice].ID);
	removeFromVector(tabelaPCB, &(tabelaPCB[indice]), indiceOfVetorTabelaPCB, 'p');
	ExecutingProcess = scheduler();
	contextChange(ExecutingProcess);
}

char** readFile(char arqName[]) {
	char* _arqName = malloc(sizeof(char)*(strlen(arqName) + 6));
	sprintf(_arqName, "%s", "./");
	strcat(_arqName, arqName);
	strcat(_arqName, ".txt");
	FILE* arq = fopen(_arqName, "r");
	if (arq == NULL) {
		printf("Não foi possível abrir o arquivo: %s", _arqName);
	}
	else {
		printf("\nSucesso\n");
	}
	char** linhas = (char**)malloc(sizeof(char*) * 1024);
	char linha[1024];
	unsigned int indice = 0;
	while (!feof(arq))
	{
		fgets(linha, sizeof(linha), arq);
		printf("\nLinha: %s", linha);
		linhas[indice] = strdup(linha);
		indice++;
	}

	return linhas;
}

void replaceProgramList(CPU* __CPU, char arqName[]) {
	char** novoArquivo = readFile(arqName);
	__CPU->programInstructionsList = novoArquivo;
	__CPU->ProgramCounter = 0;
	*(__CPU->VariavelManipulada) = 0;
}

void sendForReporter(int indice, int pipefd[])
{
	write(pipefd[1], &tabelaPCB[indice].ID, sizeof(int));
	write(pipefd[1], &tabelaPCB[indice].parentID, sizeof(int));
	write(pipefd[1], &tabelaPCB[indice].ProgramCounter, sizeof(int));
	write(pipefd[1], &tabelaPCB[indice].startTime, sizeof(unsigned long int));
	write(pipefd[1], &tabelaPCB[indice].totalElapsedTime, sizeof(unsigned long int));
	write(pipefd[1], &tabelaPCB[indice].priority, sizeof(short int));
	write(pipefd[1], &tabelaPCB[indice].status, sizeof(short int));
	write(pipefd[1], &(tabelaPCB[indice]._ProcessoSimulado->VariavelManipulada), sizeof(int));
}

int main() {
	int* pipefd;
	PCB* _tabelaPCB = initiManager(); // Array das entradas na tabela PCB
	char command = '\0';
	while (strcmp(&command, "T") != 0)
	{
		command = '\0';
		printf("\nDigite o comando\n");
		setbuf(stdin, NULL);
		scanf("%c", &command);
		if (command == 'Q') {
			printf("Função executada %c", command);
			finishUnitTime(&_CPU);
		}
		else if (command == 'U') {
			if (indiceVetorBlokedProcessHighPriority >= 0) {
				int indice = processBlokedRemoveQueUE(0, HIGH_PRIORITY);
				indiceVetorBlokedProcessHighPriority--;
				processReadyQueUE(indice);
			}
			else if (indiceVetorBlokedProcessNormalPriority >= 0) {
				int indice = processBlokedRemoveQueUE(0, NORMAL_PRIORITY);
				indiceVetorBlokedProcessNormalPriority--;
				processReadyQueUE(indice);
			}
			else if (indiceVetorBlokedProcessLowPriority >= 0) {
				int indice = processBlokedRemoveQueUE(0, LOW_PRIORITY);
				indiceVetorBlokedProcessLowPriority--;
				processReadyQueUE(indice);
			}
		}
		else if (command == 'P') {
			pipefd = criarpipe();
			childpid = criarFork();
			if (childpid == 0) {
				close(pipefd[1]);
				redirecionarEntrada(pipefd, STDIN_FILENO, 0);
				execlp("./processReporter", "./processReporter", NULL);
			}
			close(pipefd[0]);

			/*Para o processo em execução*/
			write(pipefd[1], &tabelaPCB[ExecutingProcess].ID, sizeof(int));
			write(pipefd[1], &tabelaPCB[ExecutingProcess].parentID, sizeof(int));
			write(pipefd[1], &tabelaPCB[ExecutingProcess].ProgramCounter, sizeof(int));
			write(pipefd[1], &tabelaPCB[ExecutingProcess].startTime, sizeof(unsigned long int));
			unsigned long int tempoDeCPU = tabelaPCB[ExecutingProcess].totalElapsedTime + _CPU.currentTime;
			write(pipefd[1], &tempoDeCPU, sizeof(unsigned long int));
			write(pipefd[1], &tabelaPCB[ExecutingProcess].priority, sizeof(short int));
			write(pipefd[1], &tabelaPCB[ExecutingProcess].status, sizeof(short int));
			write(pipefd[1], &(tabelaPCB[ExecutingProcess]._ProcessoSimulado->VariavelManipulada), sizeof(int));
			/*Término*/

			/*Para os processos bloqueados*/
			int i;
			for (i = 0; i < indiceVetorBlokedProcessLowPriority; i++) {
				sendForReporter(BlokedProcessLowPriority[i], pipefd);
			}

			for (i = 0; i < indiceVetorBlokedProcessNormalPriority; i++) {
				sendForReporter(BlokedProcessNormalPriority[i], pipefd);
			}

			for (i = 0; i < indiceVetorBlokedProcessHighPriority; i++) {
				sendForReporter(BlokedProcessHighPriority[i], pipefd);
			}

			close(pipefd[1]);
			//wait(0);
			/*printf("****************************************************************\n"
				"Estado do sistema:\n"
				"*******************************************************************\n"
				"Tempo Atual: %ld\n"
				"PROCESSO EXECUTANDO:\n"
				"%d, %d, %d, %d, %ld, %ld\n",
				tempo, tabelaPCB[ExecutingProcess].ID , tabelaPCB[ExecutingProcess].parentID, tabelaPCB[ExecutingProcess].priority, tabelaPCB[ExecutingProcess]._ProcessoSimulado->VariavelManipulada, tabelaPCB[ExecutingProcess].startTime, tabelaPCB[ExecutingProcess].totalElapsedTime+_CPU.currentTime);*/
		}
		else if (command == 'T') {
			//cria reporter
			exit(0);
		}
		else {
			printf("\nComando não encontrado\n");
		}
	}
	return 0;
}