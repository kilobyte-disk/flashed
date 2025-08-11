/*
 * kilobyte-disk 2025
 * main.c
 *
 * Purpose:
 * The main program
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "rlgl.h"

#include "Helium.h"
#include "GLOBALS.h"
#include "HlSprite.h"
#include "HlAnimation.h"
#include "HlData.h"
#include "HlUtils.h"

/* Default settings */
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_CANVAS_WIDTH 1600
#define DEFAULT_CANVAS_HEIGHT 900
#define DEFAULT_FPS 60

/* Macros */
#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

/* Common 16:9 resolutions to use for rendering. */
const int RESOLUTIONS[4][2] = {
	{ 1280, 720 }, /* 720p */
	{ 1920, 1080 }, /* 1080p */
	{ 2560, 1440 }, /* 1440p */
	{ 3840, 2160 } /* 4k (2160p) */
};

/* Declare states */
#include "HlErrorState.h"
#include "HlInitState.h"

#include "TemplateState.h"
#include "MenuState.h"
#include "FlashState.h"

const int (*STATES[20])(struct HlCore *, float) = {
	/* Built in states */
	HLERRORSTATE_Init,
	HLERRORSTATE_DeInit,
	HLERRORSTATE_Update,
	HLERRORSTATE_Render,

	HLINITSTATE_Init,
	HLINITSTATE_DeInit,
	HLINITSTATE_Update,
	HLINITSTATE_Render,

	/* Custom states */
	TEMPLATESTATE_Init,
	TEMPLATESTATE_DeInit,
	TEMPLATESTATE_Update,
	TEMPLATESTATE_Render,

	MENUSTATE_Init,
	MENUSTATE_DeInit,
	MENUSTATE_Update,
	MENUSTATE_Render,

	FLASHSTATE_Init,
	FLASHSTATE_DeInit,
	FLASHSTATE_Update,
	FLASHSTATE_Render,
};


/*
 * Main functions
 */

void MAIN_AutoDetectMonitorCapability(struct HlCore *CORE)
{
	int monitor_id = GetCurrentMonitor();

	int detected_width = GetMonitorWidth(monitor_id);
	int detected_height = GetMonitorHeight(monitor_id);
	int detected_refresh = GetMonitorRefreshRate(monitor_id);

	printf(
		"[Main]: Running on monitor %i, detected resolution: %ix%i\n",
		monitor_id,
		detected_width,
		detected_height
	);

	int i;
	int found_width = DEFAULT_WINDOW_WIDTH;
	int found_height = DEFAULT_WINDOW_HEIGHT;
	for (i = 0; i < 5; i++) {

		bool is_larger_than_detected = 
			(RESOLUTIONS[i][0] >= detected_width)
			&&
			(RESOLUTIONS[i][1] >= detected_height);

		if (is_larger_than_detected) {
			found_width = RESOLUTIONS[i][0];
			found_height = RESOLUTIONS[i][1];

			break;
		}
	}

	printf(
		"[Main]: Auto-detected render resolution: %i, %i\n",
		found_width,
		found_height
	);

	HELIUM_ChangeRenderResolution(CORE, found_width, found_height);

	printf("[Main]: Auto-detected refresh rate: %i\n", detected_refresh);
	HELIUM_ChangeFPSCap(CORE, detected_refresh);
}

void MAIN_DrawOverlay(struct HlCore *CORE)
{
	Vector2 font_pos = { 3.5f, 3.5f };
	DrawTextEx((*CORE->GLOBALS->F_Roboto), "flashed v0.0.1a", font_pos, 30.0f, 0, MAGENTA);
	//DrawText("helium v0.0.1", 5, 5, 25, MAGENTA);

	/* Need to do all of this to get a custom FPS text readout */
	int fps = GetFPS();
	char fps_str[10] = "FPS: ";
	char fps_char[4];

	/* reformat int fps into a string */
	sprintf(fps_char, "%d", fps);
	strcat(fps_str, fps_char);

	font_pos = (Vector2) { 3.5f, 30.0f };
	DrawTextEx((*CORE->GLOBALS->F_Roboto), fps_str, font_pos, 20.0f, 0, MAGENTA);
}

