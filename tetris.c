#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Struct que representa uma peça do Tetris
typedef struct {
    char nome; // Tipo: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// Fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int qtd;
    int prox_id;
} Fila;

// Pilha linear
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// -------- PROTOTIPOS -----------
Peca gerarPeca(Fila *f);
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int inserirPecaFila(Fila *f, Peca p);
int jogarPecaFila(Fila *f);
int reservarPeca(Fila *f, Pilha *p);
int empilhar(Pilha *p, Peca peca);
int desempilhar(Pilha *p, Peca *peca);
int usarReserva(Pilha *p);
void exibirFila(Fila *f);
void exibirPilha(Pilha *p);
int trocarFrenteTopo(Fila *f, Pilha *p);
int trocarMultiplos(Fila *f, Pilha *p);

// ----------- FUNCOES -------
Peca gerarPeca(Fila *f) {
    char tipos[4] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = f->prox_id++;
    return nova;
}

// Inicializa a fila com MAX_FILA peças
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
    f->prox_id = 0;
    for(int i = 0; i < MAX_FILA; i++) {
        inserirPecaFila(f, gerarPeca(f));
    }
}

// Inicializa a pilha (vazia)
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Insere uma peça no fim da fila circular
int inserirPecaFila(Fila *f, Peca novaPeca) {
    if(f->qtd == MAX_FILA) return 0;
    f->itens[f->fim] = novaPeca;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->qtd++;
    return 1;
}

// Remove uma peça da frente da fila
int jogarPecaFila(Fila *f) {
    if(f->qtd == 0) return 0;
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->qtd--;
    return 1;
}

// Move a peça da frente da fila para o topo da pilha
int reservarPeca(Fila *f, Pilha *p) {
    if(f->qtd == 0) {
        printf("Fila vazia!\n");
        return 0;
    }
    if(p->topo == MAX_PILHA - 1) {
        printf("Pilha cheia!\n");
        return 0;
    }
    Peca peca = f->itens[f->inicio];
    jogarPecaFila(f);
    empilhar(p, peca);
    return 1;
}

// Insere uma peça no topo da pilha
int empilhar(Pilha *p, Peca peca) {
    if(p->topo == MAX_PILHA - 1) return 0;
    p->itens[++p->topo] = peca;
    return 1;
}

// Remove uma peça do topo da pilha
int desempilhar(Pilha *p, Peca *peca) {
    if(p->topo == -1) return 0;
    *peca = p->itens[p->topo--];
    return 1;
}

// Usa a peça do topo da pilha
int usarReserva(Pilha *p) {
    if(p->topo == -1) {
        printf("Pilha vazia!\n");
        return 0;
    }
    Peca peca;
    desempilhar(p, &peca);
    printf("Peça [%c %d] usada da reserva\n", peca.nome, peca.id);
    return 1;
}

// Troca peça da frente da fila com o topo da pilha
int trocarFrenteTopo(Fila *f, Pilha *p) {
    if(f->qtd == 0 || p->topo == -1) {
        printf("Impossível trocar: Fila ou pilha vazias\n");
        return 0;
    }
    int idx = f->inicio;
    Peca aux = f->itens[idx];
    f->itens[idx] = p->itens[p->topo];
    p->itens[p->topo] = aux;
    return 1;
}

// Troca os 3 primeiros da fila com as 3 peças da pilha
int trocarMultiplos(Fila *f, Pilha *p) {
    if(f->qtd < 3 || p->topo < 2) {
        printf("Impossível trocar: menos de 3 peças na fila ou pilha\n");
        return 0;
    }
    for(int i=0; i<3; i++) {
        int idx = (f->inicio + i) % MAX_FILA;
        Peca aux = f->itens[idx];
        f->itens[idx] = p->itens[p->topo - i];
        p->itens[p->topo - i] = aux;
    }
    return 1;
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    printf("Fila de peças\n");
    int idx = f->inicio;
    for(int i=0; i < f->qtd; i++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");
}

// Exibe o estado atual da pilha
void exibirPilha(Pilha *p) {
    printf("Pilha de reserva\n(Topo -> base): ");
    if(p->topo == -1) {
        printf("[Vazia]");
    } else {
        for(int i=p->topo; i>=0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n");
}


// ----------- FUNÇÃO PRINCIPAL -----------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    srand((unsigned)time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    do {
        printf("\n======= Estado atual ======\n");
        exibirFila(&fila);
        exibirPilha(&pilha);
        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("\nOpção escolhida: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                if(jogarPecaFila(&fila)) {
                    printf("Peça jogada!\n");
                    inserirPecaFila(&fila, gerarPeca(&fila));
                } else {
                    printf("Não há peça para jogar!\n");
                }
                break;
            case 2:
                if(reservarPeca(&fila, &pilha)) {
                    printf("Peça reservada!\n");
                    inserirPecaFila(&fila, gerarPeca(&fila));
                }
                break;
            case 3:
                usarReserva(&pilha);
                break;
            case 4:
                if(trocarFrenteTopo(&fila, &pilha)) {
                    printf("Troca realizada entre peça da frente e topo da pilha!\n");
                }
                break;
            case 5:
                if(trocarMultiplos(&fila, &pilha)) {
                    printf("Troca múltipla realizada!\n");
                }
                break;
            case 0:
                printf("Encerrando programa...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while(opcao != 0);

    return 0;
}
