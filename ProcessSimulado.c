#include "ProcessManager.h"
const char arqName[] = { "Exemplo1.txt" };

//Seta o valor da variavel para instrução do tipo S, atualizando o valor para N
void updateValue(CPU* __CPU, int n){
    *(__CPU->VariavelManipulada) = n;
}

// Instrução do tipo A, realiza a soma da variavel inteira mais N (Variavel  + N)
void sumValue(CPU* __CPU, int n){
    *(__CPU->VariavelManipulada) += n;
}
// Instrução do tipo D, subtrai N da variavel inteira (Variavel - N)
void subtractValue(CPU* __CPU, int n){
    *(__CPU->VariavelManipulada) -= n;
}

char** splitString(char* string) {
	int quantidade = 0;
	int i = 0;
	char** strings = NULL, * pch = strtok(str, " ");
	while (pch != NULL) {
		strings = realloc(strings, sizeof(char*) * (quantidade + 1));
		strings[i] = malloc(strlen(pch) + 1); // + 1 para o terminador
		strcpy(strings[i++], pch);
		pch = strtok(NULL, " ");
	}

	return strings;
}

void execInstruction(CPU* __CPU) {
	char** executar = splitString((*(__CPU->programInstructionsList))[__CPU->ProgramCounter]);
	executar[0][0] = (char)toupper(executar[0][0]);
	switch (executar[0][0])
	{
	case 'S':
		updateValue(__CPU, atoi(executar[1]));
		break;
	case 'A':
		sumValue(__CPU, atoi(executar[1]));
		break;
	case 'D':
		subtractValue(__CPU, atoi(executar[1]));
		break;
	case 'B':
		blockExecutingProcess(__CPU, ExecutingProcess);
		break;
	case 'E':
		finishExecutingProcess(ExecutingProcess);
		break;
	case 'F':
		createNewProcess(&(tabelaPCB[ExecutingProcess]), NORMAL_PRIORITY);
		break; 
	case 'R':
		replaceProgramList(__CPU, executar[1]);
	default:
		printf("\nComando não encontrado\n");
		break;
	}
}

void finishUnitTime(CPU* __CPU) {
	execInstruction(__CPU);
	__CPU->Programcounter++;
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
	char* _arqName = (char*)malloc(strlen(arqName) + 2);
	sprintf(_arqName, "%s", "./");
	strcat(_arqName, arqName);
	FILE* arq = fopen(_arqName, "r");
	char** linhas = (char**)malloc(sizeof(char*) * 1024);
	char linha[1024];
	unsigned int indice = 0;
	while (!feof(arq))
	{
		fgets(linha, sizeof(linha), arq);
		linhas[indice] = strdup(linha);
	}

	return linhas;
}

void replaceProgramList(CPU* __CPU, char arqName[]) {
	char** novoArquivo = readFile(arqName);
	*(__CPU->programInstructionsList) = novoArquivo;
	__CPU->ProgramCounter = 0;
	*(__CPU->VariavelManipulada) = 0;
}