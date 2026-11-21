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
#define VIDAS_INICIAIS 3
#define PONTOS_ESTRELA 10
#define PONTOS_VITORIA 300

typedef enum { MENU, JOGANDO, VITORIA, DERROTA } Tela;

typedef struct {
    float x, y, raio, vel;
    bool ativo, meteoro;
} Objeto;

typedef struct {
    Tela tela;
    float jogadorX;
    int pontos, vidas, recorde;
    float tempoSpawn;
    Objeto objetos[MAX_OBJETOS];
} Jogo;

void JogoIniciar(Jogo *j);
void JogoAtualizar(Jogo *j, float dt, float direcao);

#endif
