#ifndef HLSPRITE_H
#define HLSPRITE_H

#include "HlAnimation.h"
#include "Helium.h"

struct HlSprite {
	/* Reference to HeliumCore */
	struct HlCore *CORE;

	int id;

	/* Size and positioning */
	Vector2 position; /* Position is RELATIVE to CORE.canvas_width, CORE.canvas_height */
	Vector2 size; /* Size is also relative to CORE.canvas_width, CORE.canvas_height */
	float rotation;

	enum HlCameraMode camera_mode;

	/* Texture related values */
	/* Feels familar to FlxSprite.hx! */
	Image image;
	int image_width;
	int image_height;

	Texture2D texture; /* Texture object. Actual texture data is stored in VRAM */

	int frame_width; /* frame width of the cliprect (in pixels) */
	int frame_height; /* frame height of the cliprect (in pixels) */

	Rectangle source_rec; /* Source rectangle (part of the texture to use for drawing) */
	Rectangle dest_rec; /* Destination rectangle (screen rectangle where the texture is drawn) */
	Vector2 origin; /* Origin of the texture (rotation/scale point). Relative to destination rectangle size */
	Vector2 origin_scale; /* Origin, but on a scale of 0-1 as a multiple of the entire image size */

	/* The "true" size of the sprite,
	 * This is used as a basis to define
	 * extra width or height that needs to be added
	 * or subtracted from the sprite
	 * depending on the size of the clipped frame.
	 * For example: you have a spritesheet with animations.
	 * The static animation is 100x100,
	 * and one animation frame is 150x150 due to effects like glow.
	 * The problem is that the sprite will appear smaller
	 * in that one frame, because it is taking that 150x150 and
	 * rendering it in a 100x100 rectangle.
	 * This is rectified by taking the values
	 * of the static frame size and scaling the sprite's size
	 * up when the 150x150 animation frame is being displayed,
	 * to maintain the same size. 
	 * These values default to image_width and image_height.*/
	int spritesheet_true_width;
	int spritesheet_true_height;
	bool scale_with_spritesheet;

	/* Animation */
	bool anim_playing;
	double anim_timer;
	double frame_delta;
	int cur_frame;
	
	struct HlAnimation *cur_anim; /* Pointer to current animation being used */

	/* Physics */
	bool physics;
	bool collisions;

	/* Edit */
	bool editable;
	bool render_handles;
	

	/* Debug */
	bool render_outline;
};

extern struct HlSprite HLSPRITE_Init(struct HlCore *CORE);

extern void HLSPRITE_DeInit(struct HlSprite *sprite);

extern void HLSPRITE_SetTexture(struct HlSprite *sprite, char *path);

extern void HLSPRITE_Render(struct HlSprite *sprite, float delta_time);

/* Animation */
extern void HLSPRITE_ApplyFrame(struct HlSprite *sprite, struct HlAnimation *anim, int frame);

extern void HLSPRITE_SetTrueSizeToFrame(struct HlSprite *sprite, struct HlAnimation *anim, int frame);

extern void HLSPRITE_PlayAnim(struct HlSprite *sprite, struct HlAnimation *anim, bool force);

extern void HLSPRITE_UpdateAnim(struct HlSprite *sprite, float delta_time);

#endif
