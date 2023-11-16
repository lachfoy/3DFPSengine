#pragma once

#include <glad/glad.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class Screenshot
{
public:
	static void TakeScreenshot(int width, int height)
	{
		unsigned char* data = new unsigned char[3 * width * height];

		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_flip_vertically_on_write(0xFF);

		stbi_write_png("test.png", width, height, 3, data, width * 3);

		delete[] data;
	}

};