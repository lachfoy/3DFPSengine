#pragma once

#include <vector>
#include <string>

#include <unordered_map>
#include <glm/glm.hpp>

enum class NavFileParseMode
{
	PARSING_VERTICES,
	PARSING_EDGES,
	NONE
};

struct NavNode
{
	glm::vec3 pos;
	float g = 0.0f;
	float h = 0.0f;
	float f = 0.0f;
};

class NavGraph
{
public:
	NavGraph() {}
	~NavGraph() = default;

	void Load(const std::string& path);

	std::vector<glm::vec3> FindPath(const glm::vec3& start, const glm::vec3& goal);

	void DebugDraw();

private:
	float GetCost(const NavNode& from, const NavNode& to) const {}

	std::vector<NavNode> m_navNodes;
	std::unordered_map<int, std::vector<int>> m_adjList;

};