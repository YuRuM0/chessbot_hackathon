#include "search.h"
#include "evaluate.h"
#include "generate.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>

struct search_result minimax(const struct position *pos, int depth, int alpha, int beta) {
	int stage;
	struct search_result result;

	result.score = -1000000;

	if (depth == 0) 
	{
		/* we have reached our search depth, so evaluate the position.       */
		result.score = evaluate(pos);
		return result;
	}
	for (stage = 0; stage < 2; stage++)
	{ /*Check for stage*/
		struct move moves[MAX_MOVES];
		size_t count = generate_legal_moves(pos, moves, stage);
		size_t index;
		for (index = 0; index < count; index++) {
			struct position copy = *pos;
			int score;

			/* do a move, the current player in `copy` is then the opponent, */
			/* and so when we call minimax we get the score of the opponent. */
			do_move(&copy, moves[index]);

			/* minimax is called recursively. this call returns the score of */
			/* negamax inputs -beta, -alpha         */
			score = -minimax(&copy, depth - 1, -beta, -alpha).score;

			/* update the best move if we found a better one.                */
			if (score > beta) /*>=: hard, > fail soft*/
			{
				result.score = score;
				return result;
			}
			if (score > result.score) {
				result.move = moves[index];
				result.score = score;
			}
			if (score > alpha) {
				alpha = score;
			}
		}
	}
	return result;
}

struct move search(const struct search_info *info) {
	int alpha = INT_MIN;
	int beta = INT_MAX;
	int depth = 4;

	/*if (info->time[WHITE] < info->time[BLACK] && we are white)
		depth = 2;*/
	return minimax(info->pos, depth, alpha, beta).move;
}
