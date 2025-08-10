/* 
 * kilobyte-disk 2025
 * GLOBALS.h
 *
 * A global struct that contains the shared
 * variables that each state can access through:
 * CORE->GLOBALS
 * 
 */

#include <stdbool.h>

#include "HlData.h"

#ifndef GLOBALS_H
#define GLOBALS_H

struct GLOBALS {
	char *CurrentDeck;

	/* Fonts */
	Font *F_Roboto;
	Font *F_VT323;
	Font *F_NotoKR;
};

#endif
