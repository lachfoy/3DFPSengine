#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#undef M_PI
#define M_PI 3.14159265358979323846 // pi
#define PI (float)M_PI

#define DEGTORAD(x) ((x) * PI / 180.0f)
#define RADTODEG(x) ((x) * 180.0f / PI)

#define ASSERT(x) if (!(x)) __debugbreak();

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

#include <glad/glad.h>

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* func, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		fprintf(stderr, "OpenGL error: %d\nFunction: %s, File: %s, Line: %d\n", (int)error, func, file, line);
		return false;
	}
	return true;
}

#define GLCALL(x) \
	GLClearError(); \
	x; \
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));
