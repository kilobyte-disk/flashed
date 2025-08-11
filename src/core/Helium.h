#ifndef HELIUM_H
#define HELIUM_H

#include "raylib.h"
#include "HlData.h"

enum HlFont {
	ROBOTO,
	VT323,
	NOTO_KR,
};

enum HlEngineStates {
	/* Built-in engine states */
	HL_ERROR,
	HL_LOADING,
	

	/* Custom states */
	TEMPLATESTATE,

	MENUSTATE,
	FLASHSTATE,
};

enum HlCameraMode {
	HL_NONE, /* Be bound by no camera. */
	HL_OVERLAY, /* Be bound by the overlay_camera2d */
	HL_WORLD, /* Be bound by the camera2d */
};

struct HlCore {
	/* Window */

	/* The current window resolution */
	int window_width;
	int window_height;

	/* The resolution of the virtual render target */
	int render_width;
	int render_height;

	/* The width and height of the canvas used to draw sprites. */
	int canvas_width;
	int canvas_height;

	Camera2D overlay_camera2d; /* Camera for GUI overlay */
	
	/* World cameras */
	Camera2D camera2d;
	Camera3D camera3d;

	RenderTexture2D render_target;

	/* render_target in the previous frame */
	RenderTexture2D last_frame; 

	Vector2 virtual_mouse; /* Mouse position in canvas space. */

	bool fullscreen;
	int target_fps;

	/* Engine state */
	bool edit_mode;

	enum HlEngineStates state;
	enum HlEngineStates last_state;

	/* GLOBALS */
	struct GLOBALS *GLOBALS;

	/* Global asset pointers
	Font *F_Roboto; 
	Font *F_VT323;
	Font *F_NotoKR; */

};

extern void HELIUM_ChangeRenderResolution(struct HlCore *CORE, int new_width, int new_height);

extern void HELIUM_ChangeFPSCap(struct HlCore *CORE, int fps);

extern void HELIUM_ChangeState(struct HlCore *CORE, enum HlEngineStates state);

extern Vector2 HELIUM_GetScreenScale(struct HlCore *CORE);

extern float HELIUM_GetAverageScreenScale(struct HlCore *CORE);
#endif
