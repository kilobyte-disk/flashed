#ifndef HLINIT_H
#define HLINIT_H

#include "Helium.h"

extern int HLINITSTATE_Init(struct HlCore *CORE, float delta_time);

extern int HLINITSTATE_DeInit(struct HlCore *CORE, float delta_time);

extern int HLINITSTATE_Update(struct HlCore *CORE, float delta_time);

extern int HLINITSTATE_Render(struct HlCore *CORE, float delta_time);

#endif
