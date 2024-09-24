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

typedef struct processo_ {
    int prioridade;
    int numero;
    int tamanho;
    struct processo_ *prox;
} Processo;

typedef struct no_ {
    Processo processo;
    struct no_ *prox;
} No;

typedef struct fila_ {
    No *proximo;
    No *anterior;
} Fila;

void iniciaFila(Fila *fila) {
    fila->proximo = NULL;
    fila->anterior = NULL;
}

int filaVazia(Fila *fila) {
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

Processo *removeDaFila(Fila *fila) {
    if (filaVazia(fila)) {
        return NULL;  // Retorna NULL se a fila estiver vazia
    }

    No *temp = fila->proximo;
    Processo *processoRemovido = (Processo *)malloc(sizeof(Processo));  // Aloca memória para o processo removido
    *processoRemovido = temp->processo;  // Copia o conteúdo do processo

    fila->proximo = fila->proximo->prox;
    free(temp);  // Libera o nó removido

    if (fila->proximo == NULL) {
        fila->anterior = NULL;  // A fila está vazia após a remoção
    }

    return processoRemovido;  // Retorna o ponteiro para o processo removido
}

void executa(Processo *exec, int quantum) {
    exec->tamanho -= quantum;
}

void imprimirFila(Fila *fila)
{
    if (filaVazia(fila))
    {
        printf("A fila vazia.\n");
        return;
    }

    No *atual = fila->proximo;
    while (atual != NULL)
    {
        printf("(%d):%d -> ", atual->processo.numero, atual->processo.tamanho);
        atual = atual->prox;
    }
    printf("NULL\n");
}

int main() {
    srand(time(NULL));

    int quantum;
    printf("Digite o valor do quantum do round robin: ");
    scanf("%d", &quantum);

    Fila prioridade4, prioridade3, prioridade2, prioridade1;
    iniciaFila(&prioridade4);
    iniciaFila(&prioridade3);
    iniciaFila(&prioridade2);
    iniciaFila(&prioridade1);

    Processo *exec;
    int numProcesso = 0;
    int numPrioridadeExec = 4;
    int temporizador = 0;

    while (1) {
        sleep(1);
        if ((rand() % 100) + 1 < 99) {
            temporizador++;
            numProcesso++;
            Processo novoProcesso = {(rand() % 4) + 1, numProcesso, (rand() % 15) + 1, NULL};
            printf("Novo processo, prioridade: %d, numProcesso: %d, tamanho: %d, prox: %p\n", novoProcesso.prioridade, novoProcesso.numero, novoProcesso.tamanho, (void *)novoProcesso.prox);
            
            if (novoProcesso.prioridade == 4) {
                adicionaFila(&prioridade4, novoProcesso);
            } else if (novoProcesso.prioridade == 3) {
                adicionaFila(&prioridade3, novoProcesso);
            } else if (novoProcesso.prioridade == 2) {
                adicionaFila(&prioridade2, novoProcesso);
            } else {
                adicionaFila(&prioridade1, novoProcesso);
            }
        } else {
            temporizador++;
        }

        printf("Prioridade 4: ");
        imprimirFila(&prioridade4);
        printf("Prioridade 3: ");
        imprimirFila(&prioridade3);
        printf("Prioridade 2: ");
        imprimirFila(&prioridade2);
        printf("Prioridade 1: ");
        imprimirFila(&prioridade1);
        printf("\n\n");

        switch (numPrioridadeExec) {
            case 4:
                exec = removeDaFila(&prioridade4);
                if (exec == NULL) {
                    printf("Fila de prioridade 4 vazia\n");
                    if (filaVazia(&prioridade3)) {
                        printf("Fila de prioridade 3 vazia\n");
                        if (filaVazia(&prioridade2)) {
                            printf("Fila de prioridade 2 vazia\n");
                            if (filaVazia(&prioridade1)) {
                                printf("Fila de prioridade 1 vazia. Nenhum processo para ser executado\n\n");
                            } else {
                                exec = removeDaFila(&prioridade1);
                                numPrioridadeExec = 1;
                                if (exec->tamanho <= quantum) {
                                    printf("Processo %d executado e finalizado\n\n", exec->numero);
                                    free(exec);
                                } else {
                                    executa(exec, quantum);  // Passa o ponteiro exec corretamente
                                    printf("Processo %d executado e adicionado a fila novamente\n\n", exec->numero);
                                    adicionaFila(&prioridade1, *exec);
                                }
                            }
                        } else {
                            exec = removeDaFila(&prioridade2);
                            numPrioridadeExec = 2;
                            if (exec->tamanho <= quantum) {
                                printf("Processo %d executado e finalizado\n\n", exec->numero);
                                free(exec);
                            } else {
                                executa(exec, quantum);  // Passa o ponteiro exec corretamente
                                printf("Processo %d executado e adicionado a fila novamente\n\n", exec->numero);
                                adicionaFila(&prioridade2, *exec);

                            }
                        }
                    } else {
                        exec = removeDaFila(&prioridade3);
                        numPrioridadeExec = 3;
                        if (exec->tamanho <= quantum) {
                            printf("Processo %d executado e finalizado\n\n", exec->numero);
                            free(exec);
                        } else {
                            executa(exec, quantum);  // Passa o ponteiro exec corretamente
                            printf("Processo %d executado e acionado a fila novamente\n\n", exec->numero);
                            adicionaFila(&prioridade3, *exec);

                        }
                    }
                } else if (exec->tamanho <= quantum) {
                    printf("Processo %d executado e finalizado\n\n", exec->numero);
                    free(exec);  // Liberar memória após a execução final
                } else {
                    executa(exec, quantum);  // Chama a função executa corretamente
                    printf("Tempo gasto até agora: %d | Processo %d de prioridade %d com tamanho %d em execução\n\n", temporizador, exec->numero, exec->prioridade, exec->tamanho);
                    adicionaFila(&prioridade4, *exec);  // Re-adiciona o ponteiro exec na fila
                }
                break;

            case 3:
                if (!filaVazia(&prioridade4)) {
                    exec = removeDaFila(&prioridade4);
                    numPrioridadeExec = 4;
                    if (exec->tamanho <= quantum) {
                        printf("Processo %d executado e finalizado\n\n", exec->numero);
                        free(exec);
                    } else {
                        executa(exec, quantum);  // Chama a função executa corretamente
                        printf("Tempo gasto até agora: %d | Processo %d de prioridade %d com tamanho %d em execução\n\n", temporizador, exec->numero, exec->prioridade, exec->tamanho);
                        adicionaFila(&prioridade4, *exec);  // Re-adiciona o ponteiro exec na fila
                    }
                } else {
                    printf("Fila de prioridade 4 vazia\n");
                    exec = removeDaFila(&prioridade3);
                    if (exec == NULL) {
                        printf("Fila de prioridade 3 vazia\n");
                        if (filaVazia(&prioridade2)) {
                            printf("Fila de prioridade 2 vazia\n");
                            if (filaVazia(&prioridade1)) {
                                printf("Fila de prioridade 1 vazia. Nenhum processo para ser executado\n");
                            }else{
                                exec = removeDaFila(&prioridade1);
                                numPrioridadeExec = 1;
                                if (exec->tamanho <= quantum) {
                                    printf("Processo %d executado e finalizado\n\n", exec->numero);
                                    free(exec);
                                } else {
                                    executa(exec, quantum);  // Passa o ponteiro exec corretamente
                                    printf("Processo %d executado e adicionado a fila novamente\n\n", exec->numero);
                                    adicionaFila(&prioridade1, *exec);
                            }
                            }
                        } else {
                            exec = removeDaFila(&prioridade2);
                            numPrioridadeExec = 2;
                            if (exec->tamanho <= quantum) {
                                printf("Processo %d executado e finalizado\n\n", exec->numero);
                                free(exec);
                            } else {
                                executa(exec, quantum);  // Passa o ponteiro exec corretamente
                                printf("Processo %d executado e adicionado a fila novamente\n\n", exec->numero);
                                adicionaFila(&prioridade2, *exec);
                            }
                        }
                    } else if (exec->tamanho <= quantum) {
                        printf("Processo %d executado e finalizado\n\n", exec->numero);
                        free(exec);
                    } else {
                        executa(exec, quantum);
                        adicionaFila(&prioridade3, *exec);
                        printf("Tempo gasto até agora: %d | Processo %d de prioridade %d com tamanho %d em execução\n\n", temporizador, exec->numero, exec->prioridade, exec->tamanho);
                    }
                }
                break;

            case 2:
                if (!filaVazia(&prioridade4)) {
                    exec = removeDaFila(&prioridade4);
                    numPrioridadeExec = 4;
                    if (exec->tamanho <= quantum) {
                        printf("Processo %d executado e finalizado\n\n", exec->numero);
                        free(exec);
                    } else {
                        executa(exec, quantum);  // Chama a função executa corretamente
                        printf("Tempo gasto até agora: %d | Processo %d de prioridade %d com tamanho %d em execução\n\n", temporizador, exec->numero, exec->prioridade, exec->tamanho);
                        adicionaFila(&prioridade4, *exec);  // Re-adiciona o ponteiro exec na fila
                    }
                } else if (!filaVazia(&prioridade3)) {
                    printf("Fila de prioridade 4 vazia\n");
                    exec = removeDaFila(&prioridade3);
                    numPrioridadeExec = 3;
                    if (exec->tamanho <= quantum) {
                        printf("Processo %d executado e finalizado\n\n", exec->numero);
                        free(exec);
                    } else {
                        executa(exec, quantum);  // Chama a função executa corretamente
                        printf("Tempo gasto até agora: %d | Processo %d de prioridade %d com tamanho %d em execução\n\n", temporizador, exec->numero, exec->prioridade, exec->tamanho);
                        adicionaFila(&prioridade3, *exec);  // Re-adiciona o ponteiro exec na fila
                    }
                } else {
                    printf("Fila de prioridade 4 vazia\n");
                    printf("Fila de prioridade 3 vazia\n");
                    exec = removeDaFila(&prioridade2);
                    if (exec == NULL) {
                        printf("Fila de prioridade 2 vazia\n");
                        if (filaVazia(&prioridade1)) {
                            printf("Fila de prioridade 1 vazia\n\n");
                        } else {
                            exec = removeDaFila(&prioridade1);
                            numPrioridadeExec = 1;
                            if (exec->tamanho <= quantum) {
                                printf("Processo %d executado e finalizado\n\n", exec->numero);
                                free(exec);
                            } else {
                                executa(exec, quantum);  // Passa o ponteiro exec corretamente
                                printf("Processo %d executado e dicionado a fila novamente\n\n", exec->numero);
                                adicionaFila(&prioridade1, *exec);
                            }
                        }
                    } else if (exec->tamanho <= quantum) {
                        printf("Processo %d executado e finalizado\n\n", exec->numero);
                        free(exec);
                    } else {
                        executa(exec, quantum);
                        printf("Tempo gasto até agora: %d | Processo %d de prioridade %d com tamanho %d em execução\n\n", temporizador, exec->numero, exec->prioridade, exec->tamanho);
                        adicionaFila(&prioridade2, *exec);
                    }
                }
                break;
            case 1:
                if (!filaVazia(&prioridade4)) {
                    exec = removeDaFila(&prioridade4);
                    numPrioridadeExec = 4;
                    if (exec->tamanho <= quantum) {
                        printf("Processo %d executado e finalizado\n\n", exec->numero);
                        free(exec);
                    } else {
                        executa(exec, quantum);  // Chama a função executa corretamente
                        printf("Tempo gasto até agora: %d | Processo %d de prioridade %d com tamanho %d em execução\n\n", temporizador, exec->numero, exec->prioridade, exec->tamanho);
                        adicionaFila(&prioridade4, *exec);  // Re-adiciona o ponteiro exec na fila
                    }
                } else if (!filaVazia(&prioridade3)) {
                    printf("Fila de prioridade 4 vazia\n");
                    exec = removeDaFila(&prioridade3);
                    numPrioridadeExec = 3;
                    if (exec->tamanho <= quantum) {
                        printf("Processo %d executado e finalizado\n\n", exec->numero);
                        free(exec);
                    } else {
                        executa(exec, quantum);  // Chama a função executa corretamente
                        printf("Tempo gasto até agora: %d | Processo %d de prioridade %d com tamanho %d em execução\n\n", temporizador, exec->numero, exec->prioridade, exec->tamanho);
                        adicionaFila(&prioridade3, *exec);  // Re-adiciona o ponteiro exec na fila
                    }
                } else if (!filaVazia(&prioridade2)) {
                    printf("Fila de prioridade 4 vazia\n");
                    printf("Fila de prioridade 3 vazia\n");
                    exec = removeDaFila(&prioridade2);
                    numPrioridadeExec = 2;
                    if (exec->tamanho <= quantum) {
                        printf("Processo %d executado e finalizado\n\n", exec->numero);
                        free(exec);
                    } else {
                        executa(exec, quantum);  // Chama a função executa corretamente
                        printf("Tempo gasto até agora: %d | Processo %d de prioridade %d com tamanho %d em execução\n\n", temporizador, exec->numero, exec->prioridade, exec->tamanho);
                        adicionaFila(&prioridade2, *exec);  // Re-adiciona o ponteiro exec na fila
                    }
                } else {
                    printf("Fila de prioridade 4 vazia\n");
                    printf("Fila de prioridade 3 vazia\n");
                    printf("Fila de prioridade 2 vazia\n");
                    exec = removeDaFila(&prioridade1);
                    if (exec == NULL) {
                        printf("Fila de prioridade 1 vazia. Nenhum processo para ser executado.\n\n");
                    } else if (exec->tamanho <= quantum) {
                        printf("Processo %d executado e finalizado\n\n", exec->numero);
                        free(exec);
                    } else {
                        executa(exec, quantum);
                        printf("Tempo gasto até agora: %d | Processo %d de prioridade %d com tamanho %d em execução\n\n", temporizador, exec->numero, exec->prioridade, exec->tamanho);
                        adicionaFila(&prioridade2, *exec);
                    }
                }

                break;
            default:
                printf("Filas de prioridade vazias\n\n");
                break;
        }
    }

    return 0;
}