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
#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

#include "Helium.h"
#include "GLOBALS.h"
#include "HlData.h"
#include "HlEase.h"

enum CardAnim {
	FRONT,
	BACK,
	MOVE_FORWARD,
	MOVE_BACKWARD,
};

struct Data {
	struct HlDeck deck;
	long int card_index;

	int card_width;
	int card_height;

	RenderTexture2D temp_inverted;
	RenderTexture2D front;
	RenderTexture2D back;

	bool anim_playing;
	bool view_front;
	enum CardAnim anim;
	float ease_duration;
	float ease_timer;

	float card_x_offset;
	float card_rotation;
	float cam_z;

	Model q_front;
	Model q_back;

	Shader flashcard_shader;
};


struct Data FlashState;

static void PreRenderCard(struct HlCore *CORE, float delta_time)
{
	/* Get card data */
	struct HlDeckCard card = FlashState.deck.cards[FlashState.card_index];

	/* Pre-render card */

	/* TODO: Fix the unoptimized flipping of the RenderTexture */

	Rectangle tex_rec = { 0.0f, 0.0f, (float) FlashState.card_width, (float) FlashState.card_height };
	Vector2 vec2_0 = { 0.0f, 0.0f };
	Vector2 text_pos = { 10.0f, 350.0f };

	/* Render front texture to inverted_temp */
	BeginTextureMode(FlashState.temp_inverted);
	ClearBackground(RAYWHITE);

	DrawTextEx(
		*CORE->GLOBALS->F_VT323,
		card.front,
		text_pos,
		70,
		0.0f,
		BLACK
	);
	
	EndTextureMode();

	/* Render inverted_temp to front texture2d */
	BeginTextureMode(FlashState.front);
	ClearBackground(BLANK);

	DrawTextureRec(FlashState.temp_inverted.texture, tex_rec, vec2_0, WHITE);

	EndTextureMode();

	/* Render back texture */
	BeginTextureMode(FlashState.temp_inverted);
	ClearBackground(LIGHTGRAY);

	DrawTextEx(
		*CORE->GLOBALS->F_VT323,
		card.back,
		text_pos,
		70,
		0.0f,
		BLACK
	);


	EndTextureMode();

	/* Render inverted_temp to back texture2d */
	BeginTextureMode(FlashState.back);
	ClearBackground(BLANK);

	DrawTextureRec(FlashState.temp_inverted.texture, tex_rec, vec2_0, WHITE);

	EndTextureMode();


	FlashState.q_front.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = FlashState.front.texture;
	FlashState.q_back.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = FlashState.back.texture;
}


/*
 * Initialize the state
 */

