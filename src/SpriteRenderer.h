#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"

#include <vector>

class SpriteEntity;

struct Vertex
{
	glm::vec2 position;
	glm::vec2 texCoord;

	inline static Vertex Make(glm::vec2 positon, glm::vec2 texCoord)
	{
		Vertex vertex;
		vertex.position = positon;
		vertex.texCoord = texCoord;
		return vertex;
	}
};

typedef std::vector<Vertex> tVertexVec;
typedef std::vector<unsigned int> tIndexVec;

enum class FlipPolicy
{
	DoNotFlip,
	FlipX
};

class RenderObject
{
public:
	RenderObject(tVertexVec* vertexVec, tIndexVec* indexVec, glm::vec2* position, Texture* texture, float rotation = 0.0f, FlipPolicy flipPolicy = FlipPolicy::DoNotFlip)
		: m_vertexVec(vertexVec), m_indexVec(indexVec), m_position(position), m_texture(texture), m_rotation(rotation), m_flipPolicy(flipPolicy) {}

	tVertexVec* GetVertexVec() const { return m_vertexVec; }
	tIndexVec* GetIndexVec() const { return m_indexVec; }
	Texture* GetTexture() const { return m_texture; }
	glm::vec2* GetPosition() const { return m_position; }
	float GetRotation() const { return m_rotation; }
	FlipPolicy GetFlipPolicy() const { return m_flipPolicy; }

private:
	tVertexVec* m_vertexVec;
	tIndexVec* m_indexVec;
	Texture* m_texture;
	glm::vec2* m_position;
	float m_rotation;
	FlipPolicy m_flipPolicy;

};

class RenderObjectCompare
{
public:
	bool operator()(const RenderObject& obj1, const RenderObject& obj2) const
	{
		return obj1.GetTexture() < obj2.GetTexture();
		//if (obj1.GetPosition() && obj2.GetPosition())
		//{
		//	return obj1.GetPosition()->y < obj2.GetPosition()->y;
		//}

		//return false;
	}
};

class SpriteRenderer
{
public:
	SpriteRenderer() = default;
	~SpriteRenderer() {}

	void Init();
	void SetProjection(unsigned int screenWidth, unsigned int screenHeight);
	void Dispose();

	void AddRenderObject(const RenderObject& renderObject);
	
	void RenderObjects();

	void FlushBatch();
	void ClearBatch();

private:
	void CreateShaderProgram();
	void CreateRenderData();

	void CheckError();

	GLuint m_shaderProgram;

	tVertexVec m_vertexBuffer;
	tIndexVec m_indexBuffer;

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;

	std::vector<RenderObject> m_renderObjects;

};