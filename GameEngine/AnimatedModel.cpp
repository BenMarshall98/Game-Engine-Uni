#include "AnimatedModel.h"
#include "glad.h"
#include "glm/gtc/type_ptr.hpp"


void AnimatedModel::Load()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(5, VBO);
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

void AnimatedModel::Render(Shader * shader)
{
	Update();

	int boneMatrixLocation = glGetUniformLocation(shader->ShaderID(), "BoneMatrix");
	glUniformMatrix4fv(boneMatrixLocation, boneMatrix.size(), GL_FALSE, value_ptr(boneMatrix[0]));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

AnimatedModel::~AnimatedModel()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(5, VBO);
	glDeleteBuffers(1, &EBO);
}

void AnimatedModel::UpdateBoneMatrixVector()
{
	boneMatrix.clear();

	for (int i = 0; i < 100; i++)
	{
		if (i > bones.size() - 1)
		{
			boneMatrix.push_back(mat4(1.0));
		}
		else
		{
			mat4 transforms = GetBoneParentTransform(bones.at(i)->parent) * AiMatrixtoGLMMatrix(bones.at(i)->node->mTransformation);

			boneMatrix.push_back(globalInverse * transforms * bones.at(i)->offsetMatrix);
		}
	}
}

mat4 AnimatedModel::GetBoneParentTransform(Bone * currentBone)
{
	if (currentBone == nullptr)
	{
		return mat4(1.0);
	}

	if (currentBone->parent == nullptr)
	{
		return AiMatrixtoGLMMatrix(currentBone->node->mTransformation);
	}
	else
	{
		return AiMatrixtoGLMMatrix(currentBone->node->mTransformation) * GetBoneParentTransform(currentBone->parent);
	}
}

void AnimatedModel::Update()
{
	UpdateBoneMatrixVector();
}