// File: Game/Source/Private/TetrisGame.cpp
#include "TetrisGame.h"
#include "Renderer2D.h"
#include "Input.h"
#include "BRoom2D.h"     // alias of BLevel2D
#include <SDL.h>         // SDL_GetTicks for seeding
#include <SDL_scancode.h>
#include <cstdlib>
#include <algorithm>

// Init renderer/input, seed RNG, define shapes, open a room, then reset.
void TetrisGame::OnAttach(Renderer2D* r, Input* i) {
    R = r; I = i;
    std::srand(static_cast<unsigned>(SDL_GetTicks()));

    // Tetromino shapes (I, J, L, O, S, T, Z), 4 rotations each
    shapes = {
        // I
        { {{{0,0},{1,0},{2,0},{3,0}}},
          {{{0,-1},{0,0},{0,1},{0,2}}},
          {{{0,0},{1,0},{2,0},{3,0}}},
          {{{0,-1},{0,0},{0,1},{0,2}}} },
        // J
        { {{{0,0},{0,1},{1,1},{2,1}}},
          {{{1,0},{2,0},{1,1},{1,2}}},
          {{{0,1},{1,1},{2,1},{2,2}}},
          {{{1,0},{1,1},{1,2},{0,2}}} },
        // L
        { {{{0,1},{1,1},{2,1},{2,0}}},
          {{{1,0},{1,1},{1,2},{2,2}}},
          {{{0,1},{1,1},{2,1},{0,2}}},
          {{{0,0},{1,0},{1,1},{1,2}}} },
        // O
        { {{{0,0},{1,0},{0,1},{1,1}}},
          {{{0,0},{1,0},{0,1},{1,1}}},
          {{{0,0},{1,0},{0,1},{1,1}}},
          {{{0,0},{1,0},{0,1},{1,1}}} },
        // S
        { {{{1,0},{2,0},{0,1},{1,1}}},
          {{{1,0},{1,1},{2,1},{2,2}}},
          {{{1,1},{2,1},{0,2},{1,2}}},
          {{{0,0},{0,1},{1,1},{1,2}}} },
        // T
        { {{{1,0},{0,1},{1,1},{2,1}}},
          {{{1,0},{1,1},{2,1},{1,2}}},
          {{{0,1},{1,1},{2,1},{1,2}}},
          {{{1,0},{0,1},{1,1},{1,2}}} },
        // Z
        { {{{0,0},{1,0},{1,1},{2,1}}},
          {{{2,0},{1,1},{2,1},{1,2}}},
          {{{0,1},{1,1},{1,2},{2,2}}},
          {{{1,0},{0,1},{1,1},{0,2}}} }
    };

    World.SetIO(R, I);
    auto room = std::make_unique<BRoom2D>(COLS, ROWS, CELL);
    room->SetBackground(32, 32, 40, 255);
    World.OpenLevel(std::move(room));

    Reset();
}

void TetrisGame::Reset() {
    for (auto& row : board) row.fill(0);
    score = 0; lines = 0; level = 1;
    fallPeriod = 0.6f; fallTimer = 0.f;
    gameOver = false;
    RefillAndShuffle();
    nxt.type = NextFromBag();
    Spawn();
}

// 7-bag: pull next piece, refill/shuffle as needed
int TetrisGame::NextFromBag() {
    if (bagIdx >= 7) RefillAndShuffle();
    return bag[bagIdx++];
}
void TetrisGame::RefillAndShuffle() {
    bag = {0,1,2,3,4,5,6};
    // Fisher–Yates using rand()
    for (int i = 6; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(bag[i], bag[j]);
    }
    bagIdx = 0;
}

void TetrisGame::Spawn() {
    cur = nxt;
    nxt.type = NextFromBag();
    nxt.rot = 0; nxt.x = 3; nxt.y = -2;
    if (Collides(cur)) gameOver = true;
}

void TetrisGame::DrawCell(int gx, int gy, int color) {
    static const uint8_t colors[][3] = {
        {0,0,0}, {0,255,255}, {0,0,255}, {255,127,0},
        {255,255,0}, {0,255,0}, {255,0,255}, {255,0,0}
    };

    const int gameWidth  = COLS * CELL;
    const int gameHeight = ROWS * CELL;
    const int offsetX = (R->ScreenW() - gameWidth) / 2;
    const int offsetY = (R->ScreenH() - gameHeight) / 2;

    const int sx = gx * CELL + offsetX;
    const int sy = gy * CELL + offsetY;

    if (color > 0) {
        R->FillRect(sx, sy, CELL-1, CELL-1,
                    colors[color][0], colors[color][1], colors[color][2], 255);
    }
    R->DrawRect(sx, sy, CELL-1, CELL-1, 128,128,128,255);
}

