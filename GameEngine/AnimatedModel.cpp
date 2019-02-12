#define GLM_ENABLE_EXPERIMENTAL

#include "AnimatedModel.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

void AnimatedModel::LoadModel()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh & mesh = meshes.at(i);

		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(5, mesh.VBO);
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

		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ivec4) * mesh.ids.size(), &mesh.ids[0], GL_STATIC_DRAW);

		glVertexAttribIPointer(3, 4, GL_INT, 4 * sizeof(int), (void*)nullptr);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO[4]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * mesh.weights.size(), &mesh.weights[0], GL_STATIC_DRAW);

		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
}

void AnimatedModel::Render(Shader * shader)
{
	shader->UseShader();

	if (boneMats.size() == 0)
	{
		for (int i = 0; i < 100; i++)
		{
			boneMats.push_back(mat4(1.0));
		}
	}

	for (int i = 0; i < boneMats.size(); i++)
	{
		string bone = "Bones[" + to_string(i) + ']';
		int boneLocation = glGetUniformLocation(shader->ShaderID(), bone.c_str());

		if (boneLocation != -1)
		{
			int j = 0;
		}
		glUniformMatrix4fv(boneLocation, 1, GL_TRUE, value_ptr(boneMats[i]));
	}

	for (int i = 0; i < meshes.size(); i++)
	{
		glBindVertexArray(meshes.at(i).VAO);
		glDrawElements(GL_TRIANGLES, meshes.at(i).indices.size(), GL_UNSIGNED_INT, nullptr);
	}
}

aiMatrix4x4 AnimatedModel::GLMMat4ToAi(mat4 mat)
{
	return aiMatrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3],
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

mat4 AnimatedModel::AiToGLMMat4(aiMatrix4x4& mat)
{
	mat4 tmp;
	tmp[0][0] = mat.a1;
	tmp[1][0] = mat.b1;
	tmp[2][0] = mat.c1;
	tmp[3][0] = mat.d1;

	tmp[0][1] = mat.a2;
	tmp[1][1] = mat.b2;
	tmp[2][1] = mat.c2;
	tmp[3][1] = mat.d2;

	tmp[0][2] = mat.a3;
	tmp[1][2] = mat.b3;
	tmp[2][2] = mat.c3;
	tmp[3][2] = mat.d3;

	tmp[0][3] = mat.a4;
	tmp[1][3] = mat.b4;
	tmp[2][3] = mat.c4;
	tmp[3][3] = mat.d4;

	return tmp;
}

AnimatedModel::~AnimatedModel()
{

}

mat4 AnimatedModel::GetBoneParentTransforms(Bone * bone)
{
	Bone * parent = bone->parent_bone;

	vector<mat4> mats;

	while (parent != nullptr)
	{
		mat4 tmp = AiToGLMMat4(parent->node->mTransformation);
		mats.push_back(tmp);

		parent = parent->parent_bone;
	}

	mat4 concatenated_transforms = mat4(1.0);

	for (int i = mats.size() - 1; i >= 0; i--)
	{
		concatenated_transforms *= mats.at(i);
	}

	return concatenated_transforms;
}

Bone * AnimatedModel::FindBone(string name)
{
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones.at(i)->name == name)
		{
			return bones.at(i);
		}
	}

	return nullptr;
}

void AnimatedModel::UpdateBoneMatsVector()
{
	if (bones.size() == 0)
	{
		return;
	}

	boneMats.clear();

	for (int i = 0; i < 100; i++)
	{
		if (i > bones.size() -1)
		{
			boneMats.push_back(mat4(1.0));
		}
		else
		{
			aiMatrix4x4 preTransform = bones.at(i)->node->mTransformation;
			mat4 transform = AiToGLMMat4(preTransform);
			mat4 tmp = mat4(1.0);
			tmp *= bones.at(i)->offset_matrix;
			tmp *= transform;
			tmp *= GetBoneParentTransforms(bones.at(i));
			tmp *= globalInverse;

			boneMats.push_back(tmp);
		}
	}
}

