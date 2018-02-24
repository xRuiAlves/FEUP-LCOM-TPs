#ifndef _SNAKEY_NIX_H_
#define _SNAKEY_NIX_H_

#include "Game.h"

/** @defgroup SnakeyNix SnakeyNix
 * @{
 * Group containing the program's main function, containing the Game object
 */

 #define FAILED_GAME_INITIALIZATION  1

/**
 * @brief Returns the address of the game "object" that is being used on the program run
 *
 * @return Game* - the address of the game "object" used
 *
 */
Game* getSnakeyNix();

 /** @} end of SnakeyNix */

#endif /* _SNAKEY_NIX_H_ */
