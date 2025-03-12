#include "uci.h"
#include "search.h"
#include "move.h"
#include "types.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

char	*opening_move_white( const struct position *pos, int counter){
	const char *opening_moves[] = {
        "e2e4p",
        "g1f3p",
        "f1c4p" 
    };
	int i = 0;

	while (i != counter && (i < 3))
		i++;
	if (counter == i)
	{
		if (counter == 1 && pos->board[35] != NO_PIECE)
			return (opening_moves[counter]);
		if (counter == 2 && pos->board[42] != NO_PIECE)
			return (opening_moves[counter]);
	}
	return (NULL);
}

char *opening_move_black(const struct position *pos, int *counter){
	int line = -1;
	static char *move[][4] =
	{{"f2f4", "Nb1c3", "g2g3", "Bf1g2"},
	{"Ng1f3", "e2e3", "Bc1g5", "d2d4"}};

/*First line Hedgehog*/
	for (int i = 16; i <= 23; i++)
	{
		if (pos->board[i] != NO_PIECE)
		{
			line = 0;
			break ;
		}
	}
/*Second line Nimzo*/
	if (line == -1)
	{
		for (int i = 24; i <= 31; i++)
		{
			if (pos->board[i] != NO_PIECE)
			{
				line = 1;
				break ;
			}
		}
	}
	if (line != -1 && *counter < 4)
		return (move[line][(*counter)++]);
	return (NULL);

}
