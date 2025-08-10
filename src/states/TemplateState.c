/*
 * kilobyte-disk 2025
 * TemplateState.c
 *
 * Purpose:
 * A Template state used as a standard for creating other states.
 */

#include <stdio.h>
#include "raylib.h"

#include "Helium.h"
#include "GLOBALS.h"

struct Data {
	int myint;
};

struct Data TemplateState;


/*
 * Initialize the state
 */

int TEMPLATESTATE_Init(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[TemplateState]: Entering TEMPLATESTATE\n");

	return 0;
}



/*
 * Cleanup the state
 */

int TEMPLATESTATE_DeInit(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[TemplateState]: Exiting TEMPLATESTATE\n");

	return 0;
}



/*
 * Update
 * NOTE: This runs BEFORE BeginDraw(); is called.
 */
int TEMPLATESTATE_Update(struct HlCore *CORE, float delta_time)
{
	/* Update values */

	return 0;
}



/*
 * Render
 * This is where things should be rendered.
 */
int TEMPLATESTATE_Render(struct HlCore *CORE, float delta_time)
{
	/* Render things */
	DrawText("TemplateState", (CORE->render_width / 2) - 100, (CORE->render_height / 2), 50, BLACK);

	return 0;
}
