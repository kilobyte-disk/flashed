#ifndef HLDIALOG_H
#define HLDIALOG_H

/* Quick and dirty fix for string processing,
 * predefine maxmum length.
 * TODO: Figure out how to do this with dynamic strings.
 */
#define MAX_TEXT_LENGTH 200

#include "Helium.h"
#include "HlEase.h"

struct HlDialog {
	/* Reference to Helium Core */
	struct HlCore *CORE;

	enum HlFont font;

	/* Size and positioning (2D) */
	Vector2 position;
	Vector2 size;

	/* Alpha shader */
	Shader alpha_shader;
	float alpha;

	bool visible;

	/* Tweening */
	bool tween_playing;
	enum HlEasingStyle easing_style;

	Vector2 start_position;
	Vector2 start_size;
	float start_alpha;

	Vector2 end_position;
	Vector2 end_size;
	float end_alpha;

	/* Text Animation */
	bool text_playing;
	char *text;
	int text_len;
	int char_index;
	float text_size;
	float text_speed;

	/* Shared between text animations and tween */
	float elapsed_time;
	float total_time;
};

extern struct HlDialog HLDIALOG_Init(struct HlCore *CORE, enum HlFont font);

extern void HLDIALOG_DeInit(struct HlDialog *dialog);

extern void HLDIALOG_SetAlpha(struct HlDialog *dialog, float alpha);

extern int HLDIALOG_AnimateText(struct HlDialog *dialog, float speed, float size, bool skippable, char *text);

extern int HLDIALOG_PlayShowTween(struct HlDialog *dialog, float time);

extern int HLDIALOG_PlayHideTween(struct HlDialog *dialog, float time);

extern void HLDIALOG_Update(struct HlDialog *dialog, float delta_time);

extern void HLDIALOG_Render(struct HlDialog *dialog, float delta_time);

#endif