void MAIN_SetupCore(struct HlCore *CORE)
{
	CORE->edit_mode = true;

	CORE->window_width = DEFAULT_WINDOW_WIDTH;
	CORE->window_height = DEFAULT_WINDOW_HEIGHT;

	/* Setup virtual render target */
	MAIN_AutoDetectMonitorCapability(CORE);

	/* Setup 2D canvas */
	CORE->canvas_width = DEFAULT_CANVAS_WIDTH;
	CORE->canvas_height = DEFAULT_CANVAS_HEIGHT;

	/* Mouse position relative to virtual render target */
	CORE->virtual_mouse = (Vector2) { 0.0f, 0.0f };

	Vector2 center = { CORE->canvas_width * 0.5f, CORE->canvas_height * 0.5f };

	Camera2D camera2d = {
        	.target = center,
		.offset = center,
		.rotation = 0.0f,
		.zoom = 1.0f,
	};

	CORE->camera2d = camera2d;

	Camera3D camera3d;
	camera3d.position = (Vector3) { 0.0f, 0.0f, 10.0f };
	camera3d.target = (Vector3) { 0.0f, 0.0f, 0.0f };
	camera3d.up = (Vector3) { 0.0f, 1.0f, 0.0f };
	camera3d.fovy = 45.0f;
	camera3d.projection = CAMERA_PERSPECTIVE;

	CORE->camera3d = camera3d;

	Camera2D overlay_camera2d = {
		.target = center,
		.offset = center,
		.rotation = 0.0f,
		.zoom = 1.0f,	
	};
	CORE->overlay_camera2d = camera2d;


	CORE->fullscreen = false;

	CORE->state = HL_LOADING;
	CORE->last_state = HL_ERROR;
}


/* 
 * Load fonts
 * Currently supporting:
 * 	English (roboto, VT323)
 * 	Korean (noto-sans-kr)
 */
