#include "Common.h"
#include "NavGraph.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "DebugRenderer.h"

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
				NavNode navNode;
				navNode.pos = glm::vec3(x, y, z);
				nodes.push_back(navNode);
			}
			break;
		case NavFileParseMode::PARSING_EDGES:
			int from, to;
			if (iss >> from >> to)
			{
				// add an edge to the adjancency list
				adjList[from].push_back(to);
				adjList[to].push_back(from); // undirected graph
			}
			break;
		default:
			ASSERT_MSG(false, "File format error parsing nav file \"%s\"", path.c_str());
		}
	}
}

std::vector<glm::vec3> NavGraph::FindPath(const glm::vec3& start, const glm::vec3& goal)
{

	
}

void NavGraph::DebugDraw()
{
	for (const auto& it : adjList)
	{
		NavNode from = nodes[it.first];
		for (int id : it.second)
		{
			NavNode to = nodes[id];

			gDebugRenderer.AddLine(from.pos, to.pos, glm::vec3(0.0f, 1.0f, 1.0f));
		}
	}
}
