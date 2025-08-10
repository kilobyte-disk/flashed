/*
 * kilobyte-disk 2025
 * HlUtils.c
 *
 * Purpose:
 * A module containing common general-purpose utility functions.
 * NOTE: Utils are categorized here when they don't fit nicely into a particular category.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

#include "GLOBALS.h"
#include "HlUtils.h"

float HL_Clamp(float n, float min, float max)
{
	const float t = n < min ? min : n;
	return t > max ? max : t;
}

Font *HL_GetFont(enum HlFont font, struct HlCore *CORE)
{
	switch (font) {
	case VT323:
		return CORE->GLOBALS->F_VT323;

	case NOTO_KR:
		return CORE->GLOBALS->F_NotoKR;
	}

	
	/* Default */
	return CORE->GLOBALS->F_Roboto;
}

/* 
 * Safely copies the contents of one string to another,
 * flushes the destination char buffer to avoid extra characters,
 * 
 * WARNING: This does NOT check that the destination char buffer
 * has enough space to copy the source buffer to.
 */
void HL_SafeStrcpy(char *destination, char *source)
{
	int dest_len = strlen(destination);
	int source_len = strlen(source);

	memset(destination, 0, (int) dest_len);
	strncpy(destination, source, source_len);
}

/* @raysan5
 * From https://www.raylib.com/examples/text/loader.html?name=text_codepoints_loading
 */

/* Remove codepoint duplicates if requested
 * WARNING: This process could be a bit slow if there text to process is very long */
int *CodepointRemoveDuplicates(int *codepoints, int codepoint_count, int *codepoint_result_count)
{
    int codepoints_clear_count = codepoint_count;
    int *codepoints_clear = (int *)RL_CALLOC(codepoint_count, sizeof(int));
    memcpy(codepoints_clear, codepoints, codepoint_count*sizeof(int));

    // Remove duplicates
    for (int i = 0; i < codepoints_clear_count; i++)
    {
        for (int j = i + 1; j < codepoints_clear_count; j++)
        {
            if (codepoints_clear[i] == codepoints_clear[j])
            {
                for (int k = j; k < codepoints_clear_count; k++) codepoints_clear[k] = codepoints_clear[k + 1];

                codepoints_clear_count--;
                j--;
            }
        }
    }
    
    // Resize array to number of resulting codepoints (lower or equal to input)
    RL_REALLOC(codepoints_clear, codepoints_clear_count*sizeof(int));
    
    *codepoint_result_count = codepoints_clear_count;
    return codepoints_clear;
}
