/*
 * kilobyte-disk 2025
 * HlEase.c
 *
 * Purpose:
 * A utility for easing functions.
 *
 * Based off Robert Penner's easing equations,
 */

#include <math.h>
#include <stdlib.h>

#include "HlEase.h"

/* For all functions,
 * t = elapsed time
 * b = start value
 * c = change in value (end_value - start_value);
 * d = duration (total time)
 */

/* Linear */
float EASE_Linear(float t, float b, float c, float d)
{
	return c * t / d + b;
}


/* Quadratic */
float EASE_QuadIn(float t, float b, float c, float d)
{
	t = t / d;
	return c * t * t + b;
}

float EASE_QuadOut(float t, float b, float c, float d)
{
	t = t / d;
	return -c * t * (t - 2) + b;
}

float EASE_QuadInOut(float t, float b, float c, float d)
{
	t = t / d * 2;
	if (t < 1) {
		return c / 2 * t * t + b;
	} else {
		return -c / 2 * ((t - 1) * (t - 3) - 1) + b;
	}
}

float EASE_QuadOutIn(float t, float b, float c, float d)
{
	if (t < (d / 2)) {
		return EASE_QuadOut(t * 2, b, c / 2, d);
	} else {
		return EASE_QuadIn(t * 2 - d, b + c / 2, c / 2, d);
	}
}


/* Cubic */
float EASE_CubeIn(float t, float b, float c, float d)
{
	t = t / d;
	return c * t * t * t + b;
}

float EASE_CubeOut(float t, float b, float c, float d)
{
	t = t / d - 1;
	return c * (t * t * t + 1) + b;
}

float EASE_CubeInOut(float t, float b, float c, float d)
{
	t = t / d * 2;
	if (t < 1) {
		return c / 2 * t * t * t + b;
	} else {
		t = t - 2;
		return c / 2 * (t * t * t + 2) + b;
	}
}

float EASE_CubeOutIn(float t, float b, float c, float d)
{
	if (t < (d / 2)) {
		return EASE_CubeOut(t * 2, b, c / 2, d);
	} else {
		return EASE_CubeIn(t * 2 - d, b + c / 2, c / 2, d);
	}
}


/* Quartic */
float EASE_QuartIn(float t, float b, float c, float d)
{
	t = t / d;
	return c * pow(t, 4) + b;
}

float EASE_QuartOut(float t, float b, float c, float d)
{
	t = t / d - 1;
	return -c * ((float) pow(t, 4) - 1) + b;
}

float EASE_QuartInOut(float t, float b, float c, float d)
{
	t = t / d * 2;
	if (t < 1) {
		return c / 2 * pow(t, 4) + b;
	} else {
		t = t - 2;
		return -c / 2 * (pow(t, 4) - 2) + b;
	}
}

float EASE_QuartOutIn(float t, float b, float c, float d)
{
	if (t < d / 2) {
		return EASE_QuartOut(t * 2, b, c / 2, d);
	} else {
		return EASE_QuartIn(t * 2 - d, b + c / 2, c / 2, d);
	}
}


/* Quintic */
float EASE_QuintIn(float t, float b, float c, float d)
{
	t = t / d;
	return c * pow(t, 5) + b;
}

float EASE_QuintOut(float t, float b, float c, float d)
{
	t = t / d - 1;
	return c * (pow(t, 5) + 1) + b;
}

float EASE_QuintInOut(float t, float b, float c, float d)
{
	t = t / d * 2;
	if (t < 1) {
		return c / 2 * pow(t, 5) + b;
	} else {
		t = t - 2;
		return c / 2 * (pow(t, 5) + 2) + b;
	}
}

float EASE_QuintOutIn(float t, float b, float c, float d)
{
	if (t < d / 2) {
		return EASE_QuintOut(t * 2, b, c / 2, d);
	} else {
		return EASE_QuintIn(t * 2 - d, b + c / 2, c / 2, d);
	}
}


/* Sine */
float EASE_SineIn(float t, float b, float c, float d)
{
	return -c * cos(t / d * (M_PI / 2)) + c + b;
}

float EASE_SineOut(float t, float b, float c, float d)
{
	return c * sin(t / d * (M_PI / 2)) + b;
}

float EASE_SineInOut(float t, float b, float c, float d)
{
	return -c / 2 * (cos(M_PI * t / d) - 1) + b;
}

float EASE_SineOutIn(float t, float b, float c, float d)
{
	if (t < d / 2) {
		return EASE_SineOut(t * 2, b, c / 2, d);
	} else {
		return EASE_SineIn(t * 2 - d, b + c / 2, c / 2, d);
	}
}


/* Exponential */
float EASE_ExpoIn(float t, float b, float c, float d)
{
	if (t == 0) {
		return b;
	} else {
		return c * pow(2, (10 * (t / d - 1))) + b - c * 0.001f;	
	}
}

float EASE_ExpoOut(float t, float b, float c, float d)
{
	if (t == d) {
		return b + c;
	} else {
		return c * 1.001f * (1.0f - pow(2, (-10.0f * t / d))) + b;
	}
}

float EASE_ExpoInOut(float t, float b, float c, float d)
{
	if (t == 0) {
		return b;
	}
	if (t == d) {
		return b + c;
	}

	t = t / d * 2;
	if  (t < 1) {
		return c / 2 * pow(2, (10 * (t - 1))) + b - c * 0.0005f;
	} else {
		t = t - 1;
		return c / 2 * 1.0005 * (2 - pow(2, (-10 * t))) + b;
	}
}

