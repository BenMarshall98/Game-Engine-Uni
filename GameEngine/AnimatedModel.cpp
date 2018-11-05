#define GLM_ENABLE_EXPERIMENTAL

#include "AnimatedModel.h"
#include "glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"


void AnimatedModel::Load()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(5, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) *vertex.size(), &vertex[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * texture.size(), &texture[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * normal.size(), &normal[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * 4 * ID.size(), &ID[0], GL_STATIC_DRAW);
	
	glVertexAttribIPointer(3, 1, GL_INT, GL_FALSE, (void*)0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * weight.size(), &weight[0], GL_STATIC_DRAW);

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

	/*for (int i = 1; i < boneMatrix.size(); i++)
	{
		boneMatrix.at(i) = mat4(1.0);
	}*/

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
			mat4 backToOrigin = globalInverse * transforms * bones.at(i)->offsetMatrix;
			boneMatrix.push_back(backToOrigin);
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

	if (currentAnimation == -1 && animations.size() > 0)
	{
		currentAnimation = 0;
		currentTime = 0;
	}

	if (currentAnimation != -1)
	{
		currentTime += (1 / 60.0f);

		if (currentTime > animations.at(currentAnimation)->timeDuration)
		{
			currentTime = 0;
			if (currentAnimation + 1 == animations.size())
			{
				currentAnimation = 0;
			}
			else
			{
				currentAnimation++;
			}
		}

		for (int i = 0; i < bones.size(); i++)
		{
			UpdateBoneTransform(bones.at(i));
		}
	}
}

unsigned int AnimatedModel::FindPosition(Bone * bone)
{
	aiNodeAnim* animNode = bone->animNode.at(currentAnimation);

	if (animNode)
	{
		for (int i = 0; i < (animNode->mNumPositionKeys - 1); i++)
		{
			if (currentTime < animNode->mPositionKeys[i + 1].mTime)
			{
				return i;
			}
		}
	}
	return 0;
}

unsigned int AnimatedModel::FindRotation(Bone * bone)
{
	aiNodeAnim* animNode = bone->animNode.at(currentAnimation);

	if (animNode)
	{
		for (int i = 0; i < animNode->mNumRotationKeys - 1; i++)
		{
			if (currentTime < animNode->mRotationKeys[i + 1].mTime)
			{
				return i;
			}
		}
	}
	return 0;
}

vec3 AnimatedModel::CalcInterpolatedPosition(Bone * bone)
{
	aiNodeAnim * node = bone->animNode.at(currentAnimation);
	if (node->mNumPositionKeys == 1)
	{
		aiVector3D vec = bone->animNode.at(currentAnimation)->mPositionKeys[0].mValue;
		return vec3(vec.x, vec.y, vec.z);
	}

	unsigned int PositionIndex = FindPosition(bone);
	unsigned int NextPositionIndex = PositionIndex + 1;

	float timeChange = node->mPositionKeys[NextPositionIndex].mTime
		- node->mPositionKeys[PositionIndex].mTime;

	float interpolation = (currentTime - node->mPositionKeys[PositionIndex].mTime)
		/ timeChange;

	aiVector3D startPos = node->mPositionKeys[PositionIndex].mValue;
	aiVector3D finishPos = node->mPositionKeys[NextPositionIndex].mValue;

	vec3 start(startPos.x, startPos.y, startPos.z);
	vec3 finish(finishPos.x, finishPos.y, finishPos.z);

	return mix(start, finish, interpolation);
}

quat AnimatedModel::CalcInterpolatedRotation(Bone * bone)
{
	aiNodeAnim * node = bone->animNode.at(currentAnimation);
	if (node->mNumRotationKeys == 1)
	{
		aiQuaternion quater = bone->animNode.at(currentAnimation)->mRotationKeys[0].mValue;
		return quat(quater.w, quater.x, quater.y, quater.z);
	}

	unsigned int RotationIndex = FindRotation(bone);
	unsigned int NextRotationIndex = RotationIndex + 1;

	float timeChange = node->mRotationKeys[NextRotationIndex].mTime
		- node->mRotationKeys[RotationIndex].mTime;

	float interpolation = (currentTime - node->mRotationKeys[RotationIndex].mTime)
		/ timeChange;

	aiQuaternion startQuat = node->mRotationKeys[RotationIndex].mValue;
	aiQuaternion finishQuat = node->mRotationKeys[NextRotationIndex].mValue;

	quat start(startQuat.w, startQuat.x, startQuat.y, startQuat.z);
	quat finish(finishQuat.w, finishQuat.x, finishQuat.y, finishQuat.z);

	return slerp(start, finish, interpolation);
}

void AnimatedModel::UpdateBoneTransform(Bone * bone)
{
	if (currentAnimation == -1)
	{
		return;
	}

	if (bone->animNode.at(currentAnimation)->mNumPositionKeys == 0)
	{
		return;
	}

	vec3 position = CalcInterpolatedPosition(bone);
	quat quater = CalcInterpolatedRotation(bone);
	vec3 sca(1.0);

	mat4 mat(1.0);

	mat *= translate(position);
	mat *= mat4_cast(quater);
	mat *= scale(sca);

	bone->node->mTransformation = GLMMatrixtoAiMatrix(mat);
}