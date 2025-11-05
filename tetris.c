#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5

// Struct que representa uma peça do Tetris Stack
typedef struct {
    char nome; // tipo da peça ('I', 'O', 'T', 'L')
    int id;    // identificador único
} Peca;

// Struct da fila circular de peças
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int qtd;
    int prox_id;
} Fila;

// PROTÓTIPOS DAS FUNÇÕES
Peca gerarPeca(Fila *f);
void inicializarFila(Fila *f);
int inserirPeca(Fila *f);
int jogarPeca(Fila *f);
void exibirFila(Fila *f);


// Função que gera uma nova peça aleatória e atribui um id único
Peca gerarPeca(Fila *f) {
    char tipos[4] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = f->prox_id++;
    return nova;
}

// Inicializa a fila com peças geradas automaticamente
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
    f->prox_id = 0;
    for(int i = 0; i < MAX_FILA; i++) {
        f->itens[f->fim] = gerarPeca(f);
        f->fim = (f->fim + 1) % MAX_FILA;
        f->qtd++;
    }
}

// Insere uma nova peça ao final da fila (enqueue)
int inserirPeca(Fila *f) {
    if(f->qtd == MAX_FILA){
        printf("Fila cheia! Não é possível inserir nova peça.\n");
        return 0;
    }
    f->itens[f->fim] = gerarPeca(f);
    f->fim = (f->fim + 1) % MAX_FILA;
    f->qtd++;
    return 1;
}

// Remove a peça da frente da fila (dequeue)
int jogarPeca(Fila *f) {
    if(f->qtd == 0){
        printf("Fila vazia! Nenhuma peça para jogar.\n");
        return 0;
    }
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->qtd--;
    return 1;
}

// Exibe o estado atual da fila de peças
void exibirFila(Fila *f) {
    printf("\nFila de peças:\n");
    int idx = f->inicio;
    for(int i = 0; i < f->qtd; i++){
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");
}

// Função principal com menu simples
int main() {
    Fila fila;
    int opcao;

    srand((unsigned)time(NULL)); // Inicializa o gerador de aleatórios
    inicializarFila(&fila);

    do {
        exibirFila(&fila);
        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha a opção: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                if(jogarPeca(&fila)) printf("Peça jogada com sucesso!\n");
                break;
            case 2:
                if(inserirPeca(&fila)) printf("Nova peça inserida na fila!\n");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while(opcao != 0);

    return 0;
}
