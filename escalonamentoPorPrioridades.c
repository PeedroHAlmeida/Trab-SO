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

void adicionaFila(Fila *fila, Processo novoProcesso){

    No *novoNo = (No*)malloc(sizeof(No));
    
    switch (novoProcesso.prioridade)
    {
    case 1:
        if (fila->anterior == NULL){
            novoNo->processo = novoProcesso;
            novoNo->prox = NULL;
        }
        
        break;
    
    default:
        break;
    }

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
    while (1)
    {
        if ((rand() % 100) + 1 < 15)
        {
            Processo novoProcesso = {(rand() % 4) + 1, numProcesso, (rand() % 20) + 1};
            switch (novoProcesso.prioridade)
            {
            case 1:
                adicionaFila(&prioridade1, novoProcesso);
                break;
            case 2:
                adicionaFila(&prioridade2, novoProcesso);
                break;
            case 3:
                adicionaFila(&prioridade3, novoProcesso);
                break;
            case 4:
                adicionaFila(&prioridade4, novoProcesso);
                break;
            default:
                printf("Erro na prioridade do processo");
                break;
            }
            
            numProcesso++;

        }
        
        
        {
            /* code */
        }
        
        
    }
    

    


}
