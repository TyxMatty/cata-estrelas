#ifndef JOGO_H
#define JOGO_H
#include <stdbool.h>

#define LARGURA 800
#define ALTURA 600
#define JOGADOR_L 90.0f
#define JOGADOR_A 18.0f
#define JOGADOR_Y (ALTURA - 50.0f)
#define JOGADOR_VEL 480.0f
#define MAX_OBJETOS 32

#define PONTOS_ESTRELA 10
#define PONTOS_VITORIA 300

typedef enum { MENU, CONFIG, JOGANDO, PAUSA, VITORIA, DERROTA } Tela;
typedef enum { FACIL, NORMAL, DIFICIL } Dificuldade;

typedef struct {
    float x, y, raio, vel;
    bool ativo, meteoro;
} Objeto;

typedef struct {
    Tela tela;
    float jogadorX;
    int pontos, vidas, recorde, dificuldade;
    float tempoSpawn;
    Objeto objetos[MAX_OBJETOS];
} Jogo;

int JogoVidasIniciais(int dificuldade);
void JogoIniciar(Jogo *j);
void JogoAtualizar(Jogo *j, float dt, float direcao);

#endif
