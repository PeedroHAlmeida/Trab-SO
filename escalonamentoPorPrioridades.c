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

void executarProcesso(Processo *exec, int *numPrioridadeExec, int quantum, Fila *prioridade, int temporizador) {
    if (exec->tamanho <= quantum) {
        printf("Processo %d executado e finalizado\n\n", exec->numero);
        free(exec);  // Liberar memória após a execução final
    } else {
        printf("Tempo gasto até agora: %d | Processo %d de prioridade %d com tamanho %d em execução\n", temporizador, exec->numero, exec->prioridade, exec->tamanho);
        executa(exec, quantum);  // Chama a função executa corretamente
        if (exec->prioridade == *numPrioridadeExec){
        adicionaFila(prioridade, *exec);  // Passa o ponteiro corretamente
        } //preciso do cafe agora
        printf("Processo %d adicionado novamente à fila de prioridade %d\n\n", exec->numero, *numPrioridadeExec);
    }
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
        temporizador++;
        if ((rand() % 100) + 1 < 99) {
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

        if (!filaVazia(&prioridade4)) {
            exec = removeDaFila(&prioridade4);
            numPrioridadeExec = 4;
            executarProcesso(exec, &numPrioridadeExec, quantum, &prioridade4, temporizador);
        }else if (!filaVazia(&prioridade3)) {
            exec = removeDaFila(&prioridade3);
            numPrioridadeExec = 3;
            executarProcesso(exec, &numPrioridadeExec, quantum, &prioridade3, temporizador);
        }else if (!filaVazia(&prioridade2)) {
            numPrioridadeExec = 2;
            exec = removeDaFila(&prioridade2);
            executarProcesso(exec, &numPrioridadeExec, quantum, &prioridade2, temporizador); 
        }else {
            numPrioridadeExec = 1;
            exec = removeDaFila(&prioridade1);
            executarProcesso(exec, &numPrioridadeExec, quantum, &prioridade1, temporizador); 
        }

        if (temporizador % 10 == 0){
            sobePrioridade(&prioridade4, &prioridade3, &prioridade2, &prioridade1);
        }
    }
    return 0;
}