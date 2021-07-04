#include "ProcessManager.h"

const int PIPE_FALIED_VALUE = -1;
const int FORK_FALIED_VALUE = -1;
pid_t  childpid = -1;

int* criarpipe() { //Fun��o usada para criar um pipe com o processo filho
	int* pipefd = (int*)malloc(sizeof(int) * 2);
	int* statusvalue = (int*)malloc(sizeof(int) * 1);
	statusvalue[0] = pipe(pipefd);
	if (*(statusvalue) == PIPE_FALIED_VALUE) { //a chamada pipe retorna '-1' se a cria��o do pipe n�o for bem sucedida
		perror("Erro ao criar um novo pipe");
		return statusvalue;
	}
	return pipefd;
}

pid_t criarFork() { //Executa o fork para criar o processo filho
	pid_t pid;
	pid = fork();
	if (pid == FORK_FALIED_VALUE) {
		perror("Erro ao criar uma c�pia do processo (fork)");
	}
	return pid;
}

void redirecionarEntrada(int pipefd[], int entrada, int destino) {//Faz a entrada do processo filho ser a sa�da do pipe do processo pai
	dup2(pipefd[destino], entrada);
	close(pipefd[destino]);
}


int getNewID() {//Entrega um novo ID para um novo processo sempre que for criado
	if (recycleIDcount > 0) {
		recycleIDcount--;
		return recycleID[recycleIDcount+1];
	}
	else {
		generatyID++;
		return generatyID;
	}
}

void removeID(int ID) {//Adiciona o ID de um processo que foi finalizado ao vetor de ID's que podem ser reutilizados
	recycleIDcount++;
	recycleID[recycleIDcount] = ID;
}

