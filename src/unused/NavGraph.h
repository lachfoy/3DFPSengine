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

class NavGraph
{
public:
	NavGraph() {}
	~NavGraph();

	void Load(const std::string& path);

	std::vector<glm::vec3> FindPath(NavNode* start, NavNode* goal);
	std::vector<glm::vec3> RandomPath();

	NavNode* NodeClosestTo(const glm::vec3& pos);

	NavNode* Begin() { return m_navNodes[0]; }

	void DebugDraw();
	void DebugDrawPath(const std::vector<glm::vec3>& path);

	NavNode* GetRandomStartNode()
	{
		if (!m_randomStartNode) m_randomStartNode = m_navNodes[rand() % (m_navNodes.size() - 1)];
		return m_randomStartNode;
	}

private:
	float GetDistance(NavNode* from, NavNode* to);

	std::vector<NavNode*> m_navNodes; // used for managing memory
	std::unordered_map<NavNode*, std::vector<NavNode*>> m_adjList;

	NavNode* m_randomStartNode = nullptr;

};