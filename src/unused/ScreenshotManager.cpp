#include "ScreenshotManager.h"

#include <glad/glad.h>
#include <filesystem>
#include <regex>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void ScreenshotManager::TakeScreenshot(int width, int height)
{
	if (!std::filesystem::exists(DirectoryPath))
	{
		std::filesystem::create_directories(DirectoryPath);
	}

	std::string fileName = GetName();

	unsigned char* data = new unsigned char[3 * width * height];

	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_flip_vertically_on_write(0xFF);

	std::string fullPath = DirectoryPath + fileName;
	stbi_write_png(fullPath.c_str(), width, height, 3, data, width * 3);

	delete[] data;
}

std::string ScreenshotManager::GetName()
{
	int count = 0;
	std::string filename;
	do
	{
		filename = BaseName + std::to_string(count) + Extension;
		count++;
	} while (std::filesystem::exists(DirectoryPath + filename));

	return filename;
}
