#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "Texture.h"
#include "SpriteRenderer.h"

class TileMap
{
public:
	TileMap(Texture* texture) : m_texture(texture) {}
	~TileMap() = default;
	
	void CreateDebugMap();
	void BuildTileMesh();

	void Render(SpriteRenderer* renderer);
	void Destroy();

private:
	tVertexVec m_vertexVec;
	tIndexVec m_indexVec;
	Texture* m_texture;

	int* m_tileData;
	int m_width;
	int m_height;

};