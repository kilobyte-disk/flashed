/*
 * kilobyte-disk 2025
 * Helium.c
 *
 * Purpose:
 * Engine core management
 */

#include <string.h>
#include "raylib.h"

#include "Helium.h"

const char STATE_NAMES[5][30] = {
	/* Built-in engine states */
	"HL_ERROR",
	"HL_LOADING",

	/* Custom states */
	"TEMPLATESTATE",

	"MENUSTATE",
	"FLASHSTATE",
};


/* Helium utils */

void HELIUM_ChangeRenderResolution(struct HlCore *CORE, int width, int height)
{
	if (CORE->render_target.texture.id > 0) {
		UnloadTexture(CORE->render_target.texture);
	}

	CORE->render_width = width;
	CORE->render_height = height;

	CORE->render_target = LoadRenderTexture(width, height);
	SetTextureFilter(CORE->render_target.texture, TEXTURE_FILTER_BILINEAR);
}

void HELIUM_ChangeFPSCap(struct HlCore *CORE, int fps)
{
	CORE->target_fps = fps;
	SetTargetFPS(fps);
}

void HELIUM_ChangeState(struct HlCore *CORE, enum HlEngineStates state)
{
	/* TODO: Check that the enum passed is VALID. */

	CORE->state = state;
}

/*
 * Returns the scale factors on each axis that dilate coordinates
 * on the canvas to the virtual render target.
 */
Vector2 HELIUM_GetScreenScale(struct HlCore *CORE)
{
	float scale_x = ((float) CORE->render_width / CORE->canvas_width);
	float scale_y = ((float) CORE->render_height / CORE->canvas_height);

	return (Vector2) { scale_x, scale_y };
}

/*
 * Returns the average of both factors from HELIUM_GetScreenScale()
 */
float HELIUM_GetAverageScreenScale(struct HlCore *CORE)
{
	Vector2 scale = HELIUM_GetScreenScale(CORE);

	return (scale.x + scale.y) * 0.5f;
}

