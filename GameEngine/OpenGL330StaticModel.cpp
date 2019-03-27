#include "OpenGL330StaticModel.h"
#include "OpenGL.h"
#include "RenderManager.h"

OpenGL330StaticModel::OpenGL330StaticModel(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec2> & pTexture, const std::vector<glm::vec3> & pNormal, const std::vector<int> & pIndices, const std::vector<glm::vec3> & pTangents) :
	StaticModel(pVertex, pTexture, pNormal, pIndices, pTangents), VAO(0), EBO(0), VBO{ 0,0,0,0,0 }, VAOBuffer(nullptr)
{
	//The following code is based from learnopengl
	glGenVertexArrays(1, &VAO);
	glGenBuffers(4, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mVertex.size(), &mVertex[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mTexture.size(), &mTexture[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mNormal.size(), &mNormal[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mTangents.size(), &mTangents[0], GL_STATIC_DRAW);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	VAOBuffer = new OpenGL330VertexBuffer(VAO);
}

void OpenGL330StaticModel::Render(Shader * shader)
{
	RenderManager::Instance()->BindVertexArray(VAOBuffer);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}

void OpenGL330StaticModel::Render(Shader * shader, const std::vector<glm::mat4> & boneMats)
{
}

OpenGL330StaticModel::~OpenGL330StaticModel()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(1, &EBO);
}
