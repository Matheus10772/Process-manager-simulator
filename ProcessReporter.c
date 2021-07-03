#include <stdio.h>
#include <string.h>
#include "ProcessManager.h"
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
	unsigned int sizeInBytes;
	unsigned long int tempo;
	PCB elementPCB;
	read(STDERR_FILENO, &sizeInBytes, sizeof(unsigned int));
	read(STDIN_FILENO, &elementPCB, sizeInBytes);
	read(STDIN_FILENO, &tempo, sizeof(unsigned long int));

	char prioridade[50];
	if (elementPCB.priority == LOW_PRIORITY) {
		sprintf(prioridade, "%s", "Prioridade baixa");
	}
	else if (elementPCB.priority == NORMAL_PRIORITY) {
		sprintf(prioridade, "%s", "Prioridade normal");
	}
	else if (elementPCB.priority == HIGH_PRIORITY) {
		sprintf(prioridade, "%s", "Prioridade alta");
	}

	printf("****************************************************************\n"
		"Estado do sistema:\n"
		"*******************************************************************\n"
		"Tempo Atual: %ld\n"
		"PROCESSO EXECUTANDO:\n"
		"%d, %d, %s, %d, %ld, %ld\n",
		tempo, elementPCB.ID, elementPCB.parentID, prioridade, elementPCB._ProcessoSimulado->VariavelManipulada, elementPCB.totalElapsedTime);

	
	return 0;
}