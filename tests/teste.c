#include "../src/jogo.h"
#include <assert.h>
#include <stdio.h>

static void Soltar(Jogo *j, bool meteoro) {
    j->objetos[0] = (Objeto){ .x = j->jogadorX, .y = JOGADOR_Y, .raio = 12, .ativo = true, .meteoro = meteoro };
    JogoAtualizar(j, 0, 0);
}

int main(void) {
    Jogo j = { 0 };
    JogoIniciar(&j);
    Soltar(&j, true);
    assert(j.vidas == VIDAS_INICIAIS && "no menu nada acontece");

    j.tela = JOGANDO;
    Soltar(&j, false);
    assert(j.pontos == PONTOS_ESTRELA && j.recorde == PONTOS_ESTRELA);
    Soltar(&j, true);
    assert(j.vidas == VIDAS_INICIAIS - 1 && j.tela == JOGANDO);

    JogoAtualizar(&j, 100, -1);
    assert(j.jogadorX == JOGADOR_L / 2 && "jogador nao sai da tela");

    j.pontos = PONTOS_VITORIA - PONTOS_ESTRELA;
    Soltar(&j, false);
    assert(j.tela == VITORIA);

    JogoIniciar(&j);
    assert(j.recorde == PONTOS_VITORIA && j.pontos == 0 && "reinicio mantem o recorde");
    j.tela = JOGANDO;
    j.vidas = 1;
    Soltar(&j, true);
    assert(j.tela == DERROTA);

    puts("OK");
    return 0;
}
