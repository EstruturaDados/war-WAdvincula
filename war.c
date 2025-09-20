
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ===============================
// Estrutura de dados
// ===============================
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ===============================
// Protótipos das funções
// ===============================
Territorio* alocarMapa(int qtd);
void cadastrarTerritorios(Territorio* mapa, int qtd);
void exibirMapa(const Territorio* mapa, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

// ===============================
// Função principal
// ===============================
int main() {
    srand(time(NULL)); // inicializa gerador de números aleatórios

    int qtd;
    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &qtd);
    getchar(); // limpa o buffer

    // Alocação dinâmica
    Territorio* mapa = alocarMapa(qtd);
    if (!mapa) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Cadastro inicial
    cadastrarTerritorios(mapa, qtd);

    int opcao;
    do {
        printf("\n===== MENU =====\n");
        printf("1 - Exibir mapa\n");
        printf("2 - Atacar\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // limpa buffer

        if (opcao == 1) {
            exibirMapa(mapa, qtd);
        } else if (opcao == 2) {
            exibirMapa(mapa, qtd);

            int idxAtacante, idxDefensor;
            printf("Escolha o índice do território atacante: ");
            scanf("%d", &idxAtacante);
            getchar();
            printf("Escolha o índice do território defensor: ");
            scanf("%d", &idxDefensor);
            getchar();

            if (idxAtacante < 0 || idxAtacante >= qtd ||
                idxDefensor < 0 || idxDefensor >= qtd) {
                printf("Índices inválidos!\n");
            } else if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
                printf("Não é possível atacar um território da mesma cor!\n");
            } else {
                atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
                exibirMapa(mapa, qtd);
            }
        }
    } while (opcao != 0);

    liberarMemoria(mapa);
    return 0;
}

// ===============================
// Implementação das funções
// ===============================

// Aloca memória dinamicamente para os territórios
Territorio* alocarMapa(int qtd) {
    return (Territorio*)calloc(qtd, sizeof(Territorio));
}

// Cadastro manual dos territórios
void cadastrarTerritorios(Territorio* mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\n=== Cadastro do Território %d ===\n", i);
        printf("Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // remover \n

        printf("Cor do exército: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar();
    }
}

// Exibe todos os territórios cadastrados
void exibirMapa(const Territorio* mapa, int qtd) {
    printf("\n=== MAPA ATUAL ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula ataque entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante->tropas <= 1) {
        printf("O atacante precisa ter mais de 1 tropa para atacar!\n");
        return;
    }

    int dadoAtq = rand() % 6 + 1;
    int dadoDef = rand() % 6 + 1;

    printf("\n%s (Atacante) rolou %d | %s (Defensor) rolou %d\n",
           atacante->nome, dadoAtq, defensor->nome, dadoDef);

    if (dadoAtq > dadoDef) {
        printf("Atacante venceu a batalha!\n");

        // Metade das tropas do atacante vão para o defensor
        int tropasMovidas = atacante->tropas / 2;
        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas;

        // O território muda de dono
        strcpy(defensor->cor, atacante->cor);
        printf("%s foi conquistado!\n", defensor->nome);
    } else {
        printf("Defensor resistiu! O atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
    }
}

// Libera memória alocada dinamicamente
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}
