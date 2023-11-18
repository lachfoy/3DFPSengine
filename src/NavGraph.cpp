#include "Common.h"
#include "NavGraph.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include "DebugRenderer.h"

void NavGraph::Load(const std::string& path)
{
	// Parse the nav graph file
	// the format is:
	// Vertex data starts with "# Vertices", then each line is read as "index x y z"
	// Edge data starts with "# Edges", then each line is read as "to from"
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
			Vertex v;
			if (iss >> v.id >> v.x >> v.y >> v.z)
			{
				m_vertices.push_back(v);
			}

			break;
		case NavFileParseMode::PARSING_EDGES:
			Edge e;
			if (iss >> e.from >> e.to)
			{
				m_edges.push_back(e);
			}
			break;
		default:
			ASSERT_MSG(false, "File format error parsing nav file \"%s\"", path.c_str());
		}
	}
}

void NavGraph::DebugDraw()
{
	for (const Edge& edge : m_edges)
	{
		// Ensure the edge indices are valid
		if (edge.from >= 0 && edge.from < m_vertices.size() &&
			edge.to >= 0 && edge.to < m_vertices.size())
		{
			const Vertex& fromVertex = m_vertices[edge.from];
			const Vertex& toVertex = m_vertices[edge.to];

			glm::vec3 from(fromVertex.x, fromVertex.y, fromVertex.z);
			glm::vec3 to(toVertex.x, toVertex.y, toVertex.z);

			gDebugRenderer.AddLine(from, to, glm::vec3(0.0f, 1.0f, 1.0f));
		}
	}
}
