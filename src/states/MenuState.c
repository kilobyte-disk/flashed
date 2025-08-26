/*
 * kilobyte-disk 2025
 * MENUSTATE.c
 *
 * Purpose:
 * The menu state for selecting files to play.
 */

#include <stdio.h>
#include "raylib.h"

#include "Helium.h"
#include "GLOBALS.h"

struct Data {
	float timer;
};

struct Data MenuState;


/*
 * Initialize the state
 */

int MENUSTATE_Init(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[MENUSTATE]: Entering MENUSTATE\n");

	

	MenuState.timer = 0.0f;
	return 0;
}



/*
 * Cleanup the state
 */

int MENUSTATE_DeInit(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[MENUSTATE]: Exiting MENUSTATE\n");

	return 0;
}



/*
 * Update
 * NOTE: This runs BEFORE BeginDraw(); is called.
 */
int MENUSTATE_Update(struct HlCore *CORE, float delta_time)
{
	/* Update values */

	MenuState.timer += delta_time;

	if (MenuState.timer >= 0.1f) {
		CORE->GLOBALS->CurrentDeck = "test.hdk";
		HELIUM_ChangeState(CORE, FLASHSTATE);
	}

	return 0;
}



/*
 * Render
 * This is where things should be rendered.
 */
int MENUSTATE_Render(struct HlCore *CORE, float delta_time)
{
	/* Render things */

	return 0;
}
