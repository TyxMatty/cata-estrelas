#include "../src/jogo.h"
#include <assert.h>
#include <stdio.h>

static void Soltar(Jogo *j, bool meteoro) {
    j->objetos[0] = (Objeto){ .x = j->jogadorX, .y = JOGADOR_Y, .raio = 12, .ativo = true, .meteoro = meteoro };
    JogoAtualizar(j, 0, 0);
}

int main(void) {
    int vidas = JogoVidasIniciais(NORMAL);
    assert(JogoVidasIniciais(FACIL) > vidas && vidas > JogoVidasIniciais(DIFICIL));

    Jogo j = { .dificuldade = NORMAL };
    JogoIniciar(&j);
    Soltar(&j, true);
    assert(j.vidas == vidas && "no menu nada acontece");

    j.tela = JOGANDO;
    Soltar(&j, false);
    assert(j.pontos == PONTOS_ESTRELA && j.recorde == PONTOS_ESTRELA);
    Soltar(&j, true);
    assert(j.vidas == vidas - 1 && j.tela == JOGANDO);

    j.tela = PAUSA;
    Soltar(&j, true);
    JogoAtualizar(&j, 1, 1);
    assert(j.vidas == vidas - 1 && j.jogadorX == LARGURA / 2.0f && "pausado nada acontece");
    j.objetos[0].ativo = false;
    j.tela = JOGANDO;

    JogoAtualizar(&j, 100, -1);
    assert(j.jogadorX == JOGADOR_L / 2 && "jogador nao sai da tela");

    j.pontos = PONTOS_VITORIA - PONTOS_ESTRELA;
    Soltar(&j, false);
    assert(j.tela == VITORIA);

    j.dificuldade = DIFICIL;
    JogoIniciar(&j);
    assert(j.recorde == PONTOS_VITORIA && j.pontos == 0 && "reinicio mantem o recorde");
    assert(j.dificuldade == DIFICIL && j.vidas == JogoVidasIniciais(DIFICIL) && "reinicio mantem a dificuldade");
    j.tela = JOGANDO;
    j.vidas = 1;
    Soltar(&j, true);
    assert(j.tela == DERROTA);

    puts("OK");
    return 0;
}
