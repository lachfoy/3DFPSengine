#include "Common.h"
#include "NavGraph.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <queue>
#include <unordered_set>

#include "DebugRenderer.h"

NavGraph::~NavGraph()
{
	for (NavNode* navNode : m_navNodes)
	{
		SAFE_DELETE(navNode);
	}
}

void NavGraph::Load(const std::string& path)
{
	// Parse the nav graph file
	// the format is:
	// Vertex data starts with "# Vertices" then each line is read as "x y z", in order of its id
	// Edge data starts with "# Edges" then each line is read as "to from" ids
	std::ifstream file(path);
	std::string line;

	NavFileParseMode parseMode = NavFileParseMode::NONE;

	while (std::getline(file, line))
	{
		if (line == "# Vertices")
		{
			parseMode = NavFileParseMode::PARSING_VERTICES;
			continue;
		}
		else if (line == "# Edges")
		{
			parseMode = NavFileParseMode::PARSING_EDGES;
			continue;
		}

		std::istringstream iss(line);
		switch (parseMode)
		{
		case NavFileParseMode::PARSING_VERTICES:
			float x, y, z;
			if (iss >> x >> y >> z)
			{
				NavNode* navNode = new NavNode();
				m_navNodes.push_back(navNode);

				navNode->pos = glm::vec3(x, y, z);

				m_adjList[navNode] = std::vector<NavNode*>();
			}
			break;
		case NavFileParseMode::PARSING_EDGES:
			int from, to;
			if (iss >> from >> to)
			{
				NavNode* nodeFrom = m_navNodes[from];
				NavNode* nodeTo = m_navNodes[to];

				// add an edge to the adjancency list
				m_adjList[nodeFrom].push_back(nodeTo);
				m_adjList[nodeTo].push_back(nodeFrom); // undirected graph
			}
			break;
		default:
			ASSERT_MSG(false, "File format error parsing nav file \"%s\"", path.c_str());
		}
	}
}

std::vector<glm::vec3> NavGraph::FindPath(NavNode* start, NavNode* goal)
{
	// reset the nodes
	for (NavNode* node : m_navNodes)
	{
		node->g = FLT_MAX;
		node->h = 0.0f;
		node->f = FLT_MAX;
		node->parent = nullptr;
	}

	std::priority_queue<NavNode*, std::vector<NavNode*>, NavNodeCompare> openSet;
	std::unordered_set<NavNode*> closedSet;

	start->g = 0.0f;
	start->h = glm::distance(start->pos, goal->pos);
	start->f = start->g + start->h;
	start->parent = nullptr;

	openSet.push(start);

	while (!openSet.empty())
	{
		NavNode* current = openSet.top();
		openSet.pop();

		if (current == goal)
		{
			// Goal reached, construct path
			std::vector<glm::vec3> path;
			while (current != nullptr)
			{
				path.push_back(current->pos);
				current = current->parent;
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		closedSet.insert(current);

		for (NavNode* neighbor : m_adjList[current])
		{
			if (closedSet.find(neighbor) != closedSet.end())
			{
				continue; // Skip already evaluated nodes
			}

			float tentative_g = current->g + glm::distance(current->pos, neighbor->pos);

			if (tentative_g < neighbor->g)
			{
				neighbor->parent = current;
				neighbor->g = tentative_g;
				neighbor->h = glm::distance(neighbor->pos, goal->pos);
				neighbor->f = neighbor->g + neighbor->h;

				openSet.push(neighbor);
			}
		}
	}

	return std::vector<glm::vec3>(); // No path found
}

std::vector<glm::vec3> NavGraph::RandomPath()
{
	NavNode* start = m_navNodes[rand() % (m_navNodes.size() - 1)];
	NavNode* goal = m_navNodes[rand() % (m_navNodes.size() - 1)];

	return FindPath(start, goal);
}

NavNode* NavGraph::NodeClosestTo(const glm::vec3& pos)
{
	float minDist = FLT_MAX;
	NavNode* closestNode = nullptr;

	for (NavNode* node : m_navNodes)
	{
		float dist = glm::distance(pos, node->pos);
		if (dist < minDist) {
			minDist = dist;
			closestNode = node;
		}
	}

	return closestNode;
}

void NavGraph::DebugDraw()
{
	for (const auto& it : m_adjList)
	{
		NavNode* from = it.first;
		for (NavNode* to : m_adjList[from])
		{
			gDebugRenderer.AddLine(from->pos, to->pos, glm::vec3(0.0f, 1.0f, 1.0f));
		}
	}
}

void NavGraph::DebugDrawPath(const std::vector<glm::vec3>& path)
{
	if (path.size() < 2) return;
	for (size_t i = 0; i < path.size() - 1; i++)
	{
		gDebugRenderer.AddLine(path[i], path[i + 1], glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);
	}
}

float NavGraph::GetDistance(NavNode* from, NavNode* to)
{
	return glm::length(to->pos - from->pos);
}
