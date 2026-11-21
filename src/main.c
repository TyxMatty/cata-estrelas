#include "raylib.h"
#include "jogo.h"
#include <stdlib.h>
#include <time.h>

static void TextoCentro(const char *t, int y, int tam, Color cor) {
    DrawText(t, (LARGURA - MeasureText(t, tam)) / 2, y, tam, cor);
}

static void Desenhar(const Jogo *j, bool pausado) {
    ClearBackground((Color){ 10, 12, 35, 255 });
    for (int i = 0; i < 70; i++) DrawPixel((i * 137) % LARGURA, (i * 211) % ALTURA, GRAY);

    if (j->tela == MENU) {
        TextoCentro("CATA-ESTRELAS", 150, 60, GOLD);
        TextoCentro("Pegue as estrelas, desvie dos meteoros", 250, 22, RAYWHITE);
        TextoCentro(TextFormat("Faca %d pontos para vencer. Voce tem %d vidas.", PONTOS_VITORIA, VIDAS_INICIAIS), 285, 22, RAYWHITE);
        TextoCentro("Setas ou A/D: mover   P: pausar   ESC: sair", 350, 20, LIGHTGRAY);
        TextoCentro("ENTER para comecar", 430, 30, GREEN);
        return;
    }

    for (int i = 0; i < MAX_OBJETOS; i++) {
        const Objeto *o = &j->objetos[i];
        if (!o->ativo) continue;
        if (o->meteoro) {
            DrawCircle(o->x, o->y, o->raio, BROWN);
            DrawCircle(o->x - 5, o->y - 4, o->raio / 3, DARKBROWN);
        } else {
            DrawPoly((Vector2){ o->x, o->y }, 4, o->raio, 0, GOLD);
            DrawPoly((Vector2){ o->x, o->y }, 4, o->raio, 45, YELLOW);
        }
    }
    DrawRectangleRounded((Rectangle){ j->jogadorX - JOGADOR_L / 2, JOGADOR_Y, JOGADOR_L, JOGADOR_A }, 0.6f, 8, SKYBLUE);

    DrawText(TextFormat("Pontos: %d / %d", j->pontos, PONTOS_VITORIA), 10, 10, 22, RAYWHITE);
    DrawText(TextFormat("Vidas: %d", j->vidas), 10, 36, 22, RED);
    DrawText(TextFormat("Recorde: %d", j->recorde), LARGURA - 170, 10, 22, GOLD);

    if (pausado) TextoCentro("PAUSADO", 260, 50, RAYWHITE);
    if (j->tela == VITORIA) TextoCentro("VOCE VENCEU!", 220, 60, GREEN);
    if (j->tela == DERROTA) TextoCentro("FIM DE JOGO", 220, 60, RED);
    if (j->tela != JOGANDO) TextoCentro("ENTER para jogar de novo", 320, 26, RAYWHITE);
}

int main(int argc, char **argv) {
    (void)argv;
    bool demo = argc > 1;
    srand(time(NULL));
    InitWindow(LARGURA, ALTURA, "Cata-Estrelas");
    SetTargetFPS(60);

    Jogo j = { 0 };
    JogoIniciar(&j);
    if (demo) j.tela = JOGANDO;
    bool pausado = false;
    int quadros = 0;

    while (!WindowShouldClose()) {
        if (j.tela != JOGANDO && IsKeyPressed(KEY_ENTER)) {
            JogoIniciar(&j);
            j.tela = JOGANDO;
            pausado = false;
        }
        if (j.tela == JOGANDO && IsKeyPressed(KEY_P)) pausado = !pausado;
        float direcao = (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) - (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A));

        if (!pausado) JogoAtualizar(&j, GetFrameTime(), direcao);

        BeginDrawing();
        Desenhar(&j, pausado);
        EndDrawing();

        if (demo && ++quadros == 150) { TakeScreenshot("screenshot.png"); break; }
    }
    CloseWindow();
    return 0;
}
