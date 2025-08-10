#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "Helium.h"

extern int MENUSTATE_Init(struct HlCore *CORE, float delta_time);

extern int MENUSTATE_DeInit(struct HlCore *CORE, float delta_time);

extern int MENUSTATE_Update(struct HlCore *CORE, float delta_time);

extern int MENUSTATE_Render(struct HlCore *CORE, float delta_time);

#endif
