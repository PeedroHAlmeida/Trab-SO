#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // para a função sleep()

// Definição das estruturas
typedef struct processo_{
    int prioridade;
    int tempo;
    int tamPrcss;
    struct processo_* prox;
}Processo;

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

// Funções de manipulação de filas
void inciaFila(Fila *fila){
    fila->proximo = NULL;
    fila->anterior = NULL;
}

int filaVazia(Fila *fila)
{
    return (fila->proximo == NULL);
}

void adicionaFila(Fila *fila, Processo novoProcesso){
    No *novoNo = (No*)malloc(sizeof(No));
    novoNo->processo = novoProcesso;
    novoNo->prox = NULL;

    if (filaVazia(fila)) {
        fila->proximo = novoNo;
        fila->anterior = novoNo;
    } else {
        fila->anterior->prox = novoNo;
        fila->anterior = novoNo;
    }
}

void removerProcesso(Fila *fila) {
    if (!filaVazia(fila)) {
        No *noRemovido = fila->proximo;
        fila->proximo = fila->proximo->prox;
        if (fila->proximo == NULL) {
            fila->anterior = NULL;
        }
        free(noRemovido);
    }
}

// Função de impressão da fila (para debug)
void imprimirFila(Fila *fila) {
    No *atual = fila->proximo;
    while (atual != NULL) {
        printf("Processo de prioridade %d, tempo restante: %d\n", atual->processo.prioridade, atual->processo.tamPrcss);
        atual = atual->prox;
    }
}

// Função de execução dos processos por prioridade com Round Robin
void executarProcessos(Fila *fila, int quantum) {
    No *atual = fila->proximo;
    while (atual != NULL) {
        printf("Executando processo com prioridade %d\n", atual->processo.prioridade);
        
        // Simula a execução do processo por um quantum
        if (atual->processo.tamPrcss > quantum) {
            atual->processo.tamPrcss -= quantum;
            printf("Processo não concluído, tempo restante: %d\n", atual->processo.tamPrcss);
            sleep(1);
        } else {
            printf("Processo concluído\n");
            removerProcesso(fila);
        }
        atual = atual->prox;
    }
}

// Função principal
int main() {
    srand(time(NULL));

    int quantum;
    printf("Digite o valor do quantum do round robin: ");
    scanf("%d", &quantum);

    // Inicializa filas por prioridade
    Fila prioridade4, prioridade3, prioridade2, prioridade1;
    inciaFila(&prioridade4);
    inciaFila(&prioridade3);
    inciaFila(&prioridade2);
    inciaFila(&prioridade1);

    int numProcesso = 1;
    int condicao = 1;
    while (condicao) {
        // Gera um processo aleatório e adiciona à fila de acordo com a prioridade
        Processo novoProcesso = {(rand() % 4) + 1, numProcesso, (rand() % 15) + 1};
        switch (novoProcesso.prioridade) {
            case 4:
                adicionaFila(&prioridade4, novoProcesso);
                break;
            case 3:
                adicionaFila(&prioridade3, novoProcesso);
                break;
            case 2:
                adicionaFila(&prioridade2, novoProcesso);
                break;
            case 1:
                adicionaFila(&prioridade1, novoProcesso);
                break;
            default:
                condicao = 0;
                printf("Erro na prioridade do processo\n");
                break;
        }
        numProcesso++;

        // Escalonador por prioridades: prioridade 1 é executada primeiro, e assim por diante
        if (!filaVazia(&prioridade1)) {
            executarProcessos(&prioridade1, quantum);
        } else if (!filaVazia(&prioridade2)) {
            executarProcessos(&prioridade2, quantum);
        } else if (!filaVazia(&prioridade3)) {
            executarProcessos(&prioridade3, quantum);
        } else if (!filaVazia(&prioridade4)) {
            executarProcessos(&prioridade4, quantum);
        }

        sleep(1); // Simula o tempo de espera entre iterações
    }

    return 0;
}
