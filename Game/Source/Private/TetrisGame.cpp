// Location: C:/Users/Damien/CLionProjects/BEngine/Game/Source/Private/TetrisGame.cpp

#include "TetrisGame.h"
#include "Engine/Source/Runtime/Renderer2D/Public/Renderer2D.h"
#include "Engine/Source/Runtime/Core/Public/Input.h"
#include <SDL_scancode.h>

/**
 * @brief Initializes the game state and sets up references to the renderer and input handler.
 *
 * This method assigns the provided renderer and input handler to the game instance.
 * It also initializes the tetromino shapes with their predefined rotations, using
 * specific layouts for each Tetris piece (I, J, L, O, S, T, Z).
 * After initializing the shapes, it calls the `Reset` method to prepare the game board,
 * reset the scoring and level information, and spawn the initial piece.
 *
 * @param r A pointer to the Renderer2D instance responsible for rendering the game.
 * @param i A pointer to the Input instance responsible for handling player inputs.
 */
void TetrisGame::OnAttach(Renderer2D* r, Input* i) {
    R = r;
    I = i;
    
    // Tetromino-Formen (I, J, L, O, S, T, Z)
    shapes = {
        // I-Piece (cyan) - KORRIGIERT
        {{{{{0,0},{1,0},{2,0},{3,0}}},  // Horizontale Position
          {{{0,-1},{0,0},{0,1},{0,2}}},  // Vertikale Position
          {{{0,0},{1,0},{2,0},{3,0}}},  // Horizontale Position (gleich wie erste)
          {{{0,-1},{0,0},{0,1},{0,2}}}}} // Vertikale Position (gleich wie zweite)
        ,




        // J-Piece (blau) - KORRIGIERT
        {{{{{0,0},{0,1},{1,1},{2,1}}},{{{1,0},{2,0},{1,1},{1,2}}},
          {{{0,1},{1,1},{2,1},{2,2}}},{{{1,0},{1,1},{1,2},{0,2}}}}},

        // L-Piece (orange) - KORRIGIERT
        {{{{{0,1},{1,1},{2,1},{2,0}}},{{{1,0},{1,1},{1,2},{2,2}}},
          {{{0,1},{1,1},{2,1},{0,2}}},{{{0,0},{1,0},{1,1},{1,2}}}}},

        // O-Piece (gelb) - KORRIGIERT
        {{{{{0,0},{1,0},{0,1},{1,1}}}}},

        // S-Piece (grün) - KORRIGIERT
        {{{{{1,0},{2,0},{0,1},{1,1}}},{{{1,0},{1,1},{2,1},{2,2}}},
          {{{1,1},{2,1},{0,2},{1,2}}},{{{0,0},{0,1},{1,1},{1,2}}}}},

        // T-Piece (lila) - KORRIGIERT
        {{{{{1,0},{0,1},{1,1},{2,1}}},{{{1,0},{1,1},{2,1},{1,2}}},
          {{{0,1},{1,1},{2,1},{1,2}}},{{{1,0},{0,1},{1,1},{1,2}}}}},

        // Z-Piece (rot) - KORRIGIERT
        {{{{{0,0},{1,0},{1,1},{2,1}}},{{{2,0},{1,1},{2,1},{1,2}}},
          {{{0,1},{1,1},{1,2},{2,2}}},{{{1,0},{0,1},{1,1},{0,2}}}}}
    };

    Reset();
}


/**
 *
 */
void TetrisGame::Reset() {
    for(auto& row : board) row.fill(0);
    score = 0;
    lines = 0;
    level = 1;
    fallPeriod = 0.6f;
    fallTimer = 0.f;
    gameOver = false;
    Spawn();
}

/**
 * @brief Spawns a new tetromino piece in the game.
 *
 * The method sets the current piece (`cur`) to the next piece (`nxt`) and then generates a new next piece with random type and default rotation and position.
 * It initializes the new piece's position in the middle of the top row.
 * If the current piece immediately collides with existing pieces or the board boundaries (i.e., no space to place it), the game is flagged as over.
 */
