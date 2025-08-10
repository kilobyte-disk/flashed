#ifndef HLUTILS_H
#define HLUTILS_H

#include "Helium.h"

extern float HL_Clamp(float n, float min, float max);

extern Font *HL_GetFont(enum HlFont font, struct HlCore *CORE);

extern void HL_SafeStrcpy(char *destination, char *source);

extern int *CodepointRemoveDuplicates(int *codepoints, int codepoint_count, int *codepoint_result_count);

#endif
