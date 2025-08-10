/*
 * kilobyte-disk 2025
 * HlSprite.c
 *
 * Purpose:
 * A sprite entity helper, to easily create and manage sprites
 * using struct HlSprite.
 */
#include <stdio.h>
#include "raylib.h"

#include "HlSprite.h"
#include "HlAnimation.h"
#include "HlEase.h"

struct HlSprite HLSPRITE_Init(struct HlCore *CORE)
{
	/* Initialize a new sprite */

	struct HlSprite new_sprite;

	/* Reference to CORE */
	new_sprite.CORE = CORE;

	/* Size and positioning */
	Vector2 position = { 0.0f, 0.0f };
	new_sprite.position = position;
	
	Vector2 size = { 100.0f, 100.0f };
	new_sprite.size = size;

	new_sprite.rotation = 0.0f;

	new_sprite.camera_mode = HL_WORLD;

	/* Texture related values */
	new_sprite.image_width = 100;
	new_sprite.image_height = 100;

	new_sprite.frame_width = 100;
	new_sprite.frame_height = 100;

	Rectangle source_rec = {
		0.0f,
		0.0f,
		(float) new_sprite.frame_width,
		(float) new_sprite.frame_height
	};

	new_sprite.source_rec = source_rec;

	Rectangle dest_rec = {
		new_sprite.size.x,
		new_sprite.size.y,
		(float) new_sprite.frame_width,
		(float) new_sprite.frame_height
	};

	new_sprite.dest_rec = dest_rec;

	Vector2 origin = { 0.0f, 0.0f };
	new_sprite.origin = origin;
	new_sprite.origin_scale = origin;

	new_sprite.spritesheet_true_width = new_sprite.image_width;
	new_sprite.spritesheet_true_height = new_sprite.image_height;

	/* Animation */
	new_sprite.scale_with_spritesheet = false;

	/* Physics */
	new_sprite.physics = false;
	new_sprite.collisions = false;

	/* Debug */
	new_sprite.render_outline = false;

	return new_sprite;
}

void HLSPRITE_DeInit(struct HlSprite *sprite)
{
	if (sprite->texture.id > 0) {
		UnloadTexture(sprite->texture);
	}
}


void HLSPRITE_SetTexture(struct HlSprite *sprite, char *path)
{
	/* NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required) */

	sprite->image = LoadImage(path);
	
	int w = sprite->image.width;
	int h = sprite->image.height;

	sprite->frame_width = w;
	sprite->frame_height = h;
	sprite->spritesheet_true_width = w;
	sprite->spritesheet_true_height = h;
	sprite->image_width = w;
	sprite->image_height = h;

	sprite->source_rec.width = (float) w;
	sprite->source_rec.height = (float) h;

	/* Convert the image to a texture, and load to VRAM */
	sprite->texture = LoadTextureFromImage(sprite->image);
	SetTextureFilter(sprite->texture, TEXTURE_FILTER_ANISOTROPIC_8X);

	UnloadImage(sprite->image);
}