void MAIN_LoadFonts(struct HlCore *CORE, struct GLOBALS *GLOBALS)
{

	int font_size = 192;
	
	/* EN Standard ASCII Latin characters */
	struct HlCodepoints EN_Codepoints = DATA_ReadCodepoints("data/codepoints/EN.hlcp");

	/* KR Every single Korean Unicode character codepoint */
	struct HlCodepoints KR_Codepoints = DATA_ReadCodepoints("data/codepoints/KR.hlcp");
	struct HlCodepoints KR_SyllablesCP = DATA_ReadCodepoints("data/codepoints/KR_Syllables.hlcp");
	struct HlCodepoints KR_JamoCP = DATA_ReadCodepoints("data/codepoints/KR_Jamo.hlcp");
	struct HlCodepoints KR_JamoExtendedACP = DATA_ReadCodepoints("data/codepoints/KR_JamoExtendedA.hlcp");
	struct HlCodepoints KR_JamoExtendedBCP = DATA_ReadCodepoints("data/codepoints/KR_JamoExtendedB.hlcp");

	DATA_AppendCodepoints(&KR_Codepoints, &EN_Codepoints);
	DATA_AppendCodepoints(&KR_Codepoints, &KR_SyllablesCP);
	DATA_AppendCodepoints(&KR_Codepoints, &KR_JamoCP);
	DATA_AppendCodepoints(&KR_Codepoints, &KR_JamoExtendedACP);
	DATA_AppendCodepoints(&KR_Codepoints, &KR_JamoExtendedBCP);

	size_t fontsize = sizeof(Font);
	
	Font data_roboto = LoadFontEx(
		"assets/fonts/Roboto/static/Roboto-Regular.ttf",
		font_size,
		&EN_Codepoints.codepoints[0],
		(unsigned int) EN_Codepoints.count
	);

	Font data_VT323 = LoadFontEx(
		"assets/fonts/VT323/VT323-Regular.ttf",
		font_size,
		&EN_Codepoints.codepoints[0],
		(unsigned int) EN_Codepoints.count
	);

	Font data_notokr = LoadFontEx(
		"assets/fonts/Noto_Sans_KR/static/NotoSansKR-Regular.ttf",
		font_size,
		&KR_Codepoints.codepoints[0],
		(unsigned int) KR_Codepoints.count
	);


	Font *F_Roboto = malloc(fontsize);
	Font *F_VT323 = malloc(fontsize);
	Font *F_NotoKR = malloc(fontsize);

	if (F_Roboto == NULL || F_VT323 == NULL || F_NotoKR == NULL) {
		printf("[Main]: FATAL: malloc fail!\n");
		exit(1);
	}

	memcpy(F_Roboto, &data_roboto, fontsize);
	memcpy(F_VT323, &data_VT323, fontsize);
	memcpy(F_NotoKR, &data_notokr, fontsize);

	GLOBALS->F_Roboto = F_Roboto;
	GLOBALS->F_VT323 = F_VT323;
	GLOBALS->F_NotoKR = F_NotoKR;


	SetTextureFilter(GLOBALS->F_Roboto->texture, TEXTURE_FILTER_ANISOTROPIC_8X);
	SetTextureFilter(GLOBALS->F_VT323->texture, TEXTURE_FILTER_ANISOTROPIC_8X);
	SetTextureFilter(GLOBALS->F_NotoKR->texture, TEXTURE_FILTER_ANISOTROPIC_8X);


	/* Once the texture altases are generated, the codepoints are no longer needed */
	DATA_FreeCodepoints(&EN_Codepoints);

	DATA_FreeCodepoints(&KR_Codepoints);
	DATA_FreeCodepoints(&KR_SyllablesCP);
	DATA_FreeCodepoints(&KR_JamoCP);
	DATA_FreeCodepoints(&KR_JamoExtendedACP);
	DATA_FreeCodepoints(&KR_JamoExtendedBCP);
}




