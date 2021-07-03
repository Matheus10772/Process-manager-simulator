#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

const int PIPE_FALIED_VALUE = -1;
const int FORK_FALIED_VALUE = -1;
pid_t childpid = -1;

int* criarpipe() {
    int* pipefd = (int*)malloc(sizeof(int) * 2);
    int* statusvalue = (int*)malloc(sizeof(int) * 1);
    statusvalue[0] = pipe(pipefd);
    if (*(statusvalue) == PIPE_FALIED_VALUE){ //a chamada pipe retorna '-1' se a criação do pipe não for bem sucedida
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

int* inicializarCommander() {
    int* pipefd;
    int escolha;
    while (*(pipefd = criarpipe()) == PIPE_FALIED_VALUE)
    {
        printf("Deseja tentar criar o pipe novamente? Digite 1 para sim e 0 para sair");
        scanf("%d", &escolha);
        if (escolha == 0)
            exit(1);
    }
    while ((childpid = criarFork()) == FORK_FALIED_VALUE) {
        printf("Deseja tentar criar o fork novamente? Digite 1 para sim e 0 para sair");
        scanf("%d", &escolha);
        if (escolha == 0)
            exit(1);
    }
    printf("\nFork feito\n");
    printf("\nChild pid: %d\n", childpid);
    if (childpid == 0) {
        printf("\nProcessando filho\n");
        close(pipefd[1]);
        redirecionarEntrada(pipefd, STDIN_FILENO, 0);
        execlp("./ProcessManager", "./ProcessManager", NULL); //Substitui o código do processo filho pelo código desejado
    }
    else {
        return pipefd;
    }
}

int main() {
    char command = '\0';
    int* pipefd;

    if (childpid != 0) {
        pipefd = inicializarCommander();
        close(pipefd[0]);
    }
    while (command != 'T')
    {
        printf("\nDigite o comando desejado:");
        printf("\nQ: Para colocar fim em uma unidade de tempo."
               "\nU: Para desbloquear o primeiro processo da fila."
               "\nP: Para imprimir o estado atual do sistema."
               "\nT: Imprimi o tempo de retorno médio e, em seguida, finaliza o simulador\n"
        );
        scanf("%c", &command);
        command = (char)toupper(command);
        write(pipefd[1], &command, 1);
    }
    return 0;
}