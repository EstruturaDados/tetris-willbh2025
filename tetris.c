#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Struct que representa uma peça
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único da peça
} Peca;

// Struct da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int qtd;
    int prox_id;
} Fila;

// Struct da pilha linear
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// PROTOTIPOS
Peca gerarPeca(Fila *f);
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int reservarPeca(Fila *f, Pilha *p);
int jogarPeca(Fila *f);
int usarReserva(Pilha *p);
int empilhar(Pilha *p, Peca peca);
int desempilhar(Pilha *p, Peca *peca);
int inserirPeca(Fila *f, Peca novaPeca);
void exibirFila(Fila *f);
void exibirPilha(Pilha *p);

// Função que gera uma nova peça aleatória
Peca gerarPeca(Fila *f) {
    char tipos[4] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = f->prox_id++;
    return nova;
}

// Inicializa a fila circular de peças
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
    f->prox_id = 0;
    for(int i = 0; i < MAX_FILA; i++) {
        Peca peca = gerarPeca(f);
        inserirPeca(f, peca);
    }
}

// Inicializa a pilha de reserva
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Insere uma peça na fila circular
int inserirPeca(Fila *f, Peca novaPeca) {
    if(f->qtd == MAX_FILA) return 0; // Fila cheia
    f->itens[f->fim] = novaPeca;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->qtd++;
    return 1;
}

// Remove peça da frente da fila (dequeue): retorna sucesso ou falha
int jogarPeca(Fila *f) {
    if(f->qtd == 0) return 0; // Fila vazia
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->qtd--;
    return 1;
}

// Move peça da frente da fila para o topo da pilha
int reservarPeca(Fila *f, Pilha *p) {
    if(f->qtd == 0) {
        printf("Fila vazia! Não há peça para reservar.\n");
        return 0;
    }
    if(p->topo == MAX_PILHA - 1) {
        printf("Pilha cheia! Não é possível reservar mais peças.\n");
        return 0;
    }
    Peca peca = f->itens[f->inicio];
    jogarPeca(f);
    empilhar(p, peca);
    return 1;
}

// Remove e utiliza peça do topo da pilha
int usarReserva(Pilha *p) {
    if(p->topo == -1) {
        printf("Pilha vazia! Nenhuma peça reservada para usar.\n");
        return 0;
    }
    Peca peca;
    desempilhar(p, &peca);
    printf("Peça [%c %d] utilizada da reserva!\n", peca.nome, peca.id);
    return 1;
}

// Empilha peça na pilha linear
int empilhar(Pilha *p, Peca peca) {
    if(p->topo == MAX_PILHA - 1) return 0;
    p->topo++;
    p->itens[p->topo] = peca;
    return 1;
}

// Desempilha peça do topo da pilha linear
int desempilhar(Pilha *p, Peca *peca) {
    if(p->topo == -1) return 0;
    *peca = p->itens[p->topo];
    p->topo--;
    return 1;
}

// Exibe o estado atual da fila de peças
void exibirFila(Fila *f) {
    printf("\nFila de peças:\n");
    int idx = f->inicio;
    for(int i = 0; i < f->qtd; i++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");
}

// Exibe o estado atual da pilha de reserva
void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    if(p->topo == -1) {
        printf("[Vazia]");
    } else {
        for(int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n");
}

// Função principal, menu de opções e fluxo do programa
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;

    srand((unsigned)time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    do {
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: // Jogar peça (dequeue)
                if(jogarPeca(&fila)) {
                    printf("Peça jogada!\n");
                } else {
                    printf("Não há peça para jogar!\n");
                }
                // Sempre mantêm a fila cheia:
                inserirPeca(&fila, gerarPeca(&fila));
                break;
            case 2: // Reservar peça (move para pilha)
                if(reservarPeca(&fila, &pilha)) {
                    printf("Peça reservada!\n");
                }
                inserirPeca(&fila, gerarPeca(&fila));
                break;
            case 3: // Usar peça da reserva
                usarReserva(&pilha);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while(opcao != 0);

    return 0;
}
