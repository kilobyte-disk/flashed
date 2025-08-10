#ifndef FLASHSTATE_H
#define FLASHSTATE_H

#include "Helium.h"

extern int FLASHSTATE_Init(struct HlCore *CORE, float delta_time);

extern int FLASHSTATE_DeInit(struct HlCore *CORE, float delta_time);

extern int FLASHSTATE_Update(struct HlCore *CORE, float delta_time);

extern int FLASHSTATE_Render(struct HlCore *CORE, float delta_time);

#endif
