#pragma once

#include <vector>
#include <string>

#include <unordered_map>
#include <glm/glm.hpp>

struct NavNode
{
	glm::vec3 pos;
	float g = FLT_MAX;
	float h = 0.0f;
	float f = 0.0f;

	NavNode* parent = nullptr;
};

struct NavNodeCompare
{
	bool operator()(const NavNode* a, const NavNode* b) const
	{
		return a->f > b->f;
	}
};

class PhysicsWorld;

class NavGrid
{
public:
	NavGrid() {}
	~NavGrid() {}

	void Generate(PhysicsWorld* physicsWorld);

private:

};