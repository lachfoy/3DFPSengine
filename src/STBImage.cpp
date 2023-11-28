#include "STBImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* STBImage::Load(const char* filename, int* width, int* height, int* numChannels, bool flipOnLoad)
{
	stbi_set_flip_vertically_on_load(flipOnLoad);

	unsigned char* data = stbi_load(filename, width, height, numChannels, 0);
	if (data == nullptr)
	{
		delete data;
		return nullptr;
	}

	return data;
}
