#include "Font.h"

#include <iostream>
#include <fstream>

Font::~Font()
{
	delete m_texture;
}

short Font::GetKerningAmount(unsigned int first, unsigned int second)
{
	for (const KerningPair& pair : m_kerningPairs) {
		if (pair.first == first && pair.second == second) {
			return pair.amount;
		}
	}

	return 0;
}

void Font::Load(const char* path)
{
	std::ifstream myfile(path, std::ios::binary | std::ios::ate);
	if (!myfile.is_open()) {
		std::cerr << "Error opening .fnt file: " << path << "\n";
		return;
	}

	std::streampos size = myfile.tellg();
	myfile.seekg(0, std::ios::beg);

	// Read file identifier and version
	char fileIdentifier[4] = {}; // One extra byte for null terminator
	myfile.read(fileIdentifier, 3);

	if (std::string(fileIdentifier) != "BMF") {
		std::cerr << "Error: Not a valid BMF file.\n";
		return;
	}

	char version;
	myfile.read(&version, 1);
	if (version != 3) {
		std::cerr << "Error loading .fnt file: Unsupported format version " << static_cast<int>(version) << ".\n";
		return;
	}

	char blockType;
	int blockSize;

	// Info Block
	myfile.read(&blockType, 1);
	myfile.read(reinterpret_cast<char*>(&blockSize), 4);

	// Skipping info block content
	myfile.seekg(blockSize, std::ios::cur);

	// Common Block
	myfile.read(&blockType, 1);
	myfile.read(reinterpret_cast<char*>(&blockSize), 4);
	std::vector<char> commonBlock(blockSize);
	myfile.read(commonBlock.data(), blockSize);

	// Extract line height (assuming the line height is at the beginning of the common block)
	m_lineHeight = *reinterpret_cast<unsigned short*>(commonBlock.data());

	// Pages Block
	myfile.read(&blockType, 1);
	myfile.read(reinterpret_cast<char*>(&blockSize), 4);
	std::vector<char> pagesBlock(blockSize);
	myfile.read(pagesBlock.data(), blockSize);

	// Assuming page names are null-terminated strings, not handling multiple pages
	std::string texturePath(pagesBlock.data());
	m_texture = new Texture(("data/fonts/" + texturePath).c_str()); // hardcoding directory. change this

	// Chars Block
	myfile.read(&blockType, 1);
	myfile.read(reinterpret_cast<char*>(&blockSize), 4);
	int numChars = blockSize / 20; // each char info block is 20 bytes

	for (int i = 0; i < numChars; ++i) {
		CharInfo charInfo;
		myfile.read(reinterpret_cast<char*>(&charInfo), 20);
		m_charInfos.emplace(charInfo.id, charInfo);
	}

	// Kerning pairs are not handled
	myfile.read(&blockType, 1);
	myfile.read(reinterpret_cast<char*>(&blockSize), 4);

	int numKerningPairs = blockSize / 10;
	for (int i = 0; i < numKerningPairs; ++i) {
		KerningPair kerningPair;
		myfile.read(reinterpret_cast<char*>(&kerningPair), 10);
		m_kerningPairs.push_back(kerningPair);
	}

	myfile.close();
}

//float textureW = static_cast<float>(m_texture->GetWidth());
//float textureH = static_cast<float>(m_texture->GetHeight());
//// Convert to texture coordinates and store
//charInfo.x = static_cast<float>(charInfo.x) / textureW;
//charInfo.y = static_cast<float>(charInfo.y) / textureH;
//charInfo.width = static_cast<float>(charInfo.width) / textureW;
//charInfo.height = static_cast<float>(charInfo.height) / textureH;