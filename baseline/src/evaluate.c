#include "evaluate.h"
#include "types.h"

static const int piece_value[6] = { 100, 300, 300, 500, 15000, 1000000 };
static const int rook_table [64] = {0,   0,   5,  10,  10,   5,   0,   0,
									0,   0,   5,  10,  10,   5,   0,   0,
									-5,   0,   5,  10,  10,   5,   0,  -5,
									-5,   0,   5,  10,  10,   5,   0,  -5,
									-5,   0,   5,  10,  10,   5,   0,  -5,
									-5,   0,   5,  10,  10,   5,   0,  -5,
									5,  10,  10,  10,  10,  10,  10,   5,
									0,   0,   5,  15,  15,   5,   0,   0 };
static const int knight_table[64] = {-50, -40, -30, -30, -30, -30, -40, -50,
									-40, -20,   0,   5,   5,   0, -20, -40,
									-30,   5,  10,  15,  15,  10,   5, -30,
									-30,   0,  15,  20,  20,  15,   0, -30,
									-30,   5,  15,  20,  20,  15,   5, -30,
									-30,   0,  10,  15,  15,  10,   0, -30,
									-40, -20,   0,   0,   0,   0, -20, -40,
									-50, -40, -30, -30, -30, -30, -40, -50};
static const int pawn_table[64] = {  0,   0,   0,   0,   0,   0,   0,   0,
									50,  50,  50,  50,  50,  50,  50,  50,
									10,  10,  20,  30,  30,  20,  10,  10,
									5,   5,  10,  25,  25,  10,   5,   5,
									0,   0,   0,  20,  20,   0,   0,   0,
									5,  -5, -10,   0,   0, -10,  -5,   5,
									5,  10,  10, -20, -20,  10,  10,   5,
									0,   0,   0,   0,   0,   0,   0,   0};
static const int queen_table[64] = {-20, -10, -10,  -5,  -5, -10, -10, -20,
									-10,   0,   5,   0,   0,   0,   0, -10,
									-10,   5,   5,   5,   5,   5,   0, -10,
									-5,   0,   5,   5,   5,   5,   0,  -5,
									0,   0,   5,   5,   5,   5,   0,  -5,
									-10,   0,   5,   5,   5,   5,   0, -10,
									-10,   5,   0,   0,   0,   0,   0, -10,
									-20, -10, -10,  -5,  -5, -10, -10, -20};
static const int king_table[64] = {-50, -40, -30, -20, -20, -30, -40, -50,
									-30, -20, -10,   0,   0, -10, -20, -30,
									-30, -10,  20,  30,  30,  20, -10, -30,
									-30, -10,  30,  40,  40,  30, -10, -30,
									-30, -10,  30,  40,  40,  30, -10, -30,
									-30, -10,  20,  30,  30,  20, -10, -30,
									-30, -30,   0,   0,   0,   0, -30, -30,
									-50, -30, -30, -30, -30, -30, -30, -50};
static const int bishop_table[64] = {-20, -10, -10, -10, -10, -10, -10, -20,
									-10,   5,   0,   0,   0,   0,   5, -10,
									-10,  10,  10,  10,  10,  10,  10, -10,
									-10,   0,  10,  10,  10,  10,   0, -10,
									-10,   5,   5,  10,  10,   5,   5, -10,
									-10,   0,   5,  10,  10,   5,   0, -10,
									-10,   0,   0,   0,   0,   0,   0, -10,
									-20, -10, -10, -10, -10, -10, -10, -20};

int mirror(int square) {
    int rank = square / 8;
    int file = square % 8;
    return (7 - rank) * 8 + file;
}

int evaluate(const struct position *pos) {
    int score[2] = { 0, 0 }; // score[0] for white, score[1] for black
    int square;
    int piece = 0;
    int square_val = 0;

    for (square = 0; square < 64; square++) {
        piece = pos->board[square];

        if (piece != NO_PIECE) {
            // Determine the square value based on the piece color
            if (COLOR(piece) == 0) { // White piece
                square_val = square;
            } else { // Black piece
                square_val = mirror(square);
            }

            // Add piece value
            score[COLOR(piece)] += piece_value[TYPE(piece)];

            // Add PST value based on piece type
            if (TYPE(piece) == PAWN)
                score[COLOR(piece)] += pawn_table[square_val];
            else if (TYPE(piece) == KNIGHT)
                score[COLOR(piece)] += knight_table[square_val];
            else if (TYPE(piece) == BISHOP)
                score[COLOR(piece)] += bishop_table[square_val];
            else if (TYPE(piece) == ROOK)
                score[COLOR(piece)] += rook_table[square_val];
            else if (TYPE(piece) == QUEEN)
                score[COLOR(piece)] += queen_table[square_val];
            else if (TYPE(piece) == KING)
                score[COLOR(piece)] += king_table[square_val];
        }
    }

    if (pos->side_to_move == 0) { // White's turn
        return score[0] - score[1];
    } else { // Black's turn
        return score[1] - score[0];
    }
}
