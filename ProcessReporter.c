#include <stdio.h>
#include <string.h>
#include "ProcessManager.h"
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
	PCB elementoPCB;
	int variavelManipulada;
	read(STDIN_FILENO, &elementoPCB.ID, sizeof(int));
	read(STDIN_FILENO, &elementoPCB.parentID, sizeof(int));
	read(STDIN_FILENO, &elementoPCB.ProgramCounter, sizeof(int));
	read(STDIN_FILENO, &elementoPCB.startTime, sizeof(unsigned long int));
	read(STDIN_FILENO, &elementoPCB.totalElapsedTime, sizeof(unsigned long int));
	read(STDIN_FILENO, &elementoPCB.priority, sizeof(short int));
	read(STDIN_FILENO, &elementoPCB.status, sizeof(short int));
	read(STDIN_FILENO, &variavelManipulada, sizeof(int));
	close(STDIN_FILENO);
	char prioridade[50];

	if (elementoPCB.priority == LOW_PRIORITY) {
		sprintf(prioridade, "%s", "Prioridade baixa");
	}
	else if (elementoPCB.priority == NORMAL_PRIORITY) {
		sprintf(prioridade, "%s", "Prioridade normal");
	}
	else if (elementoPCB.priority == HIGH_PRIORITY) {
		sprintf(prioridade, "%s", "Prioridade alta");
	}
	printf("****************************************************************\n"
		"Estado do sistema:\n"
		"*******************************************************************\n"
		"Tempo Atual: %ld\n"
		"PROCESSO EXECUTANDO:\n"
		"%d, %d, %s, %d, %ld, %ld\n",
		tempo, elementoPCB.ID, elementoPCB.parentID, prioridade, variavelManipulada, elementoPCB.startTime, elementoPCB.totalElapsedTime);

	
	return 0;
}