void TetrisGame::Spawn() {
    cur = nxt;
    nxt.type = rand() % 7;
    nxt.rot = 0;
    nxt.x = 3;
    nxt.y = -2;
    
    if(Collides(cur)) {
        gameOver = true;
    }
}

/**
 * @brief Draws an individual cell on the Tetris game board.
 *
 * This method renders a single cell of the game grid at the specified position
 * with the given color. The colors correspond to predefined values representing
 * the Tetris pieces. The cell is drawn centered on the screen, with an optional
 * border for highlighting. Empty cells are styled differently from occupied cells.
 *
 * @param gx The x-coordinate of the cell in the grid (grid-based).
 * @param gy The y-coordinate of the cell in the grid (grid-based).
 * @param color The color index of the cell, where 0 represents an empty cell,
 *        and values 1 to 7 correspond to specific Tetris piece colors.
 */
void TetrisGame::DrawCell(int gx, int gy, int color) {
    static const uint8_t colors[][3] = {
        {0,0,0},      // leer
        {0,255,255},  // cyan (I)
        {0,0,255},    // blau (J)
        {255,127,0},  // orange (L)
        {255,255,0},  // gelb (O)
        {0,255,0},    // grün (S)
        {255,0,255},  // lila (T)
        {255,0,0}     // rot (Z)
    };
    
    // Zentrieren des Spielfelds
    const int gameWidth = COLS * CELL;
    const int gameHeight = ROWS * CELL;
    const int offsetX = (1920 - gameWidth) / 2;
    const int offsetY = (1080 - gameHeight) / 2;
    
    int sx = gx * CELL + offsetX;
    int sy = gy * CELL + offsetY;
    
    if(color > 0) {
        R->FillRect(sx, sy, CELL-1, CELL-1, 
                    colors[color][0], colors[color][1], colors[color][2], 255);
    }
    R->DrawRect(sx, sy, CELL-1, CELL-1, 128, 128, 128, 255);
}

/**
 * @brief Renders the game board by drawing each cell at its corresponding position.
 *
 * This method iterates over the entire game board matrix and calls the `DrawCell`
 * method for each cell. The `DrawCell` function visualizes the cells based on their
 * assigned color value, representing either an empty space or a specific Tetris block.
 * The rendering takes into account the board's layout and cell-by-cell structure.
 */
void TetrisGame::DrawBoard() {
    for(int y = 0; y < ROWS; ++y) {
        for(int x = 0; x < COLS; ++x) {
            DrawCell(x, y, board[y][x]);
        }
    }
}

/**
 * Draws a Tetris piece onto the game board.
 *
 * This function renders the individual cells of a Tetris piece on the game board
 * by determining its shape and position. It uses the piece's type, rotation,
 * and position to calculate the appropriate grid coordinates for drawing each cell.
 *
 * If the piece type is invalid or out of bounds (not within the range of defined shapes),
 * the function returns without performing any rendering. Additionally, it skips
 * drawing any cells that are positioned above the visible portion of the board.
 *
 * Each valid cell of the piece is rendered using the `DrawCell` function, with the piece type
 * determining the visual appearance.
 *
 * @param p The Tetris piece to be drawn, including its type, rotation, and position.
 * @param alpha Unused parameter, reserved for potential transparency-related functionality.
 */
void TetrisGame::DrawPiece(const Piece& p, int /*alpha*/) {
    if(p.type < 0 || static_cast<size_t>(p.type) >= shapes.size()) return;
    
    const auto& shape = shapes[p.type][p.rot % shapes[p.type].size()];
    for(const auto& [dx,dy] : shape) {
        int x = p.x + dx;
        int y = p.y + dy;
        if(y >= 0) {
            DrawCell(x, y, p.type + 1);
        }
    }
}

/**
 * @brief Renders the user interface for the Tetris game, including the preview for the next piece.
 *
 * This method calculates the dimensions and positions required to display the game UI elements.
 * It positions the "next piece" preview box to the right of the game board and renders its border
 * using a gray-colored rectangle. The next piece is then drawn within this preview area,
 * with its position adjusted for better alignment within the box.
 *
 * The positions are dynamically calculated based on the screen resolution and game board layout to ensure proper alignment.
 *
 * Dependencies include the renderer instance (`R`) for drawing UI elements,
 * and the `nxt` object, which represents the next Tetris piece that will spawn.
 */
