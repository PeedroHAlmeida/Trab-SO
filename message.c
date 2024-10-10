/////////////////////////////////////////////////////////
//                                                     //
//               Sistemas Operacionais                 //
//           Escalonamento por prioridades             //
//                                                     //
//  Feito por..: Pedro Henrique de Almeida             //
//  Matricula..: 2022.1.08.045                         //
//  Feito por..: Jorran Luka Andrade dos Santos        //
//  Matricula..: 2022.2.08.001                         //
//  Professor..: Fellipe Guilherme Rey de Souza        //
//  Data.......: 13 de outubro de 2024                 //
//                                                     //
/////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // Para a função sleep

typedef struct processo_ {
  int prioridade;
  int id;
  int tamanho;
  int ciclo;
  struct processo_ *prox;
} Processo;

typedef struct no_ {
  Processo processo;
  struct no_ *prox;
} No;

typedef struct fila_ {
  No *proximo;
  No *anterior;
  int tamanho;
} Fila;

void iniciaFila(Fila *fila) {
  fila->proximo = NULL;
  fila->anterior = NULL;
}

int filaVazia(Fila *fila) { return (fila->proximo == NULL); }

void adicionaFila(Fila *fila, Processo novoProcesso) {
  No *novoNo = (No *)malloc(sizeof(No)); // Aloca memória para um novo nó
  novoNo->processo = novoProcesso;
  novoNo->prox = NULL;

  if (filaVazia(fila)) {
    fila->proximo = novoNo;  // Início da fila
    fila->anterior = novoNo; // Fim da fila
  } else {
    fila->anterior->prox = novoNo;
    fila->anterior = novoNo; // Atualiza o fim da fila
  }
  fila->tamanho++;
}

Processo *removeDaFila(Fila *fila) {
  if (filaVazia(fila)) {
    return NULL; // Retorna NULL se a fila estiver vazia
  }

  No *temp = fila->proximo;
  Processo *processoRemovido = (Processo *)malloc(sizeof(Processo)); // Aloca memória para o processo removido
  *processoRemovido = temp->processo; // Copia o conteúdo do processo

  fila->proximo = fila->proximo->prox;
  free(temp); // Libera o nó removido

  if (fila->proximo == NULL) {
    fila->anterior = NULL; // A fila está vazia após a remoção
  }
  fila->tamanho--;
  return processoRemovido; // Retorna o ponteiro para o processo removido
}

No *removerNo(Fila *fila, No *no) {
  if (fila == NULL || no == NULL)
    return NULL;

  // Caso o nó a ser removido seja o primeiro na fila
  if (fila->proximo == no) {
    fila->proximo = no->prox;
  } else {
    // Caso o nó esteja no meio ou no final, itera para encontrar o nó anterior
    No *anterior = fila->proximo;
    while (anterior != NULL && anterior->prox != no) {
      anterior = anterior->prox;
    }
    // Atualiza os ponteiros para remover o nó
    if (anterior != NULL) {
      anterior->prox = no->prox;
    }
  }

  if (fila->anterior == no) {
    fila->anterior = NULL;
  }

  fila->tamanho--;
  return no;
}

void incrementaCiclos(Fila **list) {
  for (int i = 0; i < 4; i++) {  // Itera pelas filas no vetor
    No *atual = list[i]->proximo;  // Aponta para o primeiro nó da fila

    while (atual != NULL) {  // Percorre a lista de nós até o final
      atual->processo.ciclo += 1;  // Incrementa o campo 'ciclo' do processo

      // Armazena o próximo nó antes de potencialmente alterar a lista
      No *proximoNo = atual->prox;

      // Verifica se a prioridade deve ser aumentada
      if (atual->processo.ciclo > 3 && atual->processo.prioridade < 4) {
        int prioridadeAnterior = atual->processo.prioridade;

        // Aumenta a prioridade e move o nó para a fila correta
        atual->processo.prioridade++;
        atual->processo.ciclo = 0; // Reseta o ciclo após mudar de prioridade

        printf("Processo %d mudou de prioridade %d para %d.\n", atual->processo.id, prioridadeAnterior, atual->processo.prioridade);

        // Remove o nó da fila atual e insere na fila de maior prioridade
        No *removido = removerNo(list[i], atual);
        if (removido != NULL) {
          adicionaFila(list[removido->processo.prioridade - 1], removido->processo);
          free(removido); // Libera o nó temporário
        }
      }

      // Avança para o próximo nó
      atual = proximoNo;
    }
  }
}

