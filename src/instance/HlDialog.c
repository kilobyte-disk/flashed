/*
 * kilobyte-disk 2025
 * HlDialog.c
 *
 * Purpose:
 * A dialog instance
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "rlgl.h"

#include "Helium.h"
#include "GLOBALS.h"
#include "HlDialog.h"
#include "HlUtils.h"
#include "HlEase.h"

#define CHAR_TIME 0.05f
#define PUNCTUATION_TIME 0.2f
#define MAX_DIALOG_LINES 15


static float ease(enum HlEasingStyle style, float elapsed_time, float start, float end, float total_time)
{
	float diff = (end - start);

	switch (style) {
	case QUAD_OUT:
		return EASE_QuadOut(elapsed_time, start, diff, total_time);
	case QUAD_IN:
		return EASE_QuadIn(elapsed_time, start, diff, total_time);
	default:
		return EASE_Linear(elapsed_time, start, diff, total_time);
	}
};

struct HlDialog HLDIALOG_Init(struct HlCore *CORE, enum HlFont font)
{
	struct HlDialog dialog;

	dialog.CORE = CORE;
	dialog.font = font;

	dialog.position = (Vector2) { CORE->canvas_width * 0.5f, CORE->canvas_height * 0.85f };
	dialog.size = (Vector2) { CORE->canvas_width * 0.47f, CORE->canvas_height * 0.15f };

	dialog.alpha_shader = LoadShader(0, "assets/shaders/alpha.fs");
	dialog.alpha = 0.0f; /* Defaults to transparent */
	HLDIALOG_SetAlpha(&dialog, 0.0f);

	dialog.visible = false;

	/* Tweening */
	dialog.tween_playing = false;
	dialog.easing_style = LINEAR;

	dialog.start_position = dialog.position;
	dialog.start_size = dialog.size;
	dialog.start_alpha = dialog.alpha;

	dialog.end_position = dialog.position;
	dialog.end_size = dialog.size;
	dialog.end_alpha = dialog.alpha;

	/* Text animations */
	dialog.text_playing = false;
	dialog.text = "\0";

	/* Shared between text animations and tween */
	dialog.elapsed_time = 0.0f;
	dialog.total_time = 0.0f;

	return dialog;
}

void HLDIALOG_DeInit(struct HlDialog *dialog)
{
	UnloadShader(dialog->alpha_shader);
}

void HLDIALOG_SetAlpha(struct HlDialog *dialog, float alpha)
{
	unsigned int a_loc = GetShaderLocation(dialog->alpha_shader, "alpha_mod");
	SetShaderValue(dialog->alpha_shader, a_loc, &alpha, SHADER_UNIFORM_FLOAT);

	dialog->alpha = alpha;
}

char *HLDIALOG_WrapText(struct HlDialog *dialog, char *text)
{
	/* Get screen scale */
	float avg_scale = HELIUM_GetAverageScreenScale(dialog->CORE);

	int max_text_len = dialog->text_len + MAX_DIALOG_LINES;
	char *formatted_text = malloc(sizeof(char) * max_text_len);
	memset(formatted_text, 0, max_text_len); /* Fill buffer with null values */


	int i = 0;
	int word_length = 0;
	int newlines = 0;
	float cur_line_width = 0.0f;

	while (i < dialog->text_len) {
		word_length = 0;

		while (text[i] != '\n' && text[i] != ' ' && text[i] != '\0' && i < dialog->text_len) {
			char char_string[2] = {text[i], '\0'};
			Vector2 char_size = MeasureTextEx(
				*HL_GetFont(dialog->font, dialog->CORE),
				char_string,
				dialog->text_size * avg_scale,
				0.0f
			);

			float char_width = char_size.x;
			
			cur_line_width += char_width;

			i++;
			word_length++;
		}

		if (text[i] == '\n') {
			/* Reset width counter when a newline is encountered */
			cur_line_width = 0.0f;
		}

		int word_start = i - word_length;

		if (cur_line_width >= dialog->size.x) {
			/* Insert newline before the word */
			formatted_text[word_start + newlines] = '\n';
			newlines++;
			
			
			/* Reset line width */
			cur_line_width = 0.0f;
		}

		/* Append word to string */
		int x = 0;
		while (x <= word_length) {
			int pos = word_start + x + newlines;
			
			formatted_text[pos] = text[word_start + x];
			x++;
		}
		
		i++;
	}

	formatted_text[i + newlines - 1] = '\0';

	return formatted_text;
}


int HLDIALOG_AnimateText(struct HlDialog *dialog, float speed, float size, bool skippable, char *text)
{
	if (dialog->tween_playing) {
		printf("[HlDialog]: Cannot play text animation while tweening!\n");
		return 1;
	}

	/* Free old dialog text */
	if (strlen(dialog->text) > 0) {
		free(&dialog->text[0]);
	}

	/* Set variables */

	dialog->text_playing = true;
	dialog->text_len = strlen(text);
	dialog->char_index = 0;
	dialog->elapsed_time = 0.0f;
	dialog->text_speed = speed;
	dialog->text_size = size;

	char *formatted_text = HLDIALOG_WrapText(dialog, text);

		
	dialog->text = &formatted_text[0];
	dialog->text_len = strlen(formatted_text);

	return 0;
}

