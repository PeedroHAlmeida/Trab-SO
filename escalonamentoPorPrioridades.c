/////////////////////////////////////////////////////////
//                                                     //
//               Sistemas Operacionais                 //      
//           Escalonamento por prioridades             //
//                                                     //
//  Feito por..: Pedro Henrique de Almeida             //
//  Matricula..: 2022.1.08.045                         //
//  Professor..: Fellipe Guilherme Rey de Souza        //
//  Data.......: 13 de outubro de 2024                 //
//                                                     // 
/////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // Para a função sleep

typedef struct processo_{
    int prioridade;
    int numero;
    int tamanho;
    struct processo_* prox;
} Processo;

typedef struct no_
{
    Processo processo;
    struct no_ *prox;
} No;

typedef struct fila_
{
    No *proximo;
    No *anterior;
} Fila;

void iniciaFila(Fila *fila){
    fila->proximo = NULL;
    fila->anterior = NULL;
}

int filaVazia(Fila *fila)
{
    return (fila->proximo == NULL);
}

void adicionaFila(Fila *fila, Processo novoProcesso) {
    No *novoNo = (No *)malloc(sizeof(No));  // Aloca memória para um novo nó
    
    novoNo->processo = novoProcesso;
    novoNo->prox = NULL;

    if (filaVazia(fila)) {
        // Fila está vazia, então o novo nó será o primeiro e o último
        fila->proximo = novoNo;   // Início da fila
        fila->anterior = novoNo;  // Fim da fila
    } else {
        // Adiciona o novo nó ao final da fila
        fila->anterior->prox = novoNo;
        fila->anterior = novoNo;  // Atualiza o fim da fila
    }
}

Processo *removeDaFila(Fila *fila)
{
    if (filaVazia(fila))
    {
        return NULL;  // Retorna NULL se a fila estiver vazia
    }

    No *temp = fila->proximo;
    Processo *processoRemovido = (Processo *)malloc(sizeof(Processo));  // Aloca memória para o processo removido
    *processoRemovido = temp->processo;  // Copia o conteúdo do processo

    fila->proximo = fila->proximo->prox;
    free(temp);  // Libera o nó removido

    if (fila->proximo == NULL)
    {
        fila->anterior = NULL;  // A fila está vazia após a remoção
    }

    return processoRemovido;  // Retorna o ponteiro para o processo removido
}

void executa (Processo *exec, int quantum){
    exec->tamanho = exec->tamanho - quantum;
}