void executa(Processo *exec, int quantum) { exec->tamanho -= quantum; }

void executarProcesso(Processo *exec, int quantum, Fila **vetor, int posicao) {
  if (exec->tamanho <= quantum) {
    printf("Processo %d executado e finalizado.\n\n", exec->id);
    printf("-------------------------------------------------------\n\n");
    // Processo finalizado, libera a memória do processo
    free(exec);
  } else {
    printf("Processo %d de prioridade %d de tamanho %d em execução.\n",  exec->id, exec->prioridade, exec->tamanho);
    executa(exec, quantum); // Executa o processo (reduz o tamanho)
    printf("Processo %d recebe tamanho = %d.\n\n", exec->id, exec->tamanho);
    printf("-------------------------------------------------------\n\n");
    adicionaFila(vetor[posicao], *exec); // Reinsere o processo na fila
  }
  incrementaCiclos(vetor); // Incrementa ciclos dos processos não executados
}

void imprimirFila(Fila *fila) {
  if (filaVazia(fila)) {
    printf("Fila vazia.\n");
    return;
  }

  No *atual = fila->proximo;
  while (atual != NULL) {
    printf("(ID %d, Tamanho %d) -> ", atual->processo.id, atual->processo.tamanho);
    atual = atual->prox;
  }
  printf("NULL\n");
}

int main() {
  srand(time(NULL));

  int quantum;

  printf("Digite o valor do quantum do round robin: ");
  scanf("%d", &quantum); 
  // Verificação se o quantum tem um valor valido
  while (quantum < 1) {
    printf("ERRO! Digite um valor inteiro positivo para o valor do quantum do round robin: ");
    scanf("%d", &quantum);
  }

  Fila prioridade4, prioridade3, prioridade2, prioridade1;
  iniciaFila(&prioridade4);
  iniciaFila(&prioridade3);
  iniciaFila(&prioridade2);
  iniciaFila(&prioridade1);

  Processo *exec;
  int numProcesso = 0;
  int temporizador = 0;
  Fila *vetor[4] = {&prioridade1, &prioridade2, &prioridade3, &prioridade4};

  while (1) {
    sleep(1);   // Pausa para visualização melhor das saidas 
    temporizador++;   // Incremento no tempo gasto
    if ((rand() % 100) + 1 < 30) {  // Criação de novos processos - 30% de chance
      numProcesso++; // ID do processo criado
      Processo novoProcesso = {(rand() % 4) + 1, numProcesso, (rand() % (quantum * 5)) + 1, 0, NULL};   // sorteio da fila de prioridade e sorteio do temanho do processo
      // o tamanho do processo estara em função do tamanho que o usuario define de quantum
      printf("Novo processo criado: ID %d; Prioridade %d; Tamanho %d.\n\n", novoProcesso.id, novoProcesso.prioridade, novoProcesso.tamanho);  // exibe no terminal o novo processo criado
      printf("-------------------------------------------------------\n\n");
      adicionaFila(vetor[novoProcesso.prioridade - 1], novoProcesso); // Adiciona na fila de prioridade sorteada
    }

    // Imprime o estado das filas a cada instante de tempo
    printf("--- Estado Atual das Filas ---\n");
    printf("Prioridade 4: ");
    imprimirFila(&prioridade4);
    printf("Prioridade 3: ");
    imprimirFila(&prioridade3);
    printf("Prioridade 2: ");
    imprimirFila(&prioridade2);
    printf("Prioridade 1: ");
    imprimirFila(&prioridade1);
    printf("-------------------------------------------------------\n\n");


    // Executa processos com base na prioridade
    for (int i = 3; i >= 0; i--) {
      if (!filaVazia(vetor[i])) {
        exec = removeDaFila(vetor[i]);
        executarProcesso(exec, quantum, vetor, i);
        break;
      }
    }
  }

  return 0;
}
