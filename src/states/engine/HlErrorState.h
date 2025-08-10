#ifndef HLERROR_H
#define HLERROR_H

#include "Helium.h"

extern int HLERRORSTATE_Init(struct HlCore *CORE, float delta_time);

extern int HLERRORSTATE_DeInit(struct HlCore *CORE, float delta_time);

extern int HLERRORSTATE_Update(struct HlCore *CORE, float delta_time);

extern int HLERRORSTATE_Render(struct HlCore *CORE, float delta_time);

#endif
