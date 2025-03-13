#include "evaluate.h"
#include "types.h"
#include "generate.h"
#include <stdbool.h>

static const int piece_value[6] = { 100, 300, 300, 500, 900, 1000000 };
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
static const int knight_bonus[64] = {
									-5, -5, -5, -5, -5, -5, -5, -5,
									-3,  0,  2,  3,  3,  2,  0, -3,
									-3,  2,  4,  5,  5,  4,  2, -3,
									-3,  3,  5,  7,  7,  5,  3, -3,
									-3,  3,  5,  7,  7,  5,  3, -3,
									-3,  2,  4,  5,  5,  4,  2, -3,
									-3,  0,  2,  3,  3,  2,  0, -3,
									-5, -5, -5, -5, -5, -5, -5, -5};
 static const int knight_penalty[64] = {
									-10, -5,  0,  0,  0,  0, -5, -10,
									-5,   0,  0,  0,  0,  0,  0,  -5,
									0,   0,  0,  0,  0,  0,  0,   0,
									0,   0,  0,  0,  0,  0,  0,   0,
									0,   0,  0,  0,  0,  0,  0,   0,
									0,   0,  0,  0,  0,  0,  0,   0,
									-5,   0,  0,  0,  0,  0,  0,  -5,
									-10, -5,  0,  0,  0,  0, -5, -10};


/*@brief ROW-> (square / 8) square = 56 → rank = 7*/
/*	COL-> (square % 8) square = 59 → file = 3*/
/* @param square */
/* @return Flip the rank (7 - rank). new square=(flipped rank∗8)+file */
int mirror(int square) {
    int rank = square / 8;
    int file = square % 8;
    return (7 - rank) * 8 + file;
}

/*
How to Improve Evaluation Function

    Pawn Chains
        *Add a bonus for connected pawns.
			-To check for connected pawns, you need to:
			Loop through all squares and find pawns.
			Check if the pawn has a friendly pawn diagonally left or right.
			Give a bonus if connected, and a penalty if isolated.
        *Add a penalty for isolated or doubled pawns.

    Rook Placement
       	*Add a bonus for rooks on open or semi-open files.
    	*Add a bonus if two rooks are connected.

    Bishop Activity
        *Add a penalty for bishops trapped by their own pawns.
        *Add a bonus for fianchetto bishops.

    Tactics (Pins, Forks, Batteries)
        *Detect if a piece is pinned and penalize that side.
        *Detect forks and add a bonus for the attacker.
        *Detect batteries (e.g., Queen and Bishop on b2-g7) and increase evaluation.
*/

 /*Returns 1 if there is a friendly pawn diagonally adjacent*/
int is_pawn_connected(const struct position *pos, int square, int color) {

    int file = square % 8;
	int mult = 1;

    if (file > 0) {
        int left_diagonal; /* -9 for white, +7 for black*/
		if (color == WHITE)
		{
			left_diagonal = square - 9;
			if (pos->board[left_diagonal] == WHITE)
				return 1;
		}

		else if (color == BLACK)
		{
			left_diagonal = square + 7;
			if (pos->board[left_diagonal] == BLACK)
				return -10;
		}

    }
	if (file < 7) {
        int right_diagonal;  /* -7 for white, +9 for black */
		if (color == WHITE)
		{
			right_diagonal = square - 7;
			if (pos->board[right_diagonal] == WHITE)
				return 10;
			mult = -1;
		}
		else if (color == BLACK )
		{
			right_diagonal = square + 9;
			if (pos->board[right_diagonal] == BLACK)
				return -10;
		}
    }
    return 15 * mult;
}

/*Doubled pawns → -10 points per extra pawn on a file.*/
int	pawn_doubled_or_isolated(const struct position *pos)
{
	int pawn_count[8] = {0};
	int file = 0;
	int square;
	int mult = 1;
	int score = 0;
	int check_left = 0;
	int check_right = 0;

	for (square = 0; square < 64; square++) {
		if (pos->board[square] == PAWN)
		{
			file = FILE(square);
			pawn_count[file]++;
		}
	}
	file = 0;
	while (file < 8)
	{
		if (pawn_count[file] > 1)
			score = (-10 * pawn_count[file]);
		if (pawn_count[file] > 0)
		{
			if(file > 0)
			{
				if (pawn_count[file - 1] > 0)
					check_left = 1;
				else
					check_left = 0;
			}
			if (file < 7)
			{
				if (pawn_count[file + 1] > 0)
					check_left = 1;
				else
					check_left = 0;
			}
			if (check_right == 0 && check_right == 0)
				score -= 15;
		}
		file++;
    }
	if (pos->side_to_move == BLACK)
		mult = -1;
	return score * mult;
}
/*
Diagonally adjacent ->
Doubled pawns → -10 points per extra pawn on a file.
Isolated pawns → -15 points if no adjacent pawns.
*/
int evaluate_pawn(const struct position *pos, int color) {
    int score = 0;
	int square;

    for (square = 0; square < 64; square++) {
		score += is_pawn_connected(pos, square, color) + pawn_doubled_or_isolated(pos);
    }
    return score;
}

int	common_pattern(const struct position *pos, int piece, int color)
{
	switch (piece)
	{
		case PAWN:
			return evaluate_pawn(pos, color);
			break;
		case QUEEN:
			break;
		case ROOK:
			break;
		case KING:
			break;
		case BISHOP:
			break;
	}
	return 0;
}

/*(pawn, knight, bishop, rook, queen, king) */
int evaluate(const struct position *pos) {
	int score[2] = { 0, 0 };
	int square;
	int square_val = 0;
	struct move moves[MAX_MOVES];
	int count = 0;
	int j = 0;
	int white_mobility = 0;
	int black_mobility = 0;

	int piece = 0;

	count = generate_legal_moves(pos, moves);
	while (j < count)
	{
		if (COLOR(pos->board[moves[j].from_square]) == WHITE) {
			white_mobility++;
		} else {
			black_mobility++;
		}
		j++;
	}
	for (square = 0; square < 64; square++) {
		piece = pos->board[square];

		if (COLOR(piece) == 0)
			square_val = square;
		else
			square_val =  mirror(square);

		switch (piece)
		{
			case NO_PIECE:
				score[COLOR(piece)] += piece_value[TYPE(piece)];
				break;
			case PAWN:
				score[COLOR(piece)] += piece_value[PAWN] + pawn_table[square_val] + common_pattern(pos, PAWN, pos->board[square]);
				break;
			case QUEEN:
				score[COLOR(piece)] += piece_value[QUEEN] + queen_table[square_val];
				break;
			case ROOK:
				score[COLOR(piece)] += piece_value[ROOK] + rook_table[square_val];
				break;
			case KNIGHT:
				score[COLOR(piece)] += piece_value[KNIGHT] + knight_table[square_val] + knight_bonus[square_val] + knight_penalty[square];
				break;
			case KING:
				score[COLOR(piece)] += piece_value[KING] + king_table[square_val];
				break;
			case BISHOP:
				score[COLOR(piece)] += piece_value[BISHOP] + bishop_table[square_val];
				break;
		}
	}
	score[WHITE] += white_mobility * 5;
	score[BLACK] += black_mobility * 5;
	return score[pos->side_to_move] - score[1 - pos->side_to_move];
}