void HLSPRITE_Render(struct HlSprite *sprite, float delta_time)
{
	if (sprite->texture.id <= 0) {
		printf("[HlSprite]: (WARNING) Texture does not exist!\n");
		return;
	}

	/* Update position and size before render */
	float width_add = 0.0f;
	float height_add = 0.0f;

	if (sprite->scale_with_spritesheet) {
		float width_scale = (
			(float) sprite->frame_width
			/
			(float) sprite->spritesheet_true_width
		);

		float height_scale = (
			(float) sprite->frame_height
			/
			(float) sprite->spritesheet_true_height
		);
		
		width_add = (width_scale * sprite->size.x) - sprite->size.x;
		height_add = (height_scale * sprite->size.y) - sprite->size.y;
	}

	/* Scale the canvas relative position and size to the true render target resolution */
	float dest_x, dest_y, dest_width, dest_height;

	Vector2 scale = HELIUM_GetScreenScale(sprite->CORE);

	dest_x = (sprite->position.x * scale.x);
	dest_y = (sprite->position.y * scale.y);
	dest_width = (sprite->size.x * scale.x);
	dest_height = (sprite->size.y * scale.y);

	sprite->dest_rec.x = dest_x;
	sprite->dest_rec.y = dest_y;
	sprite->dest_rec.width = dest_width + width_add;
	sprite->dest_rec.height = dest_height + height_add;

	sprite->origin.x = sprite->origin_scale.x * sprite->dest_rec.width;
	sprite->origin.y = sprite->origin_scale.y * sprite->dest_rec.height;

	/* Set camera mode */
	switch (sprite->camera_mode) {
	case HL_WORLD:
		BeginMode2D(sprite->CORE->camera2d);
		break;

	case HL_OVERLAY:
		BeginMode2D(sprite->CORE->overlay_camera2d);
		break;
	}



	DrawTexturePro(
		sprite->texture,
		sprite->source_rec,
		sprite->dest_rec,
		sprite->origin,
		sprite->rotation,
		WHITE
	);

	if (sprite->render_outline) {
		float avg_scale = HELIUM_GetAverageScreenScale(sprite->CORE);

		DrawRectangleLinesEx(sprite->dest_rec, 5.0f * avg_scale, RED);
	}

	if (sprite->camera_mode != HL_NONE) {
		EndMode2D();
	}
}


/* Animation */

void HLSPRITE_ApplyFrame(struct HlSprite *sprite, struct HlAnimation *anim, int frame)
{
	int x, y, width, height;

	x = anim->frame_data[frame][1];
	y = anim->frame_data[frame][2];
	width = anim->frame_data[frame][3];
	height = anim->frame_data[frame][4];

	sprite->frame_width = width;
	sprite->frame_height = height;

	sprite->source_rec.x = (float) x;
	sprite->source_rec.y = (float) y;
	sprite->source_rec.width = (float) width;
	sprite->source_rec.height = (float) height;
}


void HLSPRITE_SetTrueSizeToFrame(struct HlSprite *sprite, struct HlAnimation *anim, int frame)
{
	int width, height;
	
	width = anim->frame_data[frame][3];
	height = anim->frame_data[frame][4];

	sprite->spritesheet_true_width = width;
	sprite->spritesheet_true_height = height;
}


void HLSPRITE_PlayAnim(struct HlSprite *sprite, struct HlAnimation *anim, bool force)
{
	if ((!sprite->anim_playing) && (!force)) {
		/* If not forced, don't interrupt any playing animation */
		return;
	}

	sprite->anim_playing = true;
	sprite->anim_timer = 0.0;
	sprite->cur_frame = 1; /* Start at 1 because the initial frame
				 is already applied */

	sprite->cur_anim = anim;

	sprite->frame_delta = (1 / (double) sprite->cur_anim->frame_rate);

	/* Apply initial frame to the sprite */
	HLSPRITE_ApplyFrame(sprite, anim, 0);

	printf(
		"[HlSprite]: Playing animation, framerate: %i, total frames: %i\n",
		sprite->cur_anim->frame_rate,
		sprite->cur_anim->frames
	);
}

void HLSPRITE_UpdateAnim(struct HlSprite *sprite, float delta_time)
{
	if (!(sprite->anim_playing && (sprite->cur_anim->frames >= 0))) {
		return;
	}

	sprite->anim_timer += (double) delta_time;
	
	if (sprite->anim_timer >= sprite->frame_delta) {

		if (sprite->cur_frame > sprite->cur_anim->frames) {
			sprite->anim_playing = false;
			return;
		}

		HLSPRITE_ApplyFrame(sprite, sprite->cur_anim, sprite->cur_frame);
		
		sprite->cur_frame++;
		sprite->anim_timer = 0;
	}
}

	

