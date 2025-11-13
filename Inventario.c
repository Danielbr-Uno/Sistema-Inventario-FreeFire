/*
  torre_resgate.c
  Módulo avançado de priorização e montagem da torre de resgate.

  Funcionalidades:
  - Cadastro de até 20 componentes (nome, tipo, prioridade)
  - Ordenação:
      * Bubble Sort por nome (strings)
      * Insertion Sort por tipo (strings)
      * Selection Sort por prioridade (inteiros)
  - Medição de comparações e tempo de execução (clock())
  - Busca binária por nome (aplicável apenas após ordenação por nome)
  - Interface em menu para o jogador escolher estratégias
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define STR_NOME 30
#define STR_TIPO 20

/* Estrutura do componente */
typedef struct {
    char nome[STR_NOME];
    char tipo[STR_TIPO];
    int prioridade; // 1..10
} Componente;

/* Prototipos das funções de ordenação (cada uma preenche comparacoes) */
void bubbleSortPorNome(Componente arr[], int n, long *comparacoes);
void insertionSortPorTipo(Componente arr[], int n, long *comparacoes);
void selectionSortPorPrioridade(Componente arr[], int n, long *comparacoes);

/* Busca binária por nome (após ordenação por nome) */
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long *comparacoes);

/* Utilitários */
void mostrarComponentes(Componente arr[], int n);
void limparString(char *s);
double medirTempo(void (*algoritmo)(Componente[], int, long*),
                  Componente arr[], int n, long *comparacoes);

/* Funções de manipulação de entrada */
void cadastrarComponentes(Componente arr[], int *n);
void copiarVetor(Componente dest[], Componente src[], int n);

int main() {
    Componente componentes[MAX_COMPONENTES];
    int num = 0;
    int opcao;
    int ordenadoPorNome = 0; // flag para permitir busca binária
    char chave[STR_NOME];

    printf("=== Módulo: Montagem da Torre de Resgate ===\n");
    printf("Objetivo: escolha uma estratégia de ordenação para priorizar componentes\n\n");

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1 - Cadastrar componentes (até %d)\n", MAX_COMPONENTES);
        printf("2 - Listar componentes cadastrados\n");
        printf("3 - Ordenar com Bubble Sort (por NOME) e medir desempenho\n");
        printf("4 - Ordenar com Insertion Sort (por TIPO) e medir desempenho\n");
        printf("5 - Ordenar com Selection Sort (por PRIORIDADE) e medir desempenho\n");
        printf("6 - Busca binária por NOME (apenas após ordenar por NOME)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida.\n");
            // limpar stdin
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }
        getchar(); // consumir '\n'

        switch (opcao) {
            case 1:
                cadastrarComponentes(componentes, &num);
                ordenadoPorNome = 0;
                break;
            case 2:
                mostrarComponentes(componentes, num);
                break;
            case 3: {
                if (num == 0) { printf("Nenhum componente cadastrado.\n"); break; }
                long comparacoes = 0;
                // medir tempo usando função genérica
                double tempo = medirTempo(bubbleSortPorNome, componentes, num, &comparacoes);
                printf("\nResultado: Bubble Sort por NOME concluído.\n");
                printf("Comparações: %ld | Tempo (s): %.6f\n", comparacoes, tempo);
                mostrarComponentes(componentes, num);
                ordenadoPorNome = 1;
                break;
            }
            case 4: {
                if (num == 0) { printf("Nenhum componente cadastrado.\n"); break; }
                long comparacoes = 0;
                double tempo = medirTempo(insertionSortPorTipo, componentes, num, &comparacoes);
                printf("\nResultado: Insertion Sort por TIPO concluído.\n");
                printf("Comparações: %ld | Tempo (s): %.6f\n", comparacoes, tempo);
                mostrarComponentes(componentes, num);
                ordenadoPorNome = 0; // ordenação por tipo quebra ordem por nome
                break;
            }
            case 5: {
                if (num == 0) { printf("Nenhum componente cadastrado.\n"); break; }
                long comparacoes = 0;
                double tempo = medirTempo(selectionSortPorPrioridade, componentes, num, &comparacoes);
                printf("\nResultado: Selection Sort por PRIORIDADE concluído.\n");
                printf("Comparações: %ld | Tempo (s): %.6f\n", comparacoes, tempo);
                mostrarComponentes(componentes, num);
                ordenadoPorNome = 0;
                break;
            }
            case 6: {
                if (!ordenadoPorNome) {
                    printf("A busca binária exige que os componentes estejam ordenados por NOME.\n");
                    printf("Ordene primeiro com a opção 3 (Bubble Sort por NOME).\n");
                    break;
                }
                if (num == 0) { printf("Nenhum componente cadastrado.\n"); break; }
                printf("Informe o NOME do componente-chave para busca: ");
                if (!fgets(chave, sizeof(chave), stdin)) break;
                limparString(chave);
                long comparacoes = 0;
                int pos = buscaBinariaPorNome(componentes, num, chave, &comparacoes);
                if (pos != -1) {
                    printf("Componente-chave encontrado!\n");
                    printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                           componentes[pos].nome, componentes[pos].tipo, componentes[pos].prioridade);
                } else {
                    printf("Componente-chave NAO encontrado.\n");
                }
                printf("Comparações realizadas (busca binária): %ld\n", comparacoes);
                break;
            }
            case 0:
                printf("Encerrando módulo. Boa sorte na montagem da torre!\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    return 0;
}

