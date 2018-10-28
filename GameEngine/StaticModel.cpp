#include "StaticModel.h"
#include "glad.h"

StaticModel::StaticModel(vector<vec3> & pVertex, vector<vec2> & pTexture, vector<vec3> & pNormal, vector<int> & pIndices) :
	mVertex(pVertex), mTexture(pTexture), mNormal(pNormal), mIndices(pIndices), VAO(0), EBO(0)
{
	//The following code is based from learnopengl
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mVertex.size(), &mVertex[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * mTexture.size(), &mTexture[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mNormal.size(), &mNormal[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void StaticModel::Render(void)
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}

StaticModel::~StaticModel()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(1, &EBO);
}
