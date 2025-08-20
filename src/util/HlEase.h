#ifndef HLEASE_H
#define HLEASE_H

enum HlEasingStyle {
	LINEAR,

	QUAD_IN,
	QUAD_OUT,
	QUAD_IN_OUT,
	QUAD_OUT_IN,

	CUBE_IN,
	CUBE_OUT,
	CUBE_IN_OUT,
	CUBE_OUT_IN,

	QUART_IN,
	QUART_OUT,
	QUART_IN_OUT,
	QUART_OUT_IN,

	QUINT_IN,
	QUINT_OUT,
	QUINT_IN_OUT,
	QUINT_OUT_IN,

	SINE_IN,
	SINE_OUT,
	SINE_IN_OUT,
	SINE_OUT_IN,

	EXPO_IN,
	EXPO_OUT,
	EXPO_IN_OUT,
	EXPO_OUT_IN,

	CIRC_IN,
	CIRC_OUT,
	CIRC_IN_OUT,
	CIRC_OUT_IN,

	ELASTIC_IN,
	ELASTIC_OUT,
	ELASTIC_IN_OUT,
	ELASTIC_OUT_IN,

	BACK_IN,
	BACK_OUT,
	BACK_IN_OUT,
	BACK_OUT_IN,

	BOUNCE_OUT,
	BOUNCE_IN,
	BOUNCE_IN_OUT,
	BOUNCE_OUT_IN,
};

/* For all functions,
 * t = elapsed time
 * b = start value
 * c = change in value (end_value - start_value);
 * d = duration (total time)
 */

extern float EASE_Linear(float t, float b, float c, float d);


extern float EASE_QuadIn(float t, float b, float c, float d);

extern float EASE_QuadOut(float t, float b, float c, float d);

extern float EASE_QuadInOut(float t, float b, float c, float d);

extern float EASE_QuadOutIn(float t, float b, float c, float d);


extern float EASE_CubeIn(float t, float b, float c, float d);

extern float EASE_CubeOut(float t, float b, float c, float d);

extern float EASE_CubeInOut(float t, float b, float c, float d);

extern float EASE_CubeOutIn(float t, float b, float c, float d);


extern float EASE_QuartIn(float t, float b, float c, float d);

extern float EASE_QuartOut(float t, float b, float c, float d);

extern float EASE_QuartInOut(float t, float b, float c, float d);

extern float EASE_QuartOutIn(float t, float b, float c, float d);


extern float EASE_QuintIn(float t, float b, float c, float d);

extern float EASE_QuintOut(float t, float b, float c, float d);

extern float EASE_QuintInOut(float t, float b, float c, float d);

extern float EASE_QuintOutIn(float t, float b, float c, float d);


extern float EASE_SineIn(float t, float b, float c, float d);

extern float EASE_SineOut(float t, float b, float c, float d);

extern float EASE_SineInOut(float t, float b, float c, float d);

extern float EASE_SineOutIn(float t, float b, float c, float d);


extern float EASE_ExpoIn(float t, float b, float c, float d);

extern float EASE_ExpoOut(float t, float b, float c, float d);

extern float EASE_ExpoInOut(float t, float b, float c, float d);

extern float EASE_ExpoOutIn(float t, float b, float c, float d);


extern float EASE_CircIn(float t, float b, float c, float d);

extern float EASE_CircOut(float t, float b, float c, float d);

extern float EASE_CircInOut(float t, float b, float c, float d);

extern float EASE_CircOutIn(float t, float b, float c, float d);


extern float EASE_ElasticIn(float t, float b, float c, float d, float a, float p);

extern float EASE_ElasticOut(float t, float b, float c, float d, float a, float p);

extern float EASE_ElasticInOut(float t, float b, float c, float d, float a, float p);

extern float EASE_ElasticOutIn(float t, float b, float c, float d, float a, float p);


extern float EASE_BackIn(float t, float b, float c, float d, float s);

extern float EASE_BackOut(float t, float b, float c, float d, float s);

extern float EASE_BackInOut(float t, float b, float c, float d, float s);

extern float EASE_BackOutIn(float t, float b, float c, float d, float s);


extern float EASE_BounceOut(float t, float b, float c, float d);

extern float EASE_BounceIn(float t, float b, float c, float d);

extern float EASE_BounceInOut(float t, float b, float c, float d);

extern float EASE_BounceOutIn(float t, float b, float c, float d);

#endif
