#pragma once

#include <vector>
#include <string>

struct Vertex
{
	int id; // not really needed, we just index to the vector that we store these
	float x, y, z;
};

struct Edge
{
	int from, to;
};

enum class NavFileParseMode
{
	PARSING_VERTICES,
	PARSING_EDGES,
	NONE
};

class NavGraph
{
public:
	NavGraph() {}
	~NavGraph() = default;

	void Load(const std::string& path);

	void DebugDraw();

private:
	std::vector<Vertex> m_vertices;
	std::vector<Edge> m_edges;

};