int FLASHSTATE_Init(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[FlashState]: Entering FLASHSTATE\n");

	/* Init */
	FlashState.view_front = true;
	FlashState.anim = FRONT;
	FlashState.anim_playing = false;
	FlashState.ease_duration = 1.0f;
	FlashState.ease_timer = 0.0f;
	FlashState.card_x_offset = 0.0f;
	FlashState.card_rotation = 0.0f;
	FlashState.card_index = 0;

	FlashState.cam_z = 10.0f;
	CORE->camera3d.position = (Vector3) { 0.0f, 0.0f, FlashState.cam_z };

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





static void PlayAnim(enum CardAnim anim, float duration, float delta_time)
{
	if (FlashState.anim_playing) {
		return;
	}

	FlashState.anim_playing = true;
	FlashState.ease_timer = -delta_time;
	FlashState.ease_duration = duration;
	FlashState.anim = anim;
}

static void AdvanceCards(int advance)
{
	FlashState.card_index += advance;

	if (FlashState.card_index < 0) {
		FlashState.card_index = (FlashState.deck.elements - 1);
	}
	
	if (FlashState.card_index > (FlashState.deck.elements - 1)) {
		FlashState.card_index = 0;
	}
}

/*
 * Update
 * NOTE: This runs BEFORE BeginDraw(); is called.
 */
int FLASHSTATE_Update(struct HlCore *CORE, float delta_time)
{
	/* Update values */

	if ((IsKeyPressed(32) || IsKeyPressed(264) || IsKeyPressed(265)) && !FlashState.anim_playing) {
		FlashState.view_front = !FlashState.view_front;

		if (FlashState.view_front) {
			PlayAnim(FRONT, 1.0f, delta_time);
		} else {
			PlayAnim(BACK, 1.0f, delta_time);
		}
	}

	if (IsKeyPressed(263) && !FlashState.anim_playing) {
		PlayAnim(MOVE_BACKWARD, 0.5f, delta_time);
		AdvanceCards(-1);
	}

	if (IsKeyPressed(262) && !FlashState.anim_playing) {
		PlayAnim(MOVE_FORWARD, 0.5f, delta_time);
		AdvanceCards(1);
	}

	if (FlashState.anim_playing) {
		FlashState.ease_timer += delta_time;

		if (FlashState.ease_timer >= FlashState.ease_duration) {
			FlashState.ease_timer = FlashState.ease_duration;
			FlashState.anim_playing = false;
		}

		switch (FlashState.anim) {
		default:
			break;
		case FRONT:
			/* Flip to front of card */
			FlashState.card_rotation = EASE_ElasticOut(
							FlashState.ease_timer,
							180.0f,
							-180.0f,
							FlashState.ease_duration,
							0.25f, /* amplitude */
							FlashState.ease_duration * 0.35 /* period */
							);

			FlashState.cam_z = EASE_QuadOut(
						FlashState.ease_timer,
						9.2f,
						0.8f,
						FlashState.ease_duration
						);
			break;
		case BACK:
			/* Flip to back of card */
			FlashState.card_rotation = EASE_ElasticOut(
							FlashState.ease_timer,
							0.0f,
							180.0f,
							FlashState.ease_duration,
							0.25f, /* amplitude */
							FlashState.ease_duration * 0.35 /* period */
							);

			FlashState.cam_z = EASE_QuadOut(
						FlashState.ease_timer,
						10.0f,
						-0.8f,
						FlashState.ease_duration
						);
			break;
		case MOVE_FORWARD:	
			/* Jerk card to the right */
			FlashState.card_x_offset = EASE_ElasticOut(
							FlashState.ease_timer,
							2.0f,
							-2.0f,
							FlashState.ease_duration,
							0.05f, /* amplitude */
							FlashState.ease_duration * 0.5 /* period */
							);
			break;
		case MOVE_BACKWARD:
			/* Jerk card to the right */
			FlashState.card_x_offset = EASE_ElasticOut(
							FlashState.ease_timer,
							-2.0f,
							2.0f,
							FlashState.ease_duration,
							0.05f, /* amplitude */
							FlashState.ease_duration * 0.5 /* period */
							);		
			break;
		}
	}

	CORE->camera3d.position = (Vector3) { 0.0f, 0.0f, FlashState.cam_z };
		
	PreRenderCard(CORE, delta_time);

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

	Vector3 front = { FlashState.card_x_offset, 0.0f, -0.1f };
	Vector3 back = { FlashState.card_x_offset, 0.0f, 0.1f };
	Vector3 rotation_axis = { 1.0f, 0.0f, 0.0f };
	Vector3 scale = { 1.0f, 1.0f, 1.0f };

	DrawModelEx(FlashState.q_front, front, rotation_axis, FlashState.card_rotation, scale, WHITE);
	DrawModelEx(FlashState.q_back, back, rotation_axis, FlashState.card_rotation + 180.0f, scale, WHITE);
	
	
	EndMode3D();

	/* 2D rendering */
	Vector2 pos = { (CORE->canvas_width / 2) - 40.0f, CORE->canvas_height - 100.0f };

	char counter[20];
	sprintf(counter, "(%li/%li)", (FlashState.card_index + 1), FlashState.deck.elements);
	DrawTextEx(
		*CORE->GLOBALS->F_VT323,
		counter,
		pos,
		40,
		0.0f,
		WHITE
		);
		
	return 0;
}
