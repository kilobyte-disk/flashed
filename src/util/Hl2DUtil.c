/*
 * kilobyte-disk 2025
 * Hl2DUtil.c
 *
 * Purpose:
 * Utility functions for 2D objects.
 */

#include <stdio.h>
#include "raylib.h"

bool HL_Vec2TouchingRect(Vector2 vector2, Rectangle rectangle) {
	/* Is the vector2 currently ANYWHERE on the rectangle? */

	return
	(vector2.x >= rectangle.x) && (vector2.x <= (rectangle.x + rectangle.width))
	&&
	(vector2.y >= rectangle.y) && (vector2.y <= (rectangle.y + rectangle.height));
}

