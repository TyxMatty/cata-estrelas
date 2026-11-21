#include "raylib.h"
#include "jogo.h"
#include <stdlib.h>
#include <time.h>

static void TextoCentro(const char *t, int y, int tam, Color cor) {
    DrawText(t, (LARGURA - MeasureText(t, tam)) / 2, y, tam, cor);
}

static void DesenharMenu(const char *titulo, const char *itens[], int n, int sel) {
    TextoCentro(titulo, 120, 56, GOLD);
    for (int i = 0; i < n; i++)
        TextoCentro(i == sel ? TextFormat("> %s <", itens[i]) : itens[i], 270 + i * 50, 30, i == sel ? GOLD : RAYWHITE);
    TextoCentro("Setas: navegar e ajustar   ENTER: escolher   ESC: voltar", ALTURA - 45, 18, LIGHTGRAY);
}

static void DesenharCena(const Jogo *j) {
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
}

static void Desenhar(const Jogo *j, Tela voltar, int sel, bool mostrarFps) {
    static const char *dificuldades[] = { "Facil", "Normal", "Dificil" };
    bool cena = j->tela >= JOGANDO || (j->tela == CONFIG && voltar == PAUSA);

    ClearBackground((Color){ 10, 12, 35, 255 });
    for (int i = 0; i < 70; i++) DrawPixel((i * 137) % LARGURA, (i * 211) % ALTURA, GRAY);
    if (cena) DesenharCena(j);
    if (cena && j->tela != JOGANDO) DrawRectangle(0, 0, LARGURA, ALTURA, Fade(BLACK, 0.7f));

    switch (j->tela) {
    case MENU:
        DesenharMenu("CATA-ESTRELAS", (const char *[]){ "Jogar", "Configuracoes", "Sair" }, 3, sel);
        TextoCentro(TextFormat("Pegue estrelas, desvie dos meteoros e faca %d pontos", PONTOS_VITORIA), 195, 20, RAYWHITE);
        break;
    case CONFIG:
        DesenharMenu("CONFIGURACOES", (const char *[]){
            TextFormat("Dificuldade: %s (%d vidas)", dificuldades[j->dificuldade], JogoVidasIniciais(j->dificuldade)),
            TextFormat("Mostrar FPS: %s", mostrarFps ? "Sim" : "Nao"),
            "Voltar" }, 3, sel);
        if (voltar == PAUSA) TextoCentro("As vidas da nova dificuldade valem na proxima partida", 200, 18, LIGHTGRAY);
        break;
    case PAUSA:
        DesenharMenu("PAUSADO", (const char *[]){ "Continuar", "Configuracoes", "Menu principal" }, 3, sel);
        break;
    case VITORIA:
    case DERROTA:
        TextoCentro(j->tela == VITORIA ? "VOCE VENCEU!" : "FIM DE JOGO", 200, 60, j->tela == VITORIA ? GREEN : RED);
        TextoCentro("ENTER: jogar de novo   ESC: menu principal", 300, 24, RAYWHITE);
        break;
    default:
        break;
    }
    if (mostrarFps) DrawFPS(LARGURA - 95, ALTURA - 25);
}

int main(int argc, char **argv) {
    int demo = argc > 1 ? atoi(argv[1]) : -1;
    srand(time(NULL));
    InitWindow(LARGURA, ALTURA, "Cata-Estrelas");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    Jogo j = { .dificuldade = NORMAL };
    JogoIniciar(&j);
    if (demo >= 0) j.tela = demo;
    Tela voltar = MENU;
    int sel = 0, quadros = 0;
    bool mostrarFps = false, sair = false;

    while (!sair && !WindowShouldClose()) {
        bool enter = IsKeyPressed(KEY_ENTER), esc = IsKeyPressed(KEY_ESCAPE), p = IsKeyPressed(KEY_P);
        int lado = (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) - (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A));
        Tela antes = j.tela;

        if (j.tela == MENU || j.tela == CONFIG || j.tela == PAUSA)
            sel = (sel + 3 + (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) - (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))) % 3;

        switch (j.tela) {
        case MENU:
            if (enter && sel == 0) { JogoIniciar(&j); j.tela = JOGANDO; }
            if (enter && sel == 1) { voltar = MENU; j.tela = CONFIG; }
            if (enter && sel == 2) sair = true;
            break;
        case CONFIG:
            if (sel == 0 && (lado || enter)) j.dificuldade = (j.dificuldade + (lado ? lado : 1) + 3) % 3;
            if (sel == 1 && (lado || enter)) mostrarFps = !mostrarFps;
            if (esc || (enter && sel == 2)) j.tela = voltar;
            break;
        case JOGANDO:
            if (esc || p) j.tela = PAUSA;
            break;
        case PAUSA:
            if (esc || p || (enter && sel == 0)) j.tela = JOGANDO;
            else if (enter && sel == 1) { voltar = PAUSA; j.tela = CONFIG; }
            else if (enter && sel == 2) JogoIniciar(&j);
            break;
        case VITORIA:
        case DERROTA:
            if (enter) { JogoIniciar(&j); j.tela = JOGANDO; }
            else if (esc) JogoIniciar(&j);
            break;
        }
        if (j.tela != antes) sel = 0;

        float direcao = (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) - (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A));
        JogoAtualizar(&j, GetFrameTime(), direcao);

        BeginDrawing();
        Desenhar(&j, voltar, sel, mostrarFps);
        EndDrawing();

        if (demo >= 0 && ++quadros == 150) { TakeScreenshot("screenshot.png"); break; }
    }
    CloseWindow();
    return 0;
}