int main()
{

    srand(time(NULL));

    int quantum;
    printf("Digite o valor do quantum do round robin: ");
    scanf("%d", &quantum);

    Fila prioridade4;
    iniciaFila(&prioridade4);
    Fila prioridade3;
    iniciaFila(&prioridade3);
    Fila prioridade2;
    iniciaFila(&prioridade2);
    Fila prioridade1;
    iniciaFila(&prioridade1);

    Processo *exec;
    int numProcesso = 0;
    int numPrioridadeExec = 4;
    int condicao = 1;
    int temporizador = 0;

    while (condicao)
    {
        sleep(1);
        if ((rand() % 100) + 1 < 15)
        {
            temporizador++;
            numProcesso++;
            Processo novoProcesso = {(rand() % 4) + 1, numProcesso, (rand() % 15) + 1, NULL};
            printf("Novo processo, prioridade: %d, mumProcesso: %d, tamanho: %d, NULL\n", novoProcesso.prioridade, novoProcesso.numero, novoProcesso.tamanho, novoProcesso.prox);
            if(novoProcesso.prioridade == 4){
                adicionaFila(&prioridade4, novoProcesso);
            }else if(novoProcesso.prioridade == 3){
                adicionaFila(&prioridade3, novoProcesso);
            }else if(novoProcesso.prioridade == 2){
                adicionaFila(&prioridade2, novoProcesso);
            }else{
                adicionaFila(&prioridade1, novoProcesso);
            }

        }else{
            temporizador++;
        }

        switch (numPrioridadeExec)
            {
            case 4:
            exec = removeDaFila(&prioridade4);
            if (exec == NULL) {
                printf("Fila de prioridade 4 vazia\n");
                if (filaVazia(&prioridade3)) {
                    printf("Fila de prioridade 3 vazia\n");
                    if (filaVazia(&prioridade2)) {
                        printf("Fila de prioridade 2 vazia\n");
                        if (filaVazia(&prioridade1)) {
                            printf("Fila de prioridade 1 vazia. Nenhum processo para ser executado\n");
                        } else {
                        exec = removeDaFila(&prioridade1);
                        if(exec->tamanho <= quantum){
                            printf("Processo %d executado e finalizado\n", exec->numero);
                            free(exec);    
                        }else{
                            exec->tamanho = exec->tamanho - quantum;
                            adicionaFila(&prioridade1, *exec);
                            printf("Tempo gasto ate agora: %d | Processo %d de tamanho: %d em execucao\n", temporizador, exec->numero, exec->tamanho);
                        }
                        }
                    } else {
                        // Reutilizando exec
                        exec = removeDaFila(&prioridade2);
                        executa(exec, quantum);  // Passa o ponteiro exec corretamente
                        numPrioridadeExec = 2;
                        printf("Processo %d executado\n", exec->numero);
                    }
                } else {
                    // Reutilizando exec
                    exec = removeDaFila(&prioridade3);
                    executa(exec, quantum);  // Passa o ponteiro exec corretamente
                    numPrioridadeExec = 3;
                    printf("Processo %d executado\n", exec->numero);
                }
            } else if (exec->tamanho <= quantum) {
                printf("Processo %d executado e finalizado\n", exec->numero);
                free(exec);  // Liberar memória após a execução final
            } else {
                executa(exec, quantum);  // Chama a função executa corretamente
                printf("Tempo gasto ate agora: %d | Processo %d de tamanho: %d em execucao\n", temporizador, exec->numero, exec->tamanho);
                adicionaFila(&prioridade4, *exec);  // Re-adiciona o ponteiro exec na fila
            }
            break;

            case 3:
                if(!filaVazia(&prioridade4) && exec->tamanho <= quantum){
                    numPrioridadeExec = 4;
                    exec = removeDaFila(&prioridade4);
                    printf("Processo %d executado e finalizado\n", exec->numero);
                    free(exec);  // Liberar memória após a execução final
                } else if(!filaVazia(&prioridade4) && exec->tamanho > quantum){ 
                    numPrioridadeExec = 4;         
                    executa(exec, quantum);  // Chama a função executa corretamente
                    printf("Tempo gasto ate agora: %d | Processo %d de tamanho: %d em execucao\n", temporizador, exec->numero, exec->tamanho);
                    adicionaFila(&prioridade4, *exec);  // Re-adiciona o ponteiro exec na fila
                }else{
                    exec = removeDaFila(&prioridade3);
                    if (exec == NULL){
                        printf("Fila de prioridade 3 vazia\n");
                        numPrioridadeExec = 2;
                        if(filaVazia(&prioridade2)){
                            
                        }
                    }else if(exec->tamanho <= quantum){
                        printf("Processo %d executado e finalizado\n", exec->numero);
                        free(exec);
                        
                    }else{
                        executa(exec, quantum);
                        adicionaFila(&prioridade3, *exec);
                        printf("Tempo gasto ate agora: %d | Processo %d de tamanho: %d em execucao\n", temporizador, exec->numero, exec->tamanho);
                    }
                }
                break;
                
                
            case 2:
                exec = removeDaFila(&prioridade2);
                if (exec == NULL){
                    printf("Fila de prioridade 2 vazia\n");
                    numPrioridadeExec = 1;
                }else if(exec->tamanho <= quantum){
                    printf("Processo %d executado e finalizado\n", exec->numero);
                    free(exec);
                    
                }else{
                    exec->tamanho = exec->tamanho - quantum;
                    adicionaFila(&prioridade2, *exec);
                    printf("Tempo gasto ate agora: %d | Processo %d de tamanho: %d em execucao\n", temporizador, exec->numero, exec->tamanho);
                }
                sleep(1);
                break;
            case 1:
                exec = removeDaFila(&prioridade1);
                if (exec == NULL){
                    printf("Fila de prioridade 1 vazia\n");
                    numPrioridadeExec = 4;  // Volta para a prioridade mais alta
                }else if(exec->tamanho <= quantum){
                    printf("Processo %d executado e finalizado\n", exec->numero);
                    free(exec);
                    
                }else{
                    exec->tamanho = exec->tamanho - quantum;
                    adicionaFila(&prioridade1, *exec);
                    printf("Tempo gasto ate agora: %d | Processo %d de tamanho: %d em execucao\n", temporizador, exec->numero, exec->tamanho);
                }
                break;
            default:
                condicao = 0;
                printf("Erro na prioridade do processo\n");
                break;
            }
        
        
    }

    return 0;
}
