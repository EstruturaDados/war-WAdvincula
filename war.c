#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// =========================
// Constantes Globais
// =========================
#define NUM_TERRITORIOS 5
#define NUM_MISSOES 2
#define MAX_NOME 30

// =========================
// Estruturas de Dados
// =========================
typedef struct {
    char nome[MAX_NOME];
    char cor[10];    // dono do território
    int tropas;
} Territorio;

// =========================
// Protótipos
// =========================
Territorio* alocarMapa(void);
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);

void exibirMenuPrincipal(void);
void exibirMapa(const Territorio* mapa);
void exibirMissao(int idMissao);

void faseDeAtaque(Territorio* mapa, const char* corJogador);
void simularAtaque(Territorio* origem, Territorio* destino);

int sortearMissao(void);
int verificarVitoria(const Territorio* mapa, const char* corJogador, int idMissao);

void limparBufferEntrada(void);

// =========================
// Função Principal
// =========================
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    // Configuração inicial
    Territorio* mapa = alocarMapa();
    if (!mapa) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    inicializarTerritorios(mapa);

    char corJogador[] = "Azul";
    int idMissao = sortearMissao();

    int opcao, venceu = 0;
    do {
        exibirMapa(mapa);
        exibirMissao(idMissao);
        exibirMenuPrincipal();

        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;
            case 2:
                venceu = verificarVitoria(mapa, corJogador, idMissao);
                if (venceu)
                    printf("Parabéns! Você cumpriu sua missão!\n");
                else
                    printf("Ainda não cumpriu sua missão.\n");
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0 && !venceu);

    liberarMemoria(mapa);
    return 0;
}

// =========================
// Implementação das Funções
// =========================

Territorio* alocarMapa(void) {
    return (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

void inicializarTerritorios(Territorio* mapa) {
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].cor, "Vermelho");
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].cor, "Azul");
    mapa[1].tropas = 4;

    strcpy(mapa[2].nome, "Chile");
    strcpy(mapa[2].cor, "Verde");
    mapa[2].tropas = 3;

    strcpy(mapa[3].nome, "Uruguai");
    strcpy(mapa[3].cor, "Amarelo");
    mapa[3].tropas = 2;

    strcpy(mapa[4].nome, "Peru");
    strcpy(mapa[4].cor, "Vermelho");
    mapa[4].tropas = 6;
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

void exibirMenuPrincipal(void) {
    printf("\n=== MENU ===\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar vitória\n");
    printf("0 - Sair\n");
}

void exibirMapa(const Territorio* mapa) {
    printf("\n=== MAPA ===\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%s | Dono: %s | Tropas: %d\n",
               mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void exibirMissao(int idMissao) {
    printf("\n=== Sua Missão ===\n");
    if (idMissao == 0)
        printf("Conquistar 2 territórios vermelhos.\n");
    else if (idMissao == 1)
        printf("Conquistar qualquer 3 territórios.\n");
}

void faseDeAtaque(Territorio* mapa, const char* corJogador) {
    char origem[MAX_NOME], destino[MAX_NOME];
    printf("Digite território de origem: ");
    fgets(origem, MAX_NOME, stdin);
    origem[strcspn(origem, "\n")] = '\0';

    printf("Digite território de destino: ");
    fgets(destino, MAX_NOME, stdin);
    destino[strcspn(destino, "\n")] = '\0';

    Territorio* tOrigem = NULL;
    Territorio* tDestino = NULL;

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].nome, origem) == 0)
            tOrigem = &mapa[i];
        if (strcmp(mapa[i].nome, destino) == 0)
            tDestino = &mapa[i];
    }

    if (tOrigem && tDestino && strcmp(tOrigem->cor, corJogador) == 0) {
        simularAtaque(tOrigem, tDestino);
    } else {
        printf("Territórios inválidos ou origem não pertence ao jogador!\n");
    }
}

void simularAtaque(Territorio* origem, Territorio* destino) {
    if (origem->tropas <= 1) {
        printf("Você não tem tropas suficientes para atacar!\n");
        return;
    }

    int dadoAtq = rand() % 6 + 1;
    int dadoDef = rand() % 6 + 1;

    printf("Ataque (%s) rolou %d | Defesa (%s) rolou %d\n",
           origem->nome, dadoAtq, destino->nome, dadoDef);

    if (dadoAtq > dadoDef) {
        destino->tropas--;
        printf("Defensor perdeu 1 tropa!\n");
        if (destino->tropas <= 0) {
            strcpy(destino->cor, origem->cor);
            destino->tropas = 1;
            origem->tropas--;
            printf("%s foi conquistado!\n", destino->nome);
        }
    } else {
        origem->tropas--;
        printf("Atacante perdeu 1 tropa!\n");
    }
}

int sortearMissao(void) {
    return rand() % NUM_MISSOES;
}

int verificarVitoria(const Territorio* mapa, const char* corJogador, int idMissao) {
    int count = 0;

    if (idMissao == 0) { // Conquistar 2 vermelhos
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 &&
                strstr(mapa[i].nome, "Vermelho") == NULL &&
                strcmp(mapa[i].cor, "Vermelho") == 0) {
                count++;
            }
        }
        return count >= 2;
    } else if (idMissao == 1) { // Conquistar 3 territórios
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0)
                count++;
        }
        return count >= 3;
    }

    return 0;
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
