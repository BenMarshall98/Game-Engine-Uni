#include "AnimatedModel.h"

void AnimatedModel::LoadModel()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh & mesh = meshes.at(i);

		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(3, mesh.VBO);
		glGenBuffers(1, &mesh.EBO);

		glBindVertexArray(mesh.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mesh.vertex.size(), &mesh.vertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * mesh.textures.size(), &mesh.textures[0], GL_STATIC_DRAW);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mesh.normal.size(), &mesh.normal[0], GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
}

void AnimatedModel::Render(Shader * shader) const
{
	for (int i = 0; i < meshes.size(); i++)
	{
		glBindVertexArray(meshes.at(i).VAO);
		glDrawElements(GL_TRIANGLES, meshes.at(i).indices.size(), GL_UNSIGNED_INT, nullptr);
	}
}

AnimatedModel::~AnimatedModel()
{

}