/* ----------------------------
   Funções de ordenação
   Cada função realiza a ordenação in-place.
   O parâmetro 'comparacoes' é atualizado com o número de comparações feitas.
   ----------------------------*/

/* Bubble Sort por nome (string) */
void bubbleSortPorNome(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    if (n <= 1) return;
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                Componente tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break; // otimização: array já ordenado
    }
}

/* Insertion Sort por tipo (string) */
void insertionSortPorTipo(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = arr[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j+1] = chave;
    }
}

/* Selection Sort por prioridade (int) 
   Ordena em ordem crescente de prioridade (1 -> mais urgente)
*/
void selectionSortPorPrioridade(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[minIdx].prioridade) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Componente tmp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = tmp;
        }
    }
}

/* ----------------------------
   Busca binária por nome (após ordenação por nome)
   Retorna índice ou -1 se não encontrado.
   Conta comparações no ponteiro comparacoes.
   ----------------------------*/
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long *comparacoes) {
    int inicio = 0, fim = n - 1;
    *comparacoes = 0;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        (*comparacoes)++;
        int cmp = strcmp(arr[meio].nome, chave);
        if (cmp == 0) return meio;
        if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

/* ----------------------------
   Utilitários
   ----------------------------*/

/* Mostrar componentes formatados */
void mostrarComponentes(Componente arr[], int n) {
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    printf("\n--- Componentes (%d) ---\n", n);
    printf("%-3s | %-28s | %-12s | %s\n", "#", "NOME", "TIPO", "PRIORIDADE");
    printf("----+------------------------------+--------------+-----------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-28s | %-12s | %d\n",
               i+1, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
}

/* Limpa \n final de strings lidas com fgets */
void limparString(char *s) {
    size_t L = strlen(s);
    if (L > 0 && s[L-1] == '\n') s[L-1] = '\0';
}

/* Mede o tempo de execução de um algoritmo de ordenação que segue
   a assinatura: void alg(Componente[], int, long*)
   Retorna tempo em segundos e atualiza comparacoes.
*/
double medirTempo(void (*algoritmo)(Componente[], int, long*),
                  Componente arr[], int n, long *comparacoes) {
    clock_t t0 = clock();
    algoritmo(arr, n, comparacoes);
    clock_t t1 = clock();
    return (double)(t1 - t0) / (double)CLOCKS_PER_SEC;
}

/* ----------------------------
   Entrada de componentes
   ----------------------------*/
void cadastrarComponentes(Componente arr[], int *n) {
    printf("Cadastro de componentes (até %d). Digite 'fim' no nome para parar.\n", MAX_COMPONENTES);
    while (*n < MAX_COMPONENTES) {
        Componente c;
        printf("\nComponente #%d\n", *n + 1);
        printf("Nome: ");
        if (!fgets(c.nome, sizeof(c.nome), stdin)) break;
        limparString(c.nome);
        if (strcmp(c.nome, "fim") == 0 || strcmp(c.nome, "FIM") == 0) {
            break;
        }
        if (strlen(c.nome) == 0) {
            printf("Nome vazio. Digite novamente.\n");
            continue;
        }
        printf("Tipo (ex: controle, suporte, propulsao): ");
        if (!fgets(c.tipo, sizeof(c.tipo), stdin)) break;
        limparString(c.tipo);
        if (strlen(c.tipo) == 0) {
            printf("Tipo vazio. Digite novamente.\n");
            continue;
        }
        printf("Prioridade (1 = mais urgente, 10 = menos urgente): ");
        if (scanf("%d", &c.prioridade) != 1) {
            printf("Entrada inválida para prioridade. Tente novamente.\n");
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }
        if (c.prioridade < 1) c.prioridade = 1;
        if (c.prioridade > 10) c.prioridade = 10;
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {} // limpa buffer

        // Armazena
        arr[*n] = c;
        (*n)++;
        printf("Componente '%s' cadastrado com sucesso.\n", c.nome);

        if (*n >= MAX_COMPONENTES) {
            printf("Capacidade máxima atingida.\n");
            break;
        }
        printf("Deseja continuar cadastrando? (enter para continuar, 'fim' no próximo nome para parar)\n");
    }
}

/* ----------------------------
   Função auxiliar: copiar vetor (não usada, mas útil para comparação sem alterar original)
   ----------------------------*/
void copiarVetor(Componente dest[], Componente src[], int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}
