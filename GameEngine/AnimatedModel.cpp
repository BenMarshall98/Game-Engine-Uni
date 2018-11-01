#include "AnimatedModel.h"
#include "glad.h"

AnimatedModel::AnimatedModel()
{
}

AnimatedModel::~AnimatedModel()
{
}

void AnimatedModel::Load()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) *vertex.size(), &vertex[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * texture.size(), &texture[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * normal.size(), &normal[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * ID.size(), &ID[0], GL_STATIC_DRAW);
	
	glVertexAttribIPointer(3, 1, GL_INT, GL_FALSE, (void*)0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * weight.size(), &weight[0], GL_STATIC_DRAW);

	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}