int HLDIALOG_PlayShowTween(struct HlDialog *dialog, float time)
{
	if (dialog->visible) {
		return 1;
	}
	if (dialog->text_playing) {
		printf("[HlDialog]: Cannot play tween while text is playing!\n");
		return 1;
	}

	dialog->tween_playing = true;
	dialog->easing_style = QUAD_OUT;

	dialog->elapsed_time = 0.0f;
	dialog->total_time = time;

	dialog->start_position = dialog->position;
	dialog->start_size = dialog->size;
	dialog->start_alpha = dialog->alpha;

	dialog->end_position = (Vector2) { dialog->CORE->canvas_width * 0.5f, dialog->CORE->canvas_height * 0.8f };
	dialog->end_size = (Vector2) { dialog->CORE->canvas_width * 0.5f, dialog->CORE->canvas_height * 0.18f };
	dialog->end_alpha = 0.9f;

	dialog->visible = true;
}

int HLDIALOG_PlayHideTween(struct HlDialog *dialog, float time)
{
	if (!dialog->visible) {
		return 1;
	}
	if (dialog->text_playing) {
		printf("[HlDialog]: Cannot play tween while text is playing!\n");
		return 1;
	}

	dialog->tween_playing = true;
	dialog->easing_style = QUAD_IN;

	dialog->elapsed_time = 0.0f;
	dialog->total_time = time;

	dialog->start_position = dialog->position;
	dialog->start_size = dialog->size;
	dialog->start_alpha = dialog->alpha;

	dialog->end_position = (Vector2) { dialog->CORE->canvas_width * 0.5f, dialog->CORE->canvas_height * 0.85f };
	dialog->end_size = (Vector2) { dialog->CORE->canvas_width * 0.48f, dialog->CORE->canvas_height * 0.15f };
	dialog->end_alpha = 0.0f;

	dialog->visible = false;
}


void HLDIALOG_Update(struct HlDialog *dialog, float delta_time)
{
	if (dialog->tween_playing) {

		/* Update tween */
		Vector2 pos = {
			ease(
				dialog->easing_style,
				dialog->elapsed_time,
				dialog->start_position.x,
				dialog->end_position.x,
				dialog->total_time
			),
			ease(
				dialog->easing_style,
				dialog->elapsed_time,
				dialog->start_position.y,
				dialog->end_position.y,
				dialog->total_time
			)
		};

		Vector2 size = {
			ease(
				dialog->easing_style,
				dialog->elapsed_time,
				dialog->start_size.x,
				dialog->end_size.x,
				dialog->total_time
			),
			ease(
				dialog->easing_style,
				dialog->elapsed_time,
				dialog->start_size.y,
				dialog->end_size.y,
				dialog->total_time
			)
		};

		dialog->position = pos;
		dialog->size = size;
		
		HLDIALOG_SetAlpha(
				dialog,
				HL_Clamp(
					ease(
						dialog->easing_style,
						dialog->elapsed_time,
						dialog->start_alpha,
						dialog->end_alpha,
						dialog->total_time
					),
					0.0f,
					1.0f
				)
		);
		
		if (dialog->elapsed_time >= dialog->total_time) {
			dialog->tween_playing = false;
			
			/* Make sure all values are what they should be */
			dialog->position = dialog->end_position;
			dialog->size = dialog->end_size;
			HLDIALOG_SetAlpha(dialog, dialog->end_alpha);
		}
	} else if (dialog->text_playing) {
		
		/* Update text typewriter animation. */
				
		char cur_char = dialog->text[dialog->char_index];
		float wait_time = CHAR_TIME;

		if (cur_char == '.' || cur_char == ',' || cur_char == '?' || cur_char == '!') {
			wait_time = PUNCTUATION_TIME;
		}

		
		if (dialog->elapsed_time >= wait_time) {
			dialog->elapsed_time = 0.0f;

			dialog->char_index++;
		}

		if (dialog->char_index >= dialog->text_len) {
			dialog->text_playing = false;
		}
	} else {
		/* Don't do anything if both are not running */
		return;
	}

	dialog->elapsed_time += delta_time;
}

void HLDIALOG_Render(struct HlDialog *dialog, float delta_time)
{
	/* Don't bother if the entire thing is invisible */
	if (dialog->alpha == 0.0f) {
		return;
	}

	BeginMode2D(dialog->CORE->overlay_camera2d);
	BeginShaderMode(dialog->alpha_shader);

	/* Scale the canvas relative position and size to the true render target resolution */
	float dest_x, dest_y, dest_width, dest_height;

	Vector2 scale = HELIUM_GetScreenScale(dialog->CORE);
	float avg_scale = HELIUM_GetAverageScreenScale(dialog->CORE);

	float outline_thickness = 5.0f * ((scale.x + scale.y) * 0.5f);

	Rectangle dialog_body = {
		dialog->position.x * scale.x,
		dialog->position.y * scale.y, 
		dialog->size.x * scale.x,
		dialog->size.y * scale.y 
	};

	Rectangle dialog_outline = {
		dialog_body.x - (dialog_body.width * 0.5f),
		dialog_body.y, dialog_body.width, dialog_body.height
	};

	Vector2 body_origin = { dialog_body.width * 0.5f, 0.0f };

	DrawRectanglePro(dialog_body, body_origin, 0.0f, BLACK);
	DrawRectangleLinesEx(dialog_outline, outline_thickness, GRAY);


	/* Render the text */
	Vector2 text_pos = {
		dialog_outline.x + (dialog_body.width * 0.05),
	       	dialog_outline.y + (dialog_body.height * 0.05)
	};

	Vector2 text_origin = { 0.0f, 0.0f };

	/* Lazy-ass solution to character waiting time, draw dialog->char_index + 1. */
	DrawTextPro(
		*HL_GetFont(dialog->font, dialog->CORE),
		TextSubtext(dialog->text, 0, dialog->char_index + 1),
		text_pos,
		text_origin,
		0.0f,
		dialog->text_size * avg_scale,
		0.0f,
		WHITE
	);

	EndShaderMode();
	EndMode2D();
}