void AnimatedModel::Update()
{
	UpdateBoneMatsVector();

	Animation animation = GetFirstAnimation();

	time += (1.0 / 60.0);
	//time = 0;

	if (time < animation.startTime)
	{
		time = animation.startTime;
	}

	if (time > animation.endTime)
	{
		time = animation.startTime;
	}

	for (int i = 0; i < bones.size(); i++)
	{
		UpdateKeyframeTransform(bones.at(i), time);
	}
}

unsigned int AnimatedModel::FindPosition(Bone * bone, float time)
{
	for (unsigned int i = 0; i < bone->animNode->mNumPositionKeys - 1; i++)
	{
		if (time < (float)bone->animNode->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

unsigned int AnimatedModel::FindRotation(Bone * bone, float time)
{
	for (unsigned int i = 0; i < bone->animNode->mNumRotationKeys - 1; i++)
	{
		if (time < (float)bone->animNode->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	return 0;
}

vec3 AnimatedModel::CalcInterpolatedPosition(Bone * bone, float time)
{
	if (bone->animNode->mNumPositionKeys == 1)
	{
		aiVector3D aiPos = bone->animNode->mPositionKeys[0].mValue;
		vec3 pos(aiPos.x, aiPos.y, aiPos.z);
		return pos;
	}

	unsigned int PositionIndex = FindPosition(bone, time);
	unsigned int NextPositionIndex = (PositionIndex + 1);

	float pos1Time = (float)bone->animNode->mPositionKeys[PositionIndex].mTime;
	float pos2Time = (float)bone->animNode->mPositionKeys[NextPositionIndex].mTime;

	float DeltaTime = pos2Time - pos1Time;
	float Factor = (time - pos1Time) / DeltaTime;

	const aiVector3D StartPosition = bone->animNode->mPositionKeys[PositionIndex].mValue;
	const aiVector3D EndPosition = bone->animNode->mPositionKeys[NextPositionIndex].mValue;

	vec3 p1(StartPosition.x, StartPosition.y, StartPosition.z);
	vec3 p2(EndPosition.x, EndPosition.y, EndPosition.z);

	vec3 pos = mix(p1, p2, Factor);

	return pos;
}

quat AnimatedModel::CalcInterpolatedRotation(Bone * bone, float time)
{
	if (bone->animNode->mNumRotationKeys == 1)
	{
		aiQuaternion aiRot = bone->animNode->mRotationKeys[0].mValue;
		quat rot(aiRot.w, aiRot.x, aiRot.y, aiRot.z);
		return rot;
	}

	unsigned int RotationIndex = FindRotation(bone, time);
	unsigned int NextRotationIndex = (RotationIndex + 1);

	float rot1Time = (float)bone->animNode->mRotationKeys[RotationIndex].mTime;
	float rot2Time = (float)bone->animNode->mRotationKeys[NextRotationIndex].mTime;

	float DeltaTime = rot2Time - rot1Time;
	float Factor = (time - rot1Time) / DeltaTime;

	const aiQuaternion& StartRotation = bone->animNode->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotation = bone->animNode->mRotationKeys[NextRotationIndex].mValue;

	quat r1(StartRotation.w, StartRotation.x, StartRotation.y, StartRotation.z);
	quat r2(EndRotation.w, EndRotation.x, EndRotation.y, EndRotation.z);

	quat rot = slerp(r1, r2, Factor);

	return rot;
}

void AnimatedModel::UpdateKeyframeTransform(Bone * bone, float time)
{
	if (bone->animNode == nullptr)
	{
		return;
	}

	vec3 pos = CalcInterpolatedPosition(bone, time);
	quat rot = CalcInterpolatedRotation(bone, time);
	vec3 sca(1.0);

	mat4 mat(1.0);
	mat *= translate(pos);
	mat *= mat4_cast(rot);
	mat *= scale(sca);

	mat = transpose(mat);

	bone->node->mTransformation = GLMMat4ToAi(mat);
}