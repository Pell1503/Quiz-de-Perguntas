#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int pontuacao = 0;
int perguntaAtual = 0;
int bonusSequencia = 0;
int valorBase = 2;

typedef struct {
    char pergunta[100];
    char opA[50];
    char opB[50];
    char opC[50];
    char correta; // 'a', 'b', 'c'
} Questao;

Questao quizFacil[] = {
    {"Qual a capital do Brasil?", "A) São Paulo", "B) Brasília", "C) Rio de Janeiro", 'b'},
    {"2 + 2 é igual a:", "A) 3", "B) 4", "C) 5", 'b'},
    {"Cor do céu num dia limpo?", "A) Azul", "B) Verde", "C) Vermelho", 'a'},
    {"Qual é o contrário de alto?", "A) Grande", "B) Curto", "C) Baixo", 'c'},
    {"Quantas pernas tem um cachorro?", "A) 2", "B) 4", "C) 6", 'b'},
    {"A cor do semáforo que significa 'pare' é:", "A) Verde", "B) Vermelho", "C) Amarelo", 'b'}
};

Questao quizMedio[] = {
    {"Maior planeta do sistema solar?", "A) Marte", "B) Júpiter", "C) Terra", 'b'},
    {"Qual linguagem estamos usando aqui?", "A) Python", "B) Java", "C) C", 'c'},
    {"Quantos continentes existem?", "A) 5", "B) 6", "C) 7", 'c'},
    {"Qual é o nome do satélite natural da Terra?", "A) Lua", "B) Estrela", "C) Sol", 'a'},
    {"Quantos segundos tem um minuto?", "A) 60", "B) 100", "C) 90", 'a'},
    {"Qual animal é conhecido por sua memória?", "A) Elefante", "B) Gato", "C) Tartaruga", 'a'}
};

Questao quizDificil[] = {
    {"Quem descobriu a gravidade?", "A) Einstein", "B) Newton", "C) Galileu", 'b'},
    {"Velocidade da luz em km/s?", "A) 300.000", "B) 150.000", "C) 1.000.000", 'a'},
    {"Qual a fórmula da água?", "A) CO2", "B) H2O", "C) O2", 'b'},
    {"Qual o ano da Independência do Brasil?", "A) 1822", "B) 1889", "C) 1500", 'a'},
    {"Quantos elementos tem a tabela periódica atualmente?", "A) 118", "B) 112", "C) 108", 'a'},
    {"Quem pintou a Mona Lisa?", "A) Picasso", "B) Da Vinci", "C) Van Gogh", 'b'}
};

Questao* quiz = NULL;
int totalPerguntas = 0;

void embaralharIndices(int indices[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        indices[i] = i;
    }
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
}

void printMenu() {
    screenClear();
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(10, 5);
    printf("=== QUIZ CLI ===");

    screenGotoxy(10, 7);
    printf("Escolha a dificuldade:");

    screenGotoxy(12, 9);
    printf("1 - Fácil");

    screenGotoxy(12, 10);
    printf("2 - Médio");

    screenGotoxy(12, 11);
    printf("3 - Difícil");

    screenGotoxy(12, 12);
    printf("4 - Misto");

    screenGotoxy(12, 14);
    printf("Q - Sair");

    screenGotoxy(10, 16);
    printf("Pressione uma tecla...");
    screenUpdate();
}

void printPergunta(int index) {
    screenClear();
    screenSetColor(CYAN, DARKGRAY);

    screenGotoxy(0, 0);
    printf("Pontuação: %d | Bônus: +%d", pontuacao, bonusSequencia);

    screenGotoxy(5, 5);
    printf("%s", quiz[index].pergunta);

    screenGotoxy(7, 7);
    printf("%s", quiz[index].opA);
    screenGotoxy(7, 8);
    printf("%s", quiz[index].opB);
    screenGotoxy(7, 9);
    printf("%s", quiz[index].opC);

    screenGotoxy(5, 11);
    printf("Escolha a resposta: (a / b / c)");
}

void mostrarResultado(char resposta, char correta) {
    screenGotoxy(5, 13);
    if (resposta == correta) {
        int pontosGanhos = valorBase + bonusSequencia;
        pontuacao += pontosGanhos;
        bonusSequencia++;
        printf("✔ Correto! +%d pontos", pontosGanhos);
    } else {
        printf("✘ Errado! Resposta correta era: %c", correta);
        bonusSequencia = 0;
    }
    screenUpdate();

    int count = 0;
    while (count < 10) {
        if (timerTimeOver()) count++;
    }
}

int main() {
    screenInit(1);
    keyboardInit();
    timerInit(100); // 100ms
    srand(time(NULL)); // aleatoriedade

    while (1) {
        printMenu();
        char nivel = 0;

        while (1) {
            if (keyhit()) {
                nivel = readch();
                if (nivel == '1' || nivel == '2' || nivel == '3' || nivel == '4' || nivel == 'q' || nivel == 'Q') break;
            }
        }

        if (nivel == 'q' || nivel == 'Q') break;

        static Questao quizMisto[18];
        switch (nivel) {
            case '1':
                quiz = quizFacil;
                totalPerguntas = sizeof(quizFacil)/sizeof(Questao);
                valorBase = 2;
                break;
            case '2':
                quiz = quizMedio;
                totalPerguntas = sizeof(quizMedio)/sizeof(Questao);
                valorBase = 4;
                break;
            case '3':
                quiz = quizDificil;
                totalPerguntas = sizeof(quizDificil)/sizeof(Questao);
                valorBase = 6;
                break;
            case '4': {
                int index = 0;
                for (int i = 0; i < sizeof(quizFacil)/sizeof(Questao); i++) quizMisto[index++] = quizFacil[i];
                for (int i = 0; i < sizeof(quizMedio)/sizeof(Questao); i++) quizMisto[index++] = quizMedio[i];
                for (int i = 0; i < sizeof(quizDificil)/sizeof(Questao); i++) quizMisto[index++] = quizDificil[i];
                quiz = quizMisto;
                totalPerguntas = index;
                valorBase = 3;
                break;
            }
        }

        pontuacao = 0;
        perguntaAtual = 0;
        bonusSequencia = 0;

        int indices[totalPerguntas];
        embaralharIndices(indices, totalPerguntas);

        for (int i = 0; i < totalPerguntas; i++) {
            int idx = indices[i];
            printPergunta(idx);
            screenUpdate();

            char resposta = 0;
            while (!keyhit()) {}
            resposta = readch();

            if (resposta == 'a' || resposta == 'b' || resposta == 'c') {
                mostrarResultado(resposta, quiz[idx].correta);
            }
        }

        screenClear();
        screenGotoxy(10, 10);
        printf("Quiz encerrado! Pontuação final: %d", pontuacao);
        screenUpdate();

        int wait = 0;
        while (wait < 30) {
            if (timerTimeOver()) wait++;
        }
    }

    screenClear();
    screenGotoxy(10, 10);
    printf("Obrigado por jogar! :)");
    screenUpdate();

    int wait = 0;
    while (wait < 30) {
        if (timerTimeOver()) wait++;
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}
