/*
 * kilobyte-disk 2025
 * HlErrorState.c
 *
 * Purpose:
 * An internal state used to display errors.
 * TODO: Make it fully functional and aesthetically pleasing.
 */

#include <stdio.h>
#include "raylib.h"

#include "Helium.h"


/*
 * Initialize the state
 */

int HLERRORSTATE_Init(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[HlErrorState]: Entering HL_ERROR state\n");

	return 0;
}



/*
 * Cleanup the state
 */

int HLERRORSTATE_DeInit(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[HlErrorState]: Exiting HL_ERROR state\n");

	return 0;
}



/*
 * Update
 * NOTE: This runs BEFORE BeginDraw(); is called.
 */
int HLERRORSTATE_Update(struct HlCore *CORE, float delta_time)
{
	/* Update values */
	
	return 0;
}



/*
 * Render
 * This is where things should be rendered.
 */
int HLERRORSTATE_Render(struct HlCore *CORE, float delta_time)
{
	ClearBackground(RED);

	/* Render things */
	Vector2 scale = HELIUM_GetScreenScale(CORE);
	float avg_scale = HELIUM_GetAverageScreenScale(CORE);

	DrawText("ERROR", ((CORE->canvas_width / 2) - 100) * scale.x, ((CORE->canvas_height / 2) * scale.y), 50 * (int) avg_scale, WHITE);

	return 0;
}