void TetrisGame::DrawBoard() {
    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x)
            DrawCell(x, y, board[y][x]);
}

void TetrisGame::DrawPiece(const Piece& p, int /*alpha*/) {
    if (p.type < 0 || static_cast<size_t>(p.type) >= shapes.size()) return;
    const auto& shape = shapes[p.type][p.rot % shapes[p.type].size()];
    for (const auto& [dx,dy] : shape) {
        const int x = p.x + dx;
        const int y = p.y + dy;
        if (y >= 0) DrawCell(x, y, p.type + 1);
    }
}

void TetrisGame::DrawUI() {
    const int gameWidth  = COLS * CELL;
    const int gameHeight = ROWS * CELL;
    const int offsetX = (R->ScreenW() - gameWidth) / 2;
    const int offsetY = (R->ScreenH() - gameHeight) / 2;

    const int previewX = offsetX + gameWidth + 20;
    const int previewY = offsetY;

    R->DrawRect(previewX, previewY, CELL*4, CELL*4, 128,128,128,255);

    Piece preview = nxt;
    preview.x = (previewX / CELL) + 1;
    preview.y = (previewY / CELL) + 1;
    DrawPiece(preview);
}

// World draws background; we draw board, active piece, and UI.
void TetrisGame::Render() {
    World.Render();
    DrawBoard();
    DrawPiece(cur);
    DrawUI();
}

bool TetrisGame::Collides(const Piece& p) const {
    if (p.type < 0 || static_cast<size_t>(p.type) >= shapes.size()) return true;
    const auto& shape = shapes[p.type][p.rot % shapes[p.type].size()];
    for (const auto& [dx,dy] : shape) {
        const int x = p.x + dx;
        const int y = p.y + dy;
        if (x < 0 || x >= COLS || y >= ROWS) return true;
        if (y >= 0 && board[y][x] != 0)    return true;
    }
    return false;
}

void TetrisGame::Rotate(int dir) {
    if (gameOver) return;
    Piece test = cur;
    test.rot = (test.rot + dir + 4) % 4;
    if (!Collides(test)) cur = test;
}

void TetrisGame::Move(int dx) {
    if (gameOver) return;
    Piece test = cur; test.x += dx;
    if (!Collides(test)) cur = test;
}

void TetrisGame::DropOne() {
    if (gameOver) return;
    Piece test = cur; test.y++;
    if (!Collides(test)) { cur = test; fallTimer = 0.f; }
    else { LockPiece(); }
}

void TetrisGame::HardDrop() {
    if (gameOver) return;
    while (!Collides(cur)) cur.y++;
    cur.y--;
    LockPiece();
}

void TetrisGame::LockPiece() {
    const auto& shape = shapes[cur.type][cur.rot % shapes[cur.type].size()];
    for (const auto& [dx,dy] : shape) {
        const int x = cur.x + dx;
        const int y = cur.y + dy;
        if (y >= 0 && y < ROWS) board[y][x] = cur.type + 1;
    }
    score += ClearLines() * 100 * level;
    UpdateLevel();
    Spawn();
}

int TetrisGame::ClearLines() {
    int linesCleared = 0;
    for (int y = ROWS-1; y >= 0; --y) {
        bool full = true;
        for (int x = 0; x < COLS; ++x)
            if (board[y][x] == 0) { full = false; break; }

        if (full) {
            ++linesCleared; ++lines;
            for (int y2 = y; y2 > 0; --y2) board[y2] = board[y2-1];
            board[0].fill(0);
            ++y; // re-check after shift
        }
    }
    return linesCleared;
}

void TetrisGame::SpeedUp(bool downHeld) {
    fallPeriod = downHeld ? 0.05f : 0.6f / level;
}

void TetrisGame::UpdateLevel() {
    level = 1 + (lines / 10);
    fallPeriod = 0.6f / level;
}

// Use engine's dt; tick world; input; auto-fall.
void TetrisGame::Update(const UpdateContext& ctx) {
    World.Tick(ctx.dt);

    if (gameOver) {
        if (I->KeyPressed(SDL_SCANCODE_R)) Reset();
        return;
    }

    if (I->KeyPressed(SDL_SCANCODE_LEFT))  Move(-1);
    if (I->KeyPressed(SDL_SCANCODE_RIGHT)) Move(1);
    if (I->KeyPressed(SDL_SCANCODE_Z))     Rotate(-1);
    if (I->KeyPressed(SDL_SCANCODE_X))     Rotate(1);
    if (I->KeyPressed(SDL_SCANCODE_SPACE)) HardDrop();

    SpeedUp(I->KeyDown(SDL_SCANCODE_DOWN));

    fallTimer += ctx.dt;
    if (fallTimer >= fallPeriod) {
        fallTimer = 0.f;
        DropOne();
    }
}
