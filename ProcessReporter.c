#include <stdio.h>
#include <string.h>
#include "ProcessManager.h"
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

void ClearScreen() {
	system("clear");
}

void printRemindReporter() {
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
	printf("%d, %d, %s, %d, %ld, %ld\n", elementoPCB.ID, elementoPCB.parentID, prioridade, variavelManipulada, elementoPCB.startTime, elementoPCB.totalElapsedTime);
}

int main() {
	sleep(1);
	ClearScreen();
	unsigned long int tempo;
	read(STDIN_FILENO, &tempo, sizeof(unsigned long int));
	printf("****************************************************************\n"
		   "                       Estado do sistema:                       \n"
		   "****************************************************************\n"
		   "Tempo Atual: %ld\n", tempo);
	printRemindReporter();
	int i;	
	
	
	int totalElementosBlokedLowPriority;
	read(STDIN_FILENO, &totalElementosBlokedLowPriority, sizeof(int));
	printf("BLOQUEADOS:\n");
	if (totalElementosBlokedLowPriority >= 0) {
		for (i = 0; i <= totalElementosBlokedLowPriority; i++) {
			printRemindReporter();
		}
	}

	int totalElementosBlokedNormalPriority;
	read(STDIN_FILENO, &totalElementosBlokedNormalPriority, sizeof(int));
	if (totalElementosBlokedNormalPriority >= 0) {
		for (i = 0; i <= totalElementosBlokedNormalPriority; i++) {
			printRemindReporter();
		}
	}

	int totalElementosBlokedHighPriority;
	read(STDIN_FILENO, &totalElementosBlokedHighPriority, sizeof(int));
	if (totalElementosBlokedHighPriority >= 0) {
		for (i = 0; i <= totalElementosBlokedHighPriority; i++) {
			printRemindReporter();
		}
	}

	int totalElementosReadyLowPriority;
	read(STDIN_FILENO, &totalElementosReadyLowPriority, sizeof(int));
	printf("PRONTOS:\n");
	if (totalElementosReadyLowPriority >= 0) {
		for (i = 0; i <= totalElementosReadyLowPriority; i++) {
			printRemindReporter();
		}
	}

	int totalElementosReadyNormalPriority = 0;
	//read(STDIN_FILENO, &totalElementosReadyNormalPriority, sizeof(int));
	printf("Reporter elementos ready normal: %d", totalElementosReadyNormalPriority);
	if (totalElementosReadyNormalPriority >= 0) {
		printf("Entrei no if");
		for (i = 0; i <= totalElementosReadyNormalPriority; i++) {
			printf("Executei o for");
			printRemindReporter();
		}
	}

	int totalElementosReadyHighPriority;
	read(STDIN_FILENO, &totalElementosReadyHighPriority, sizeof(int));
	if (totalElementosReadyHighPriority >= 0) {
		for (i = 0; i <= totalElementosReadyHighPriority; i++) {
			printRemindReporter();
		}
	}

	printf("****************************************************************\n");
	printf("\nPressione qualque tecla para continuar\n");
	close(STDIN_FILENO);
	return 0;
}