#ifndef TEMPLATESTATE_H
#define TEMPLATESTATE_H

#include "Helium.h"

extern int TEMPLATESTATE_Init(struct HlCore *CORE, float delta_time);

extern int TEMPLATESTATE_DeInit(struct HlCore *CORE, float delta_time);

extern int TEMPLATESTATE_Update(struct HlCore *CORE, float delta_time);

extern int TEMPLATESTATE_Render(struct HlCore *CORE, float delta_time);

#endif
