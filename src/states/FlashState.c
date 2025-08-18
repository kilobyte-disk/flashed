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

	int card_width;
	int card_height;

	RenderTexture2D temp_inverted;
	RenderTexture2D front;
	RenderTexture2D back;

	Model cube;
	float cube_rot;

	Model q_front;
	Model q_back;

	Shader flashcard_shader;
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
	FlashState.card_width = 1280;
	FlashState.card_height = 720;

	FlashState.temp_inverted = LoadRenderTexture(FlashState.card_width, FlashState.card_height);
	FlashState.front = LoadRenderTexture(FlashState.card_width, FlashState.card_height);
	FlashState.back = LoadRenderTexture(FlashState.card_width, FlashState.card_height);


	/* Generate meshes */
	Mesh m_cube = GenMeshCube(2.0f, 2.0f, 2.0f);
	Model cube = LoadModelFromMesh(m_cube);

	FlashState.cube = cube;

	Mesh m_plane = GenMeshPlane(8.0f, 4.0f, 1, 1);

	Model q_front = LoadModelFromMesh(m_plane);
	Model q_back = LoadModelFromMesh(m_plane);

	q_front.transform = MatrixRotateX(90 * DEG2RAD);
	q_back.transform = MatrixRotateX(90 * DEG2RAD);

	FlashState.q_front = q_front;
	FlashState.q_back = q_back;


	/* Load shader */
	Shader flashcard_shader = LoadShader(NULL, "assets/shaders/flashcard.fs");
	q_front.materials[0].shader = flashcard_shader;

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
	/* TODO: Fix the unoptimized flipping of the RenderTexture */

	Rectangle tex_rec = { 0.0f, 0.0f, (float) FlashState.card_width, (float) FlashState.card_height };
	Vector2 vec2_0 = { 0.0f, 0.0f };

	/* Render front texture to inverted_temp */
	BeginTextureMode(FlashState.temp_inverted);
	ClearBackground(RAYWHITE);

	DrawText("FRONT SIDE", 400, 350, 70, BLACK);

	EndTextureMode();

	/* Render inverted_temp to front texture2d */
	BeginTextureMode(FlashState.front);
	ClearBackground(RAYWHITE);

	DrawTextureRec(FlashState.temp_inverted.texture, tex_rec, vec2_0, WHITE);

	EndTextureMode();

	/* Render back texture */
	BeginTextureMode(FlashState.temp_inverted);
	ClearBackground(RAYWHITE);

	DrawText("BACK SIDE", 400, 350, 70, BLACK);

	EndTextureMode();

	/* Render inverted_temp to back texture2d */
	BeginTextureMode(FlashState.back);
	ClearBackground(RAYWHITE);

	DrawTextureRec(FlashState.temp_inverted.texture, tex_rec, vec2_0, WHITE);

	EndTextureMode();


	FlashState.q_front.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = FlashState.front.texture;
	FlashState.q_back.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = FlashState.back.texture;

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

	Vector3 front = { 0.0f, 0.0f, -0.1f };
	Vector3 back = { 0.0f, 0.0f, 0.1f };
	Vector3 rotation_axis = { 0.0f, 1.0f, 0.0f };
	Vector3 scale = { 1.0f, 1.0f, 1.0f };

	DrawModelEx(FlashState.q_front, front, rotation_axis, FlashState.cube_rot, scale, WHITE);
	DrawModelEx(FlashState.q_back, back, rotation_axis, FlashState.cube_rot + 180.0f, scale, WHITE);
	
	
	EndMode3D();

	/* 2D rendering */
		
	return 0;
}
