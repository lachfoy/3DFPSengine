#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#define M_PI 3.14159265358979323846 // pi
#define PI (float)M_PI

#define DEGTORAD(x) ((x) * PI / 180.0f)
#define RADTODEG(x) ((x) * 180.0f / PI)

#include <cassert>
#define ASSERT(_Expression) assert(_Expression)

#define lerp(norm, min, max) ( (norm) * ((max) - (min)) + (min) )