void TetrisGame::DrawUI() {
    const int gameWidth = COLS * CELL;
    const int gameHeight = ROWS * CELL;
    const int offsetX = (1920 - gameWidth) / 2;
    const int offsetY = (1080 - gameHeight) / 2;
    
    // Next-Piece-Vorschau (rechts vom Spielfeld)
    int previewX = offsetX + gameWidth + 20;
    int previewY = offsetY;
    
    R->DrawRect(previewX, previewY, CELL*4, CELL*4, 128, 128, 128, 255);
    
    Piece preview = nxt;

    // Korrigierte Position für die Vorschau
    preview.x = (previewX / CELL) + 1;  // +1 für einen kleinen Abstand vom Rand
    preview.y = (previewY / CELL) + 1;  // +1 für einen kleinen Abstand vom Rand

    DrawPiece(preview);
}

/**
 * @brief Renders the current state of the Tetris game to the screen.
 *
 * This method clears the screen and draws various game elements, including the
 * game board, the currently active piece, the user interface (e.g., next piece
 * preview), and any relevant background elements. After rendering all these
 * components, the method presents the drawn frame to the screen.
 *
 * The rendering process includes:
 * - Clearing the screen with a solid color.
 * - Calculating the dimensions and position of the gameboard.
 * - Rendering the background of the gameboard for visual clarity and appeal.
 * - Drawing the blocks on the gameboard, the current piece in play, and the next piece preview.
 * - Calling the present function of the renderer to display the final frame on the screen.
 *
 * This method ensures that all game visuals remain up-to-date and provides a
 * visually consistent experience each frame.
 */
void TetrisGame::Render() {
    R->Clear(0, 0, 0, 255);

    // Spielfeld-Hintergrund
    const int gameWidth = COLS * CELL;
    const int gameHeight = ROWS * CELL;
    const int offsetX = (1920 - gameWidth) / 2;
    const int offsetY = (1080 - gameHeight) / 2;

    // Optional: Hintergrund für das Spielfeld
    R->FillRect(offsetX - 2, offsetY - 2,
                gameWidth + 4, gameHeight + 4,
                32, 32, 32, 255);

    DrawBoard();
    DrawPiece(cur);
    DrawUI();

    R->Present();
}


/**
 * @brief Checks for collisions between the given piece and the game board or boundaries.
 *
 * This function determines whether the given Tetris piece overlaps with any non-empty
 * cells on the board or goes out of bounds based on its current position, rotation, and type.
 *
 * @param p A reference to the Piece object being checked for collision.
 * @return true if the piece collides with the game board's boundaries, cells, or is invalid.
 * @return false if there are no collisions.
 */
bool TetrisGame::Collides(const Piece& p) const {
    if(p.type < 0 || static_cast<size_t>(p.type) >= shapes.size()) return true;
    
    const auto& shape = shapes[p.type][p.rot % shapes[p.type].size()];
    for(const auto& [dx,dy] : shape) {
        int x = p.x + dx;
        int y = p.y + dy;
        
        // Spezielle Behandlung für I-Piece (Typ 0)
        if(p.type == 0) {
            // Horizontale Ausrichtung (Rotation 0 oder 2)
            if(p.rot == 0 || p.rot == 2) {
                if(x < 0 || x >= COLS || y >= ROWS) return true;
            }
            // Vertikale Ausrichtung (Rotation 1 oder 3)
            else {
                if(x < 0 || x >= COLS || y >= ROWS) return true;
            }
        } else {
            // Normale Kollisionsprüfung für alle anderen Teile
            if(x < 0 || x >= COLS || y >= ROWS) return true;
        }

        if(y >= 0 && board[y][x] != 0) return true;
    }
    return false;
}


