#pragma once

#include "Entity.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "SpriteRenderer.h"

class Texture;

class SpriteEntity : public Entity
{
public:
	SpriteEntity() = default;
	SpriteEntity(glm::vec2 position, glm::vec2 size, Texture* texture);
	~SpriteEntity() {}

	glm::vec2 GetSize() const { return m_size; }
	glm::vec2 GetExtents() const { return m_size * 0.5f; }

	void SetRotation(float radians); // sets the rotation

	void SetFlipPolicy(FlipPolicy flipPolicy); // I don't like this lol

	virtual void Update(float dt) {};

	virtual void Render(SpriteRenderer* renderer);
	void RenderDebugQuad();
	
	virtual void OnDestroy() {};

	static bool Collision(const SpriteEntity& objA, const SpriteEntity& objB)
	{
		const glm::vec2& aCenter = objA.GetPosition();
		const glm::vec2& bCenter = objB.GetPosition();
		const glm::vec2& aExtents = objA.GetExtents();
		const glm::vec2& bExtents = objB.GetExtents();
	
		float xDiff = abs(aCenter.x - bCenter.x);
		float yDiff = abs(aCenter.y - bCenter.y);

		return (
			xDiff <= aExtents.x + bExtents.x &&
			yDiff <= aExtents.y + bExtents.y
		);
	}

	static void ResolveCollision(SpriteEntity& objA, SpriteEntity& objB)
	{
		glm::vec2 objAtoObjB = objB.m_position - objA.m_position;
		float distanceBetween = glm::length(objAtoObjB);

		float diff = objA.m_radius + objB.m_radius - distanceBetween;

		if (diff > 0.0f) {  // Check if there's an overlap before resolving the collision.
			glm::vec2 dirAtoB = glm::normalize(objAtoObjB);

			objA.m_position -= dirAtoB * (diff * 0.5f);
			objB.m_position += dirAtoB * (diff * 0.5f);
		}
	}

protected:
	tVertexVec m_vertexVec;
	tIndexVec m_indexVec;
	Texture* m_texture = nullptr;
	glm::vec2 m_size;

	float m_rotation = 0.0f;
	FlipPolicy m_flipPolicy = FlipPolicy::DoNotFlip;

	float m_radius; // Used for physics detection/resolution

};
