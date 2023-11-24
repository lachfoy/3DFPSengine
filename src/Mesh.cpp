#include "Mesh.h"

#include <cstdio> // printf
#include <iostream> // cerr
#include <cassert> // assert

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayId);
	glDeleteBuffers(1, &m_vertexBufferId);
}

Mesh* Mesh::CreateQuad(const glm::vec2& size)
{
	float quadVertices[48] = {
		// Position             // Normal        // Texture Coords
		-size.x / 2, -size.y / 2, 0,   0, 0, -1,   0, 0, // Bottom Left
		 size.x / 2, -size.y / 2, 0,   0, 0, -1,   1, 0, // Bottom Right
		 size.x / 2,  size.y / 2, 0,   0, 0, -1,   1, 1, // Top Right

		-size.x / 2, -size.y / 2, 0,   0, 0, -1,   0, 0, // Bottom Left
		 size.x / 2,  size.y / 2, 0,   0, 0, -1,   1, 1, // Top Right
		-size.x / 2,  size.y / 2, 0,   0, 0, -1,   0, 1  // Top Left
	};


	Mesh* mesh = new Mesh();
	mesh->m_numTriangles = 2;
	printf("# of triangles = %d\n", mesh->m_numTriangles);

	// create the actual mesh
	mesh->m_vertexArrayId = 0;
	mesh->m_vertexBufferId = 0;
	glGenVertexArrays(1, &mesh->m_vertexArrayId);

	// bind the mesh data
	glBindVertexArray(mesh->m_vertexArrayId);
	glGenBuffers(1, &mesh->m_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(float), quadVertices, GL_STATIC_DRAW);

	GLsizei stride = (3 + 3 + 2) * sizeof(float); // temp kinda
	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)0);

	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(float) * 3));

	// tex coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(float) * 6));

	// unbind
	glBindVertexArray(0);

	return mesh;
}

void Mesh::Draw() const
{
	glBindVertexArray(m_vertexArrayId);
	glDrawArrays(GL_TRIANGLES, 0, 3 * m_numTriangles);
	glBindVertexArray(0);
}
