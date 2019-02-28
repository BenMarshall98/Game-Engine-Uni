#include "Mesh.h"

Mesh::Mesh(vector<vec3> pVertex, vector<vec3> pNormal, vector<vec2> pTextures, vector<vec4> pWeights, vector<ivec4> pIds, vector<int> pIndices) :
	vertex(pVertex), normal(pNormal), textures(pTextures), weights(pWeights), ids(pIds), indices(pIndices), VAO(0), EBO(0)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(5, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertex.size(), &vertex[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * textures.size(), &textures[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * normal.size(), &normal[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ivec4) * ids.size(), &ids[0], GL_STATIC_DRAW);

	glVertexAttribIPointer(3, 4, GL_INT, 4 * sizeof(int), (void*)nullptr);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * weights.size(), &weights[0], GL_STATIC_DRAW);

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(5, VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Render()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}