/**
 * Rotates the currently active Tetris piece in the specified direction if the game is not over and the rotation
 * does not result in a collision.
 *
 * The rotation modifies the `rot` attribute of the current piece, cycling through a value range of 0 to 3 (inclusive),
 * which corresponds to the four possible rotations of a Tetris piece. If the attempted rotation of the piece does not
 * cause it to collide with the board or other pieces, the current piece is updated with the rotated position.
 *
 * @param dir The direction of rotation: positive for clockwise rotation, negative for counter-clockwise rotation.
 *            Typically, `dir` would be 1 for clockwise and -1 for counter-clockwise rotations.
 */
void TetrisGame::Rotate(int dir) {
    if(gameOver) return;
    
    Piece test = cur;
    test.rot = (test.rot + dir + 4) % 4;
    
    if(!Collides(test)) {
        cur = test;
    }
}

/**
 * @brief Moves the currently controlled Tetris piece horizontally.
 *
 * This function attempts to shift the currently active Tetris piece (cur) by a specified
 * horizontal displacement (dx). The move is only performed if the resulting position
 * does not lead to a collision with other pieces or the boundaries of the board.
 *
 * @param dx The horizontal displacement applied to the piece. Positive values move
 *           the piece to the right, and negative values move it to the left.
 *
 * @note If the game is over (@c gameOver is true), this function does nothing.
 */
void TetrisGame::Move(int dx) {
    if(gameOver) return;
    
    Piece test = cur;
    test.x += dx;
    
    // Spezielle Behandlung für I-Piece
    if(cur.type == 0) {
        // Prüfe ob die Bewegung gültig ist
        if(!Collides(test)) {
            cur = test;
        }
    } else {
        // Normale Bewegung für andere Teile
        if(!Collides(test)) {
            cur = test;
        }
    }
}


/**
 * @brief Moves the currently active piece one unit downwards.
 *
 * This method attempts to move the active piece (`cur`) one step down on the game board.
 * If the piece does not collide with the board boundaries or existing blocks after the move,
 * the piece's position is updated, and the fall timer is reset to zero.
 * However, if the move would result in a collision, the piece is locked in its current position
 * by invoking the `LockPiece` method.
 *
 * If the game is already over, this method does nothing.
 */
void TetrisGame::DropOne() {
    if(gameOver) return;
    
    Piece test = cur;
    test.y++;
    
    if(!Collides(test)) {
        cur = test;
        fallTimer = 0;
    } else {
        LockPiece();
    }
}

/**
 * @brief Instantly drops the current piece to the lowest possible position on the board.
 *
 * This method repeatedly moves the active piece downward until it collides with
 * either the bottom of the board or an existing piece. After finding the valid
 * position, it locks the piece into place and triggers necessary updates like
 * clearing completed lines and spawning the next piece.
 *
 * If the game is over, the method returns immediately without performing any actions.
 */
void TetrisGame::HardDrop() {
    if(gameOver) return;
    
    while(!Collides(cur)) {
        cur.y++;
    }
    cur.y--;
    LockPiece();
}

/**
 * @brief Locks the current tetromino piece in place on the game board.
 *
 * This method integrates the current piece (`cur`) into the game board by marking its occupied cells as part of the board.
 * Once the piece is locked, it can no longer be moved or rotated.
 * After locking the piece, the method checks for any complete lines and removes them if present.
 * It also prepares the game state for the next piece or ends the game if no further moves are possible.
 */
void TetrisGame::LockPiece() {
    const auto& shape = shapes[cur.type][cur.rot % shapes[cur.type].size()];
    for(const auto& [dx,dy] : shape) {
        int x = cur.x + dx;
        int y = cur.y + dy;
        if(y >= 0 && y < ROWS) {
            board[y][x] = cur.type + 1;
        }
    }
    
    score += ClearLines() * 100 * level;
    UpdateLevel();
    Spawn();
}

/**
 * Clears completed lines from the Tetris board, shifts the remaining lines down,
 * and updates the internal line count.
 *
 * This method iterates over each row of the game board and checks if the row is
 * completely filled. If a row is fully filled, it is cleared, and all rows above
 * it are shifted down by one position. The topmost row is set to empty after the shift.
 *
 * The number of lines cleared during the invocation is returned as the method's result.
 *
 * @return The number of lines cleared from the board.
 */
