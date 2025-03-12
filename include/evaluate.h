#ifndef EVALUATE_H
#define EVALUATE_H

#include "position.h"

/* the evaluation function tries to determine the value of the current       */
/* position for the current player. the greater the value, the better the    */
/* position is for the current player. our basic implementation just sums    */
/* the value of all pieces on the board. knights are worth more than pawns,  */
/* rooks are worth more than knights, and so on. the evaluation function is  */
/* very important, because all moves the engine makes are based on the value */
/* assigned to them by this function. you will find some common improvements */
/* listed below, but you are strongly encouraged to come up with your own    */
/* ideas. if you need some inspiration, play a game of chess and take a look */
/* at one of the positions. who's winning? and, why? this function returns   */
/* a positive value when the current player is winning, and a negative value */
/* when the opponent is winning.                                             */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: piece square tables                                 *//*DONE*/
/* improve the evaluation to also take into account the position of pieces   */
/* on the board. for example, knights can reach more squares from the center */
/* of the board, and pawns are more valuable when they are close to          */
/* promoting. you can also vary this depending on the phase of the game. for */
/* example, at the start of the game, the king wants to castle and hide in a */
/* corner behind the other pieces, but when there are few other pieces left  */
/* the king should become more active.                                       */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: common patterns                                     *//* DOING */
/* pawns like to be connected in chains so they can defend each other. rooks */
/* like to be placed on open files, and bishops like to be placed on open    */
/* diagonals. other common patterns include pins, forks, and batteries. add  */
/* some logic to your evaluation function to detect these patterns and       */
/* change the value of the position accordingly.                             */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: mobility score                                      */ /* DOING */
/* the mobility score is a measure of how many moves a player can make. if a */
/* player has many possible moves, they probably have a strong position.     */
/*                                                                           */
/* https://www.chessprogramming.org/Evaluation                               */
/* https://www.chessprogramming.org/Piece-Square_Tables                      */
/* https://www.chessprogramming.org/Mobility                                 */
int evaluate(const struct position *pos);

#endif

/*
 8  56 57 58 59 60 61 62 63
 7  48 49 50 51 52 53 54 55
 6  40 41 42 43 44 45 46 47
 5  32 33 34 35 36 37 38 39  <-- e5 is here (35)
 4  24 25 26 27 28 29 30 31
 3  16 17 18 19 20 21 22 23
 2   8  9 10 11 12 13 14 15
 1   0  1  2  3  4  5  6  7
    a  b  c  d  e  f  g  h
*/
