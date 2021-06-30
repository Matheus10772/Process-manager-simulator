#include "ProcessManager.h"
#define N 6
#define Max_Inst 50

//Cria um novo processo simulado, setando seus valores e alocando memoria
PCB * processo(char *programa){
    PCB* pcb = (processoSimulado*) malloc(sizeof(processoSimulado));
    processoSimulado * process;
    process -> VariavelManipulada = 0;
    process -> cpu = 0;
    pcb -> prioridade = valor_prioridade(pcb ->  instrucao);

    return pcb;
}

//Seta o valor da variavel para instrução do tipo S, atualizando o valor para N
void atualizar_valor(PCB * aux){
    (aux->process) -> VariavelManipulada = N;   
}

// Instrução do tipo A, realiza a soma da variavel inteira mais N (Variavel  + N)
void adicionar_valor(PCB * aux){
    (aux->process) -> VariavelManipulada = ((aux->process) -> VariavelManipulada) + N;
}
// Instrução do tipo D, subtrai N da variavel inteira (Variavel - N)
void subtrair_valor(PCB * aux){
    (aux->process) -> VariavelManipulada = ((aux->process) -> VariavelManipulada) - N;
}


//Função de prioridade de instruções, nele as instruções do processo são percorridas enquanto não se acha 
//a prioridade E, pois a mesma encerra o processo simulado, e caso ache a prioridade C que bloqueia o processo
// acrescente 1 na variavel de prioridade; 
int  valor_prioridade(tipoSimulado ** simulado){
    int v_prioridade = 0;
    char c_prioridade; 
    int contador;

    while((c_prioridade = simulado[i++] -> tipo) != 'E'){
        if(c_prioridade == 'C'){
            v_prioridade++;
        }
    }
    return v_prioridade;
}

//
tipoSimulado** instrucoes(char *nome){
    char arq[] = "instrucoes";
    int contador = 0;
    char tipo, valor[Max_Inst];
    strcat(arq,nome);
    tipoSimulado** instrucao;
    instrucao = (tipoSimulado**) malloc(Max_Inst* sizeof(tipoSimulado*));

    // Alocando espaço para os possiveis valores das intruções do tipoSimulado
    for(int i = 0; i < instrucao; i++){
        instrucao[i] = (tipoSimulado*) malloc(sizeof(tipoSimulado));
        instrucao[i] -> tipo = (char) malloc(Max_Inst * sizeof(char));
    }

    //Abertura e verificação de erro de um arquivo
    FILE * arquivo = fopen(arq, "rt");
    if(arquivo == NULL){
        printf("ERRO AO ABRIR O ARQUIVO\n");
        abort();
    }
    while(!feof(arquivo)){
        
        strcpy(valor, ""); 
        
        fscanf(arquivo, "%c ", &tipo);         
        if(!(tipo == 'B' || tipo == 'E')){
            fscanf(arquivo, "%s ", valor);     
        }
        // save into array
        instruction[i]->tipo = type;
        strcpy(instruction[i]->valor, value);
        
	    contador++;
	}
	
	fclose(arquivo);
    
    return instrucao;
}
}

