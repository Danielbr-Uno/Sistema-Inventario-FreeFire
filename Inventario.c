#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10  // Capacidade máxima da mochila sequencial

// ----------------------
// Estruturas de Dados
// ----------------------
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// ----------------------
// Contadores de Comparações
// ----------------------
int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

// ----------------------
// Funções da Mochila (VETOR)
// ----------------------
void inserirItemVetor(Item mochila[], int *numItens);
void removerItemVetor(Item mochila[], int *numItens);
void listarItensVetor(Item mochila[], int numItens);
int buscarSequencialVetor(Item mochila[], int numItens, char nome[]);
void ordenarVetor(Item mochila[], int numItens);
int buscarBinariaVetor(Item mochila[], int numItens, char nome[]);

// ----------------------
// Funções da Mochila (LISTA ENCADEADA)
// ----------------------
No* inserirItemLista(No* inicio);
No* removerItemLista(No* inicio);
void listarItensLista(No* inicio);
No* buscarItemLista(No* inicio, char nome[]);

// ----------------------
// Funções Auxiliares
// ----------------------
void menuPrincipal();
void menuVetor(Item mochila[], int *numItens);
void menuLista(No** inicio);

// ----------------------
// Função Principal
// ----------------------
int main() {
    Item mochilaVetor[MAX_ITENS];
    int numItensVetor = 0;
    No* mochilaLista = NULL;

    int opcao;
    do {
        menuPrincipal();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                menuVetor(mochilaVetor, &numItensVetor);
                break;
            case 2:
                menuLista(&mochilaLista);
                break;
            case 0:
                printf("\nEncerrando o sistema... Até a próxima missão!\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ----------------------
// MENUS
// ----------------------
void menuPrincipal() {
    printf("\n==== MOCHILA DE LOOT ====\n");
    printf("1. Versão com Vetor (Lista Sequencial)\n");
    printf("2. Versão com Lista Encadeada\n");
    printf("0. Sair\n");
    printf("Escolha a estrutura de dados: ");
}

void menuVetor(Item mochila[], int *numItens) {
    int opcao;
    do {
        printf("\n--- MOCHILA (VETOR) ---\n");
        printf("1. Inserir Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Sequencial\n");
        printf("5. Ordenar e Buscar Binária\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: inserirItemVetor(mochila, numItens); break;
            case 2: removerItemVetor(mochila, numItens); break;
            case 3: listarItensVetor(mochila, *numItens); break;
            case 4: {
                char nome[30];
                printf("Nome do item para busca: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                comparacoesSequencial = 0;
                int pos = buscarSequencialVetor(mochila, *numItens, nome);
                if (pos != -1)
                    printf("Item encontrado: %s (%s) - Qtd: %d\n",
                           mochila[pos].nome, mochila[pos].tipo, mochila[pos].quantidade);
                else
                    printf("Item não encontrado.\n");
                printf("Comparações realizadas: %d\n", comparacoesSequencial);
                break;
            }
            case 5: {
                ordenarVetor(mochila, *numItens);
                printf("Mochila ordenada por nome.\n");
                listarItensVetor(mochila, *numItens);
                char nome[30];
                printf("Nome do item para busca binária: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                comparacoesBinaria = 0;
                int pos = buscarBinariaVetor(mochila, *numItens, nome);
                if (pos != -1)
                    printf("Item encontrado: %s (%s) - Qtd: %d\n",
                           mochila[pos].nome, mochila[pos].tipo, mochila[pos].quantidade);
                else
                    printf("Item não encontrado.\n");
                printf("Comparações realizadas (binária): %d\n", comparacoesBinaria);
                break;
            }
            case 0: break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

void menuLista(No** inicio) {
    int opcao;
    do {
        printf("\n--- MOCHILA (LISTA ENCADEADA) ---\n");
        printf("1. Inserir Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Sequencial\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: *inicio = inserirItemLista(*inicio); break;
            case 2: *inicio = removerItemLista(*inicio); break;
            case 3: listarItensLista(*inicio); break;
            case 4: {
                char nome[30];
                printf("Nome do item para busca: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                comparacoesSequencial = 0;
                No* resultado = buscarItemLista(*inicio, nome);
                if (resultado)
                    printf("Item encontrado: %s (%s) - Qtd: %d\n",
                           resultado->dados.nome, resultado->dados.tipo, resultado->dados.quantidade);
                else
                    printf("Item não encontrado.\n");
                printf("Comparações realizadas: %d\n", comparacoesSequencial);
                break;
            }
            case 0: break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

// ----------------------
// Funções Vetor
// ----------------------
void inserirItemVetor(Item mochila[], int *numItens) {
    if (*numItens >= MAX_ITENS) {
        printf("Mochila cheia!\n");
        return;
    }
    Item novo;
    printf("Nome: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';
    printf("Tipo: ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar();

    mochila[*numItens] = novo;
    (*numItens)++;
    printf("Item adicionado com sucesso!\n");
}

void removerItemVetor(Item mochila[], int *numItens) {
    if (*numItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }
    char nome[30];
    printf("Nome do item para remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';
    int pos = buscarSequencialVetor(mochila, *numItens, nome);
    if (pos == -1) {
        printf("Item não encontrado.\n");
        return;
    }
    for (int i = pos; i < *numItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    (*numItens)--;
    printf("Item removido com sucesso!\n");
}

void listarItensVetor(Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }
    printf("\n--- Itens na Mochila ---\n");
    for (int i = 0; i < numItens; i++) {
        printf("[%d] %s (%s) - Qtd: %d\n", i + 1,
               mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

int buscarSequencialVetor(Item mochila[], int numItens, char nome[]) {
    for (int i = 0; i < numItens; i++) {
        comparacoesSequencial++;
        if (strcmp(mochila[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

void ordenarVetor(Item mochila[], int numItens) {
    for (int i = 0; i < numItens - 1; i++) {
        for (int j = i + 1; j < numItens; j++) {
            if (strcmp(mochila[i].nome, mochila[j].nome) > 0) {
                Item temp = mochila[i];
                mochila[i] = mochila[j];
                mochila[j] = temp;
            }
        }
    }
}

int buscarBinariaVetor(Item mochila[], int numItens, char nome[]) {
    int inicio = 0, fim = numItens - 1;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        comparacoesBinaria++;
        int cmp = strcmp(mochila[meio].nome, nome);
        if (cmp == 0)
            return meio;
        else if (cmp < 0)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }
    return -1;
}

// ----------------------
// Funções Lista Encadeada
// ----------------------
No* inserirItemLista(No* inicio) {
    No* novo = (No*) malloc(sizeof(No));
    if (!novo) {
        printf("Erro de alocação!\n");
        return inicio;
    }
    printf("Nome: ");
    fgets(novo->dados.nome, sizeof(novo->dados.nome), stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = '\0';
    printf("Tipo: ");
    fgets(novo->dados.tipo, sizeof(novo->dados.tipo), stdin);
    novo->dados.tipo[strcspn(novo->dados.tipo, "\n")] = '\0';
    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);
    getchar();

    novo->proximo = inicio;
    printf("Item adicionado à lista!\n");
    return novo;
}

No* removerItemLista(No* inicio) {
    if (!inicio) {
        printf("Lista vazia.\n");
        return inicio;
    }
    char nome[30];
    printf("Nome do item para remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    No *atual = inicio, *anterior = NULL;
    while (atual && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (!atual) {
        printf("Item não encontrado.\n");
        return inicio;
    }

    if (anterior == NULL)
        inicio = atual->proximo;
    else
        anterior->proximo = atual->proximo;

    free(atual);
    printf("Item removido com sucesso!\n");
    return inicio;
}

void listarItensLista(No* inicio) {
    if (!inicio) {
        printf("Lista vazia.\n");
        return;
    }
    printf("\n--- Itens na Lista ---\n");
    No* atual = inicio;
    int i = 1;
    while (atual) {
        printf("[%d] %s (%s) - Qtd: %d\n", i++,
               atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
}

No* buscarItemLista(No* inicio, char nome[]) {
    No* atual = inicio;
    while (atual) {
        comparacoesSequencial++;
        if (strcmp(atual->dados.nome, nome) == 0)
            return atual;
        atual = atual->proximo;
    }
    return NULL;
}
