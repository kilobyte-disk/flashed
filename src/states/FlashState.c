/*
 * kilobyte-disk 2025
 * FlashState.c
 *
 * Purpose:
 * The flashcard portion of the program. 
 * Display each card to the user, and allow them
 * to flip the card to see the rear side.
 */

#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#include "Helium.h"
#include "GLOBALS.h"
#include "HlData.h"

struct Data {
	struct HlDeck deck;

	RenderTexture2D front;
	RenderTexture2D back;

	Model cube;
	float cube_rot;

	Model q_front;
	Model q_back;

	Shader alpha_discard;
};

struct Data FlashState;


/*
 * Initialize the state
 */

int FLASHSTATE_Init(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[FlashState]: Entering FLASHSTATE\n");

	/* Init */
	FlashState.cube_rot = 0.0f;


	/* Load flashcards */
	int FILENAME_LIMIT = 100; /* TODO: make loading lib for this */
	char path[FILENAME_LIMIT];

	sprintf(path, "./data/cards/%s", CORE->GLOBALS->CurrentDeck);

	printf("Loading deck %s\n", path);
	
	FlashState.deck = DATA_ReadHlDeck(path);

	/* Load RenderTextures */
	FlashState.front = LoadRenderTexture(1280, 720);
	FlashState.back = LoadRenderTexture(1280, 720);


	/* Generate meshes */
	Mesh m_cube = GenMeshCube(2.0f, 2.0f, 2.0f);
	Model cube = LoadModelFromMesh(m_cube);

	FlashState.cube = cube;

	Mesh m_plane_front = GenMeshPlane(8.0f, 4.0f, 1, 1);
	Model q_front = LoadModelFromMesh(m_plane_front);
	q_front.transform = MatrixRotateX(90 * DEG2RAD);

	FlashState.q_front = q_front;

	/* Load shader */
	Shader alpha_discard = LoadShader(NULL, "assets/shaders/alpha_discard.fs");
	q_front.materials[0].shader = alpha_discard;

	return 0;
}



/*
 * Cleanup the state
 */

int FLASHSTATE_DeInit(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[FlashState]: Exiting FLASHSTATE\n");

	return 0;
}



/*
 * Update
 * NOTE: This runs BEFORE BeginDraw(); is called.
 */
int FLASHSTATE_Update(struct HlCore *CORE, float delta_time)
{
	/* Update values */
	FlashState.cube_rot += 0.1f;

	if (FlashState.cube_rot >= 360.0f) {
		FlashState.cube_rot = 0.0f;
	}

	/* Card pre-render */

	/* Render front texture */
	BeginTextureMode(FlashState.front);
	ClearBackground(RAYWHITE);

	DrawText("side 1 test", 600, 350, 48, BLACK);

	EndTextureMode();

	/* Render back texture */
	BeginTextureMode(FlashState.back);
	ClearBackground(RAYWHITE);

	EndTextureMode();

	/* TODO: Flip the texture so that it is rendered correctly. */
	FlashState.q_front.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = FlashState.front.texture;

	return 0;
}



/*
 * Render
 * This is where things should be rendered.
 */
int FLASHSTATE_Render(struct HlCore *CORE, float delta_time)
{
	
	Color bgcolor = { 66, 72, 82, 255 };
	ClearBackground(bgcolor);

	/* 3D rendering */
	BeginMode3D(CORE->camera3d);

	Vector3 center = { 0.0f, 0.0f, 0.0f };
	Vector3 rotation_axis = { 0.0f, 1.0f, 0.0f };
	Vector3 scale = { 1.0f, 1.0f, 1.0f };

	DrawModelEx(FlashState.q_front, center, rotation_axis, 0.0f, scale, WHITE);
	
	
	EndMode3D();

	/* 2D rendering */
		
	return 0;
}
