#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10   // Capacidade máxima da mochila

// ----------------------
// Estrutura de Dados
// ----------------------
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// ----------------------
// Declaração de Funções
// ----------------------
void inserirItem(Item mochila[], int *numItens);
void removerItem(Item mochila[], int *numItens);
void listarItens(Item mochila[], int numItens);
void buscarItem(Item mochila[], int numItens);
int buscarPosicao(Item mochila[], int numItens, char nome[]);

// ----------------------
// Função Principal
// ----------------------
int main() {
    Item mochila[MAX_ITENS];
    int numItens = 0;
    int opcao;

    printf("=== SISTEMA DE INVENTÁRIO - MOCHILA DE LOOT ===\n");

    do {
        printf("\n-------------------------\n");
        printf("1. Cadastrar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item\n");
        printf("0. Sair\n");
        printf("-------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {
            case 1:
                inserirItem(mochila, &numItens);
                break;
            case 2:
                removerItem(mochila, &numItens);
                break;
            case 3:
                listarItens(mochila, numItens);
                break;
            case 4:
                buscarItem(mochila, numItens);
                break;
            case 0:
                printf("\nSaindo do sistema... Até a próxima missão!\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ----------------------
// Função: inserirItem
// Permite cadastrar um novo item na mochila
// ----------------------
void inserirItem(Item mochila[], int *numItens) {
    if (*numItens >= MAX_ITENS) {
        printf("\nA mochila está cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novo;
    printf("\n--- Cadastro de Item ---\n");
    printf("Nome do item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // Remove o \n do final

    printf("Tipo (arma, munição, cura, ferramenta, etc): ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar();

    mochila[*numItens] = novo;
    (*numItens)++;

    printf("\nItem '%s' adicionado com sucesso!\n", novo.nome);
    listarItens(mochila, *numItens);
}

// ----------------------
// Função: removerItem
// Remove um item com base no nome informado
// ----------------------
void removerItem(Item mochila[], int *numItens) {
    if (*numItens == 0) {
        printf("\nA mochila está vazia. Nenhum item para remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nInforme o nome do item que deseja remover: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int posicao = buscarPosicao(mochila, *numItens, nomeBusca);

    if (posicao == -1) {
        printf("\nItem '%s' não encontrado na mochila.\n", nomeBusca);
    } else {
        for (int i = posicao; i < *numItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        (*numItens)--;
        printf("\nItem '%s' removido com sucesso!\n", nomeBusca);
        listarItens(mochila, *numItens);
    }
}

// ----------------------
// Função: listarItens
// Exibe todos os itens cadastrados
// ----------------------
void listarItens(Item mochila[], int numItens) {
    printf("\n=== Itens na Mochila ===\n");
    if (numItens == 0) {
        printf("Nenhum item cadastrado.\n");
        return;
    }

    for (int i = 0; i < numItens; i++) {
        printf("[%d] Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// ----------------------
// Função: buscarItem
// Busca um item pelo nome (busca sequencial)
// ----------------------
void buscarItem(Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("\nA mochila está vazia.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nInforme o nome do item que deseja buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int posicao = buscarPosicao(mochila, numItens, nomeBusca);

    if (posicao == -1) {
        printf("\nItem '%s' não encontrado.\n", nomeBusca);
    } else {
        printf("\nItem encontrado!\n");
        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
               mochila[posicao].nome, mochila[posicao].tipo, mochila[posicao].quantidade);
    }
}

// ----------------------
// Função: buscarPosicao
// Realiza a busca sequencial pelo nome e retorna o índice
// ----------------------
int buscarPosicao(Item mochila[], int numItens, char nome[]) {
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}
