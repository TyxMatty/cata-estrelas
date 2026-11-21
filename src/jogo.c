#include "jogo.h"
#include <math.h>
#include <stdlib.h>

void JogoIniciar(Jogo *j) {
    int recorde = j->recorde;
    *j = (Jogo){ .tela = MENU, .jogadorX = LARGURA / 2.0f, .vidas = VIDAS_INICIAIS, .recorde = recorde };
}

static void CriarObjeto(Jogo *j) {
    for (int i = 0; i < MAX_OBJETOS; i++) {
        Objeto *o = &j->objetos[i];
        if (o->ativo) continue;
        o->ativo = true;
        o->meteoro = rand() % 100 < 40;
        o->raio = o->meteoro ? 18.0f : 12.0f;
        o->x = o->raio + rand() % (int)(LARGURA - 2 * o->raio);
        o->y = -o->raio;
        o->vel = 180.0f + j->pontos * 0.8f + rand() % 100;
        return;
    }
}

void JogoAtualizar(Jogo *j, float dt, float direcao) {
    if (j->tela != JOGANDO) return;

    j->jogadorX += direcao * JOGADOR_VEL * dt;
    if (j->jogadorX < JOGADOR_L / 2) j->jogadorX = JOGADOR_L / 2;
    if (j->jogadorX > LARGURA - JOGADOR_L / 2) j->jogadorX = LARGURA - JOGADOR_L / 2;

    j->tempoSpawn -= dt;
    if (j->tempoSpawn <= 0) {
        CriarObjeto(j);
        j->tempoSpawn = fmaxf(0.25f, 0.8f - j->pontos * 0.0015f);
    }

    for (int i = 0; i < MAX_OBJETOS; i++) {
        Objeto *o = &j->objetos[i];
        if (!o->ativo) continue;
        o->y += o->vel * dt;

        bool tocou = o->y + o->raio >= JOGADOR_Y && o->y - o->raio <= JOGADOR_Y + JOGADOR_A &&
                     fabsf(o->x - j->jogadorX) <= JOGADOR_L / 2 + o->raio;
        if (tocou) {
            o->ativo = false;
            if (o->meteoro) j->vidas--;
            else j->pontos += PONTOS_ESTRELA;
        } else if (o->y - o->raio > ALTURA) {
            o->ativo = false;
        }
    }

    if (j->pontos > j->recorde) j->recorde = j->pontos;
    if (j->vidas <= 0) j->tela = DERROTA;
    else if (j->pontos >= PONTOS_VITORIA) j->tela = VITORIA;
}