int main()
{
	/*
	 * Raylib setup
	 */

	/* Set OpenGL configuration flags */
	SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

	/* Initialize GLFW and raylib */
	InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Flashed");
	SetTargetFPS(DEFAULT_FPS);

	SetWindowMinSize(1280, 720);

	/* 
	 * Create HlCore instance
	 */

	struct HlCore CORE;
	MAIN_SetupCore(&CORE);

	
	/* 
	 * Create GLOBALS data storage
	 */
	struct GLOBALS GLOBALS;
	CORE.GLOBALS = &GLOBALS;
	MAIN_LoadFonts(&CORE, &GLOBALS);
	

	/*
	 * Main loop
	 */

	while (!WindowShouldClose()) {
		float delta_time = GetFrameTime();
		
		/* Check for state change */
		if (CORE.last_state != CORE.state) {
			printf("[Main]: Changing states... New state: %i\n\n", CORE.state);

			/* Cleanup last state */
			int last_state = (CORE.last_state) * 4; 
			int deinit_result = STATES[last_state + 1](&CORE, delta_time);

			/* Init new state */
			int new_state = (CORE.state) * 4;
			int init_result = STATES[new_state](&CORE, delta_time);

			/* TODO: Transition state? */

			CORE.last_state = CORE.state;
		}

		/*
		 * Update
		 */

		/* Update virtual render target scaling relative to the actual window size */
		CORE.window_width = GetScreenWidth();
		CORE.window_height = GetScreenHeight();

		/* Maintain aspect ratio with scale */
		float scaled_render_width, scaled_render_height;
		float aspect_ratio = (float) CORE.render_width / CORE.render_height;

		/* Scale both window dimensions to aspect ratio and see which one is more efficient to scale by */
		float x_dist = (CORE.window_width - (CORE.window_height * aspect_ratio));
		float y_dist = (CORE.window_height - (CORE.window_width * (1 / aspect_ratio)));

		float mouse_x_offset = 0.0f;
		float mouse_y_offset = 0.0f;

		/* Get smallest difference in size */
		if (x_dist < y_dist) {
			float render_scale = (float) CORE.window_width / CORE.render_width;

			/* Scale the width to match window_width */
			scaled_render_width = CORE.render_width * render_scale;
			/* Apply aspect ratio to the height (reciprocal needed) */
			scaled_render_height = scaled_render_width * (1.0f / aspect_ratio);

			mouse_y_offset = (CORE.window_height - scaled_render_height) / 2.0f;
		} else {
			float render_scale = (float) CORE.window_height / CORE.render_height;

			/* Scale the height to match window_height */
			scaled_render_height = CORE.render_height * render_scale;
			/* Apply aspect ratio to the width */
			scaled_render_width = scaled_render_height * (aspect_ratio);

			mouse_x_offset = (CORE.window_width - scaled_render_width) / 2.0f;
		}

		/* Update mouse */
		Vector2 mouse = GetMousePosition();

		/* Transform mouse from window coordinates to canvas coordinates */
		float mouse_x_scale = ((float) CORE.canvas_width / (scaled_render_width));
		float mouse_y_scale = ((float) CORE.canvas_height / (scaled_render_height));

		CORE.virtual_mouse.x = (mouse.x - mouse_x_offset) * mouse_x_scale;
		CORE.virtual_mouse.y = (mouse.y - mouse_y_offset) * mouse_y_scale;

		/* Clamp mouse between the maxmimum extents of the canvas */
		if (CORE.virtual_mouse.x < 0.0f) {
			CORE.virtual_mouse.x = 0.0f;
		}
		if (CORE.virtual_mouse.y < 0.0f) {
			CORE.virtual_mouse.y = 0.0f;
		}
		if (CORE.virtual_mouse.x > (float) CORE.canvas_width) {
			CORE.virtual_mouse.x = (float) CORE.canvas_width;
		}
		if (CORE.virtual_mouse.y > (float) CORE.canvas_height) {
			CORE.virtual_mouse.y = (float) CORE.canvas_height;
		}


		/* Update state */
		int current_state = (CORE.state) * 4;
		int update_result = STATES[current_state + 2](&CORE, delta_time);

		/*
		 * Render to virtual render target
		 */
		BeginTextureMode(CORE.render_target);
		
		ClearBackground(RAYWHITE);

		/* Run state render */
		int render_result = STATES[current_state + 3](&CORE, delta_time);

		/* Render overlay */
		MAIN_DrawOverlay(&CORE);


		EndTextureMode();


		/*
		 * Render to real window
		 */

		BeginDrawing();

		ClearBackground(BLACK);

		/* Draw virtual screen to the real window */
		Rectangle source = { 0.0f, 0.0f, (float) CORE.render_target.texture.width, (float) -CORE.render_target.texture.height };
		Rectangle dest = { CORE.window_width * 0.5f, CORE.window_height * 0.5f, scaled_render_width, scaled_render_height};
		Vector2 origin = { dest.width * 0.5f, dest.height * 0.5f };

		DrawTexturePro(CORE.render_target.texture, source, dest, origin, 0.0f,  WHITE);



				
		EndDrawing();

		/* Store this frame for the next cycle */
		CORE.last_frame = CORE.render_target;
	}

	/*
	 * Cleanup
	 */

	/* Deinit the last loaded state first */
	int deinit_result = STATES[(CORE.state * 4) + 1](&CORE, 0.0f);

	UnloadTexture(CORE.render_target.texture);

	UnloadFont(*(CORE.GLOBALS->F_Roboto));
	UnloadFont(*(CORE.GLOBALS->F_VT323));
	UnloadFont(*(CORE.GLOBALS->F_NotoKR));

	free(CORE.GLOBALS->F_Roboto);
	free(CORE.GLOBALS->F_VT323);
	free(CORE.GLOBALS->F_NotoKR);

	/* Goodbye! */
	CloseWindow();

	return 0;
}


