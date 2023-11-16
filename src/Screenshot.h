#pragma once

#include <glad/glad.h>

#include <filesystem>
#include <string>
#include <regex>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class Screenshot
{
public:
	Screenshot() {}
	~Screenshot() {}

	static void TakeScreenshot(int width, int height)
	{
		if (!std::filesystem::exists(s_directoryPath))
		{
			std::filesystem::create_directories(s_directoryPath);
		}

		std::string fileName = GetName();

		unsigned char* data = new unsigned char[3 * width * height];

		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_flip_vertically_on_write(0xFF);

		stbi_write_png((s_directoryPath + "/" + fileName).c_str(), width, height, 3, data, width * 3);

		delete[] data;
	}

private:
	static std::string GetName()
	{
		int count = 0;
		std::string filename;
		do
		{
			filename = s_baseName + std::to_string(count) + s_extension;
			count++;
		} while (std::filesystem::exists(s_directoryPath + "/" + filename));

		return filename;
	}

	static const std::string s_directoryPath;
	static const std::string s_baseName;
	static const std::string s_extension;

};

const std::string Screenshot::s_directoryPath = "./screenshots";
const std::string Screenshot::s_baseName = "screenshot_";
const std::string Screenshot::s_extension = ".png";
