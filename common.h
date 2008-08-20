#ifndef __COMMON__
#define __COMMON__

#include <cassert>

#define MAX_INPUT_LENGTH 256

typedef int Size;
typedef enum {NOT_PLAYED,PLAYER_1,PLAYER_2} Token;

inline Token other_player(Token player) {
	switch (player) {
	case PLAYER_1:
		return PLAYER_2;
	case PLAYER_2:
		return PLAYER_1;
    default:
		assert(false);
	}
}

typedef enum {NORMAL,LOOSER,WINNER} Mode;
typedef float Value;
typedef int Count;

#endif
