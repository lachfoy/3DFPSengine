#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846 // pi
#endif
#define PI (float)M_PI

#define DEGTORAD(x) ((x) * PI / 180.0f)
#define RADTODEG(x) ((x) * 180.0f / PI)

#include <cassert>
#define ASSERT(_Expression) assert(_Expression)

#include <cstdio>
#include <cstdlib>
#define ASSERT_MSG(expression, message, ...) \
	do { \
		if (!(expression)) { \
			char assertMessage[256]; \
			snprintf(assertMessage, sizeof(assertMessage), (message), ##__VA_ARGS__); \
			fprintf(stderr, "Assertion failed: %s\nFile: %s, Line: %d\n", assertMessage, __FILE__, __LINE__); \
			abort(); \
		} \
	} while(0)

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = nullptr; } }
