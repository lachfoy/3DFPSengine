#include "TileMap.h"

void TileMap::CreateDebugMap()
{
	m_width = 32;
	m_height = 18;

	int temp[32 * 18] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	m_tileData = new int[m_width * m_height];
	for (int i = 0; i < m_width * m_height; i++)
	{
		m_tileData[i] = temp[i];
	}
}

void TileMap::BuildTileMesh()
{
	float tileTexSize = 1.f;

	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			int tile = m_tileData[x + m_width * y];
			float tx0 = (tile & 15) * tileTexSize;
			float ty0 = (tile >> 4) * tileTexSize;

			Vertex topLeft;
			topLeft.position = glm::vec2(x, y) * 32.0f;
			topLeft.texCoord = glm::vec2(tx0, ty0);
			m_vertexVec.push_back(topLeft);

			Vertex topRight;
			topRight.position = glm::vec2(x + 1, y) * 32.0f;
			topRight.texCoord = glm::vec2(tx0 + tileTexSize, ty0);
			m_vertexVec.push_back(topRight);

			Vertex bottomLeft;
			bottomLeft.position = glm::vec2(x, y + 1) * 32.0f;
			bottomLeft.texCoord = glm::vec2(tx0, ty0 + tileTexSize);
			m_vertexVec.push_back(bottomLeft);

			Vertex bottomRight;
			bottomRight.position = glm::vec2(x + 1, y + 1) * 32.0f;
			bottomRight.texCoord = glm::vec2(tx0 + tileTexSize, ty0 + tileTexSize);
			m_vertexVec.push_back(bottomRight);
		}
	}

	unsigned int j = 0;
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			m_indexVec.push_back(j + 0);
			m_indexVec.push_back(j + 1);
			m_indexVec.push_back(j + 2);
			m_indexVec.push_back(j + 1);
			m_indexVec.push_back(j + 3);
			m_indexVec.push_back(j + 2);
			j += 4;
		}
	}
}

void TileMap::Render(SpriteRenderer* renderer)
{
	// TODO dont use the sprite renderer for this. Add a more efficient tile map renderer
	RenderObject renderObject = RenderObject(
		&m_vertexVec,
		&m_indexVec,
		nullptr,
		m_texture,
		0.0f
	);

	renderer->AddRenderObject(renderObject);
}

void TileMap::Destroy()
{
	delete m_tileData;
	m_tileData = nullptr;
};
