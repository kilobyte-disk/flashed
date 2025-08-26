/*
 * kilobyte-disk 2025
 * HlInitState.c
 *
 * Purpose:
 * The first state run, it is used to perform basic setup and
 * hand off control to the first custom state.
 */

#include <stdio.h>
#include "raylib.h"

#include "Helium.h"
#include "GLOBALS.h"

struct Data {
	double timer;
};

struct Data HlInitState;

/*
 * Initialize the state
 */

int HLINITSTATE_Init(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[HlInitState]: Entering HL_INIT state\n");

	HlInitState.timer = 0.0;

	return 0;
}



/*
 * Cleanup the state
 */

int HLINITSTATE_DeInit(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[HlInitState]: Exiting HL_INIT state\n");

	HlInitState.timer = 0.0;

	return 0;
}



/*
 * Update
 * NOTE: This runs BEFORE BeginDraw(); is called.
 */
int HLINITSTATE_Update(struct HlCore *CORE, float delta_time)
{
	/* Update values */
	HlInitState.timer += delta_time;	

	if (HlInitState.timer > 1.0) {
		HELIUM_ChangeState(CORE, MENUSTATE);
	}
	
	return 0;
}



/*
 * Render
 * This is where things should be rendered.
 */
int HLINITSTATE_Render(struct HlCore *CORE, float delta_time)
{
	ClearBackground(BLACK);

	/* Render */
	Vector2 scale = HELIUM_GetScreenScale(CORE);
	float avg_scale = HELIUM_GetAverageScreenScale(CORE);

	DrawText("HL_INIT", ((CORE->canvas_width / 2) - 100) * scale.x, ((CORE->canvas_height / 2) * scale.y), 50 * (int) avg_scale, BLACK);

	return 0;
}
