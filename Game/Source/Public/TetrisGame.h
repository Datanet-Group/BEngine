#pragma once
#include "Engine/Source/Runtime/Core/Public/IGame.h"
#include <array>
#include <vector>

class Renderer2D; class Input;

class TetrisGame : public IGame {
public:
    void OnAttach(Renderer2D* r, Input* i) override;
    void OnDetach() override {}
    void Update(const UpdateContext& ctx) override;
    void Render() override;

private:
    Renderer2D* R=nullptr; Input* I=nullptr;
    static constexpr int COLS=10, ROWS=20, CELL=48;
    std::array<std::array<int, COLS>, ROWS> board{};
    struct Piece { int type=0, rot=0, x=3, y=-2; } cur, nxt;
    float fallTimer=0.f, fallPeriod=0.6f; int score=0, lines=0, level=1; bool gameOver=false;
    std::vector<std::vector<std::array<std::pair<int,int>,4>>> shapes;
    
    // Previous key states for edge detection
    bool prevLeft=false, prevRight=false, prevZ=false, prevX=false, prevSpace=false, prevR=false;

    void Reset(); void Spawn(); bool Collides(const Piece&) const; void LockPiece();
    int ClearLines(); void Rotate(int dir); void Move(int dx); void DropOne(); void HardDrop();
    void SpeedUp(bool downHeld); void UpdateLevel();
    void DrawCell(int gx,int gy,int color, int offsetX = -1, int offsetY = -1); void DrawBoard(); void DrawPiece(const Piece&,int alpha=255); void DrawUI();
};