void* reallocVector(void* vetor, int* indice, int* valorMaximo, char tipo) {//Ralocar o tamanho do vetor, caso seja necess�rio
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

void removeFromVector(const void* vetor, void* valueOfremove, int indiceMaximo, char tipo) {//Remove um determinado elemento de um vetor e puxa os itens remanescente para as primeiras posi��es
																							//De modo a n�o ficar "buracos" no vetor
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

PCB* getLastElementOfPCBTable() {//Retorna o �ltimo elemento do vetor de tabela PCB

	return &tabelaPCB[indiceOfVetorTabelaPCB+1];
}

void createNewProcess(PCB* pcbCalled, int priority) { //cria um novo processo e coloca o indice na lista de processos prontos
	processoSimulado* newProcess = (processoSimulado*)malloc(sizeof(processoSimulado) * 1); //Cria uma nova estrutura que representa um novo processo
	tabelaPCB = (PCB*)reallocVector(tabelaPCB, &indiceOfVetorTabelaPCB, &sizeOfVetorTabelaPCB, 'p');
	PCB* LastElementOfPCBTable = getLastElementOfPCBTable();
	indiceOfVetorTabelaPCB++;
	if (pcbCalled->ID != -1) //Se passar nesse teste, significa que n�o � o primeiro elemento da tabela PCB
	{
		newProcess->VariavelManipulada = (pcbCalled->_ProcessoSimulado)->VariavelManipulada;
		newProcess->programInstructionsList = pcbCalled->_ProcessoSimulado->programInstructionsList;

		LastElementOfPCBTable->parentID = pcbCalled->ID;
		LastElementOfPCBTable->ProgramCounter = pcbCalled->ProgramCounter;
	}
	else //Significa que � o primeiro elemento da tabela PCB
	{
		;
		//LastElementOfPCBTable = pcbCalled;
		newProcess->programInstructionsList = readFile("init"); //conferir isso
		newProcess->VariavelManipulada = 0;
		LastElementOfPCBTable->parentID = -1;
		LastElementOfPCBTable->ProgramCounter = 0;
	}

	LastElementOfPCBTable->totalElapsedTime = 0;
	LastElementOfPCBTable->_ProcessoSimulado = newProcess;
	LastElementOfPCBTable->status = READY_STATUS;
	LastElementOfPCBTable->startTime = tempo;
	LastElementOfPCBTable->ID = getNewID();
	LastElementOfPCBTable->priority = priority;
	processReadyQueUE(indiceOfVetorTabelaPCB);
}

int scheduler() {//Retorna o indice de algum processo que deve ser escalado, ou seja, que entrar� em execu��o
	if (indiceVetorReadyProcessHighPriority >= 0) {
		int indice = processReadyRemoveQueUE(0, HIGH_PRIORITY);
		indiceVetorReadyProcessHighPriority--;
		tabelaPCB[indice].priority = NORMAL_PRIORITY;
		return indice;
	}
	else if (indiceVetorReadyProcessNormalPriority >= 0) {
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

void contextChange(int indice) {//Muda qual processo ficar� em execu��o na CPU
	_CPU.currentTime = 0;
	_CPU.ProgramCounter = tabelaPCB[indice].ProgramCounter;
	_CPU.programInstructionsList = tabelaPCB[indice]._ProcessoSimulado->programInstructionsList;
	_CPU.VariavelManipulada = &(tabelaPCB[indice]._ProcessoSimulado->VariavelManipulada);
}

char** splitString(char* string) {//divide uma string em v�rias sub-strings mediante ao delimitador fornecido
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

char** readFile(char arqName[]) {//L� um arquivo de instru��es de um processo, mediante ao fornecimento do nome do arquivo
	char* _arqName = malloc(sizeof(char) * (strlen(arqName) + 6));
	sprintf(_arqName, "%s", "./");
	strcat(_arqName, arqName);
	strcat(_arqName, ".txt");
	FILE* arq = fopen(_arqName, "r");
	if (arq == NULL) {
		printf("N�o foi poss�vel abrir o arquivo: %s", _arqName);
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
		linhas[indice] = strdup(linha);
		indice++;
	}

	return linhas;
}

void replaceProgramList(CPU* __CPU, char arqName[]) {//Substitu� a lista de instru��es do processo atualmente em execu��o
	char** novoArquivo = readFile(arqName);
	__CPU->programInstructionsList = novoArquivo;
	__CPU->ProgramCounter = 0;
	*(__CPU->VariavelManipulada) = 0;
}

void updateValue(CPU* __CPU, int n) {//Seta o valor da variavel para instru��o do tipo S, atualizando o valor para N
	*(__CPU->VariavelManipulada) = n;
}

void sumValue(CPU* __CPU, int n) {// Instru��o do tipo A, realiza a soma da variavel inteira mais N (Variavel  + N)
	*(__CPU->VariavelManipulada) += n;
}

void subtractValue(CPU* __CPU, int n) {// Instru��o do tipo D, subtrai N da variavel inteira (Variavel - N)
	*(__CPU->VariavelManipulada) -= n;
}

void execInstruction(CPU* __CPU) {//Executa a fun��o correspondente da Lista de instru��es do processo em execu��o
	char** executar = splitString(__CPU->programInstructionsList[__CPU->ProgramCounter]);
	executar[0][0] = (char)toupper(executar[0][0]);
	if (executar[0][0] == 'S') {
		updateValue(__CPU, atoi(executar[1]));
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
	/*else {
		printf("\nComando n�o encontrado2\n");
	}*/

}

void finishUnitTime(CPU* __CPU) { //Incrementa o contador de programa, para executar a pr�xima instru��o, incrementa o tempo
								  //e o tempo de CPU pelo processo atualmente em execu��o
	execInstruction(__CPU);
	__CPU->ProgramCounter++;
	tempo++;
	__CPU->currentTime++;
}

void blockExecutingProcess(CPU* __CPU, int indice) { //Bloqueia o processo atualmente em execu��o
	processBlokedQueUE(indice);
	tabelaPCB[indice].ProgramCounter = __CPU->ProgramCounter;
	tabelaPCB[indice].priority = HIGH_PRIORITY;
	tabelaPCB[indice].totalElapsedTime += __CPU->currentTime;
	tabelaPCB[indice].status += BLOKED_STATUS;
	ExecutingProcess = scheduler();
	contextChange(ExecutingProcess);
}

void finishExecutingProcess(int indice) {//Finaliza o processo atualemente em execu��o
	removeID(tabelaPCB[indice].ID);
	removeFromVector(tabelaPCB, &(tabelaPCB[indice]), indiceOfVetorTabelaPCB, 'p');
	ExecutingProcess = scheduler();
	contextChange(ExecutingProcess);
}

void sendForReporter(int indice, int pipefd[])//Envia uma estrutura PCB atrav�s do pipe para o processReporter
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

PCB* initiManager() { //Fun��o que cria uma nova estrutura de dados que representa uma CPU, aloca mem�ria para os vetores de �ndices dos processos e 
					  //inicializa o vetor de tabelas PCB
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
	contextChange(scheduler());
	return tabelaPCB;
}

int main() {
	int* pipefd;
	PCB* _tabelaPCB = initiManager(); // Array das entradas na tabela PCB
	char command = '\0';
	while (command != 'T')
	{
		command = '\0';
		setbuf(stdin, NULL);
		scanf("%c", &command);
		if (command == 'Q') {
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
		else if (command == 'P' || command == 'T') {
			pipefd = criarpipe();
			childpid = criarFork();
			if (childpid == 0) {
				close(pipefd[1]);
				redirecionarEntrada(pipefd, STDIN_FILENO, 0);
				execlp("./processReporter", "./processReporter", NULL);
			}
			close(pipefd[0]);

			write(pipefd[1], &tempo, sizeof(unsigned long int));
			/*Para o processo em execu��o*/
			write(pipefd[1], &tabelaPCB[ExecutingProcess].ID, sizeof(int));
			write(pipefd[1], &tabelaPCB[ExecutingProcess].parentID, sizeof(int));
			write(pipefd[1], &tabelaPCB[ExecutingProcess].ProgramCounter, sizeof(int));
			write(pipefd[1], &tabelaPCB[ExecutingProcess].startTime, sizeof(unsigned long int));
			unsigned long int tempoDeCPU = tabelaPCB[ExecutingProcess].totalElapsedTime + _CPU.currentTime;
			write(pipefd[1], &tempoDeCPU, sizeof(unsigned long int));
			write(pipefd[1], &tabelaPCB[ExecutingProcess].priority, sizeof(short int));
			write(pipefd[1], &tabelaPCB[ExecutingProcess].status, sizeof(short int));
			write(pipefd[1], &(tabelaPCB[ExecutingProcess]._ProcessoSimulado->VariavelManipulada), sizeof(int));
			//***************************************************************************************************

			/*Para os processos bloqueados*/
			int i;
			write(pipefd[1], &indiceVetorBlokedProcessLowPriority, sizeof(int));
			if (indiceVetorBlokedProcessLowPriority >= 0) {
				for (i = 0; i < indiceVetorBlokedProcessLowPriority; i++) {
					sendForReporter(BlokedProcessLowPriority[i], pipefd);
				}
			}

			write(pipefd[1], &indiceVetorBlokedProcessNormalPriority, sizeof(int));
			if (indiceVetorBlokedProcessNormalPriority >= 0) {
				for (i = 0; i < indiceVetorBlokedProcessNormalPriority; i++) {
					sendForReporter(BlokedProcessNormalPriority[i], pipefd);
				}
			}

			write(pipefd[1], &indiceVetorBlokedProcessHighPriority, sizeof(int));
			if (indiceVetorBlokedProcessHighPriority >= 0) {
				for (i = 0; i < indiceVetorBlokedProcessHighPriority; i++) {
					sendForReporter(BlokedProcessHighPriority[i], pipefd);
				}
			}
			//****************************************************************************************************

			/*Para os processos prontos*/
			write(pipefd[1], &indiceVetorReadyProcessLowPriority, sizeof(int));
			if (indiceVetorReadyProcessLowPriority >= 0) {
				for (i = 0; i < indiceVetorReadyProcessLowPriority; i++) {
					sendForReporter(ReadyProcessLowPriority[i], pipefd);
				}
			}

			write(pipefd[1], &indiceVetorReadyProcessNormalPriority, sizeof(int));
			if (indiceVetorReadyProcessNormalPriority >= 0) {
				for (i = 0; i < indiceVetorReadyProcessNormalPriority; i++) {
					sendForReporter(ReadyProcessNormalPriority[i], pipefd);
				}
			}

			write(pipefd[1], &indiceVetorReadyProcessHighPriority, sizeof(int));
			if (indiceVetorReadyProcessHighPriority >= 0) {
				for (i = 0; i < indiceVetorReadyProcessHighPriority; i++) {
					sendForReporter(ReadyProcessHighPriority[i], pipefd);
				}
			}
			//****************************************************************************************************

			wait(0);
			close(pipefd[1]);
			if (command == 'T')
				exit(0);
		}
		/*else {
			printf("\nComando n�o encontrado\n");
		}*/
	}
	return 0;
}