int TetrisGame::ClearLines() {
    int linesCleared = 0;
    
    for(int y = ROWS-1; y >= 0; --y) {
        bool full = true;
        for(int x = 0; x < COLS; ++x) {
            if(board[y][x] == 0) {
                full = false;
                break;
            }
        }
        
        if(full) {
            linesCleared++;
            lines++;
            
            // Zeilen nach unten verschieben
            for(int y2 = y; y2 > 0; --y2) {
                board[y2] = board[y2-1];
            }
            board[0].fill(0);
            y++; // Diese Zeile nochmal prüfen
        }
    }
    
    return linesCleared;
}

/**
 * @brief Adjusts the falling speed of the current piece based on user input.
 *
 * This method modifies the fall speed of the game piece. If the player is holding
 * the "down" key, the falling speed is set to a rapid value. Otherwise, the fall speed
 * is adjusted based on the current game level, decreasing as the level increases.
 *
 * @param downHeld A boolean indicating whether the "down" key is actively being held.
 *                 If true, the piece falls significantly faster.
 */
void TetrisGame::SpeedUp(bool downHeld) {
    fallPeriod = downHeld ? 0.05f : 0.6f / level;
}

/**
 * Updates the current game level based on the number of lines cleared.
 *
 * This method recalculates the game level by dividing the total number of cleared lines
 * by 10 and adding 1. As the level increases, the falling period of the Tetris pieces
 * decreases, making the game progressively faster and more challenging.
 *
 * Effects:
 * - The `level` variable is updated to reflect the current game level.
 * - The `fallPeriod` variable is adjusted based on the new level, reducing the time interval
 *   between automatic piece drops.
 *
 * Preconditions:
 * - The `lines` variable must correctly track the total number of lines cleared during gameplay.
 *
 * Postconditions:
 * - The falling speed of pieces increases with higher levels.
 */
void TetrisGame::UpdateLevel() {
    level = 1 + (lines / 10);
    fallPeriod = 0.6f / level;
}

/**
 * @brief Updates the game state including piece movement, rotation, and falling logic.
 *
 * This method processes user inputs to move, rotate, or drop the current piece.
 * It handles automatic piece falling based on the configured time interval and
 * adjusts the game speed when the down arrow key is held. If a game-over state is reached,
 * it allows the player to reset the game by pressing the reset key.
 *
 * @param ctx A reference to the UpdateContext providing timing and state information
 *            for the current update cycle.
 */
void TetrisGame::Update(const UpdateContext& /*ctx*/) {
    if(gameOver) {
        bool currR = I->KeyPressed(SDL_SCANCODE_R);
        if(currR && !prevR) {
            Reset();
        }
        prevR = currR;
        return;
    }
    
    // Steuerung mit Edge-Detection
    bool currLeft = I->KeyPressed(SDL_SCANCODE_LEFT);
    bool currRight = I->KeyPressed(SDL_SCANCODE_RIGHT);
    bool currZ = I->KeyPressed(SDL_SCANCODE_Z);
    bool currX = I->KeyPressed(SDL_SCANCODE_X);
    bool currSpace = I->KeyPressed(SDL_SCANCODE_SPACE);
    
    if(currLeft && !prevLeft) Move(-1);
    if(currRight && !prevRight) Move(1);
    if(currZ && !prevZ) Rotate(-1);
    if(currX && !prevX) Rotate(1);
    if(currSpace && !prevSpace) HardDrop();
    
    prevLeft = currLeft;
    prevRight = currRight;
    prevZ = currZ;
    prevX = currX;
    prevSpace = currSpace;
    
    SpeedUp(I->KeyDown(SDL_SCANCODE_DOWN));
    
    // Automatisches Fallen
    static const float fixedTimeStep = 1.0f / 60.0f;
    fallTimer += fixedTimeStep;
    if(fallTimer >= fallPeriod) {
        fallTimer = 0;
        DropOne();
    }
}