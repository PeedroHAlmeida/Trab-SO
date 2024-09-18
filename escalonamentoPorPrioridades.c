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

    if (filaVazia(fila))
    {
        fila->proximo = novoNo;
        fila->anterior = novoNo;
    }
    else
    {
        fila->anterior->prox = novoNo;
        fila->anterior = novoNo;
    }
}

Processo *removeFila(){

}


void main()
{

    srand(time(NULL));

    int quantum;
    printf("Digite o valor do quantum do round robin: ");
    scanf("%d", &quantum);

    Fila prioridade4;
    inciaFila(&prioridade4);
    Fila prioridade3;
    inciaFila(&prioridade3);
    Fila prioridade2;
    inciaFila(&prioridade2);
    Fila prioridade1;
    inciaFila(&prioridade1);

    int numProcesso = 1;
    int condicao = 1;
    while (condicao)
    {
        if (1)//(rand() % 100) + 1 < 15)
        {
            Processo novoProcesso = {(rand() % 4) + 1, numProcesso, (rand() % 15) + 1};
            switch (novoProcesso.prioridade)
            {
            case 4:
                adicionaFila(&prioridade4, novoProcesso);
                imprimirFila(&prioridade4);
                Processo *exec = removeFila();
                if (exec == NULL){
                    printf("Fila de prioridade 4 vazia");

                }else if(exec->tamPrcss <= 0){
                    printf("Processo finalizado");
                    free(exec);
                    
                }else{
                    printf("Processo não finalizado e adicionado ao final da fila");
                    adicionaFila(&prioridade4, *exec);
                }
                sleep(1);
                break;
            case 3:
                adicionaFila(&prioridade3, novoProcesso);
                imprimirFila(&prioridade3);
                sleep(1);
                break;
            case 2:
                adicionaFila(&prioridade2, novoProcesso);
                imprimirFila(&prioridade2);
                sleep(1);
                break;
            case 1:
                adicionaFila(&prioridade1, novoProcesso);
                printf("prioridade 1");
                imprimirFila(&prioridade1);
                sleep(1);
                break;
            default:
                condicao = 0;
                printf("Erro na prioridade do processo\n");
                break;
            }
            
            numProcesso++;

        }
        
        numProcesso++;
        
    }
    

    


}