float EASE_ExpoOutIn(float t, float b, float c, float d)
{
	if (t < d / 2) {
		return EASE_ExpoOut(t * 2, b, c / 2, d);
	} else {
		return EASE_ExpoIn(t * 2 - d, b + c / 2, c / 2, d);
	}
}


/* Circular */
float EASE_CircIn(float t, float b, float c, float d)
{
	t = t / d;
	return -c * (sqrt(1 - (t*t))) + b;
}

float EASE_CircOut(float t, float b, float c, float d)
{
	t = t / d - 1;
	return c * sqrt(1.0 - (t*t)) + b;
}

float EASE_CircInOut(float t, float b, float c, float d)
{
	t = t / d * 2;
	if (t < 1) {
		return -c / 2 * (sqrt(1.0 - (t*t)) - 1) + b;
	} else {
		t = t - 2;
		return c / 2 * (sqrt(1.0 - (t*t)) + 1) + b;
	}
}

float EASE_CircOutIn(float t, float b, float c, float d)
{
	if (t < d / 2) {
		return EASE_CircOut(t * 2, b, c / 2, d);
	} else {
		return EASE_CircIn((t * 2) - d, b + c / 2, c / 2, d);
	}
}


/* Elastic */
/*
 * a = amplitude
 * p = period
 * 
 * p = d * 0.3 DEFAULT
 */
float EASE_ElasticIn(float t, float b, float c, float d, float a, float p)
{
	if (t == 0) {
		return b;
	}
	
	t = t / d;

	if (t == 1) {
		return b + c;
	}

	float s;

	if (a < abs(c)) {
		a = c;
		s = p / 4;
	} else {
		s = p / (2 * M_PI) * asin(c/a);
	}

	t = t - 1;

	return -(a * pow(2, 10 * t) * sin((t * d - s) * (2 * M_PI) / p)) + b;
}

float EASE_ElasticOut(float t, float b, float c, float d, float a, float p)
{
	if (t == 0) {
		return b;
	}

	t = t / d;

	if (t == 1) {
		return b + c;
	}

	float s;

	if (a < abs(c)) {
		a = c;
		s = p / 4;
	} else {
		s = p / (2 * M_PI) * asin(c/a);
	}

	return a * pow(2, -10 * t) * sin((t * d - s) * (2 * M_PI) / p) + c + b;
}

float EASE_ElasticInOut(float t, float b, float c, float d, float a, float p)
{
	if (t == 0) {
		return b;
	}

	t = t / d * 2;

	float s;

	if (a < abs(c)) {
		a = c;
		s = p / 4;
	} else {
		s = p / (2 * M_PI) * asin(c/a);
	}

	if (t < 1) {
		t = t - 1;
		return -0.5 * (a * pow(2, 10 * t) * sin((t * d - s) * (2 * M_PI) / p)) + b;
	} else {
		t = t - 1;
		return a * pow(2, -10 * t) * sin((t * d - s) * (2 * M_PI) / p ) * 0.5 + c + b;
	}
}

float EASE_ElasticOutIn(float t, float b, float c, float d, float a, float p)
{
	if (t < d / 2) {
		return EASE_ElasticOut(t * 2, b, c / 2, d, a, p);
	} else {
		return EASE_ElasticIn((t * 2) - d, b + c / 2, c / 2, d, a, p);
	}
}


/* Back easing */
/* s = 1.70158f DEFAULT */

float EASE_BackIn(float t, float b, float c, float d, float s)
{
	t = t / d;
	return c * t * t * ((s + 1) * t - s) + b;
}

float EASE_BackOut(float t, float b, float c, float d, float s)
{
	t = t / d - 1;
	return c * (t * t * ((s + 1) * t + s) + 1) + b;
}

float EASE_BackInOut(float t, float b, float c, float d, float s)
{
	s = s * 1.525;
	t = t / d * 2;
	if (t < 1) {
		return c / 2 * (t * t * ((s + 1) * t - s)) + b;
	} else {
		t = t - 2;
		return c / 2 * (t * t * ((s + 1) * t + s) + 2) + b;
	}
}

float EASE_BackOutIn(float t, float b, float c, float d, float s)
{
	if (t < d / 2) {
		return EASE_BackOut(t * 2, b, c / 2, d, s);
	} else {
		return EASE_BackIn((t * 2) - d, b + c / 2, c / 2, d, s);
	}
}


/* Bounce easing */
float EASE_BounceOut(float t, float b, float c, float d)
{
	t = t / d;
	if (t < 1 / 2.75) {
		return c * (7.5625 * t * t) + b;
	} else if (t < 2 / 2.75) {
		t = t - (1.5 / 2.75);
		return c * (7.5625 * t * t + 0.75) + b;
	} else if (t < 2.5 / 2.75) {
		t = t - (2.25 / 2.75);
		return c * (7.5625 * t * t + 0.9375) + b;
	} else {
		t = t - (2.265 / 2.75);
		return c * (7.5625 * t * t + 0.9854785) + b;
	}
}

float EASE_BounceIn(float t, float b, float c, float d)
{
	return c - EASE_BounceOut(d - t, 0, c, d) + b;
}

float EASE_BounceInOut(float t, float b, float c, float d)
{
	if (t < d / 2) {
		return EASE_BounceIn(t * 2, 0, c, d) * 0.5 + b;
	} else {
		return EASE_BounceOut(t * 2 - d, 0, c, d) * 0.5 + c * 0.5 + b;
	}
}

float EASE_BounceOutIn(float t, float b, float c, float d)
{
	if (t < d / 2) {
		return EASE_BounceOut(t * 2, b, c / 2, d);
	} else {
		return EASE_BounceIn((t * 2) - d, b + c / 2, c / 2, d);
	}
}
