#include "AnimatedModel.h"
#include "glm/gtc/type_ptr.hpp"

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
	Update();

	int boneLocation = glGetUniformLocation(shader->ShaderID(), "Bones");
	glUniformMatrix4fv(boneLocation, boneMats.size(), GL_FALSE, value_ptr(boneMats[0]));

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

	mat4 concatenated_transforms = mat4();

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
		if (i > 1)
		{
			boneMats.push_back(mat4(1.0));
		}
		else
		{
			mat4 concatenated_transformation = GetBoneParentTransforms(bones.at(i)) *
				AiToGLMMat4(bones.at(i)->node->mTransformation);

			boneMats.push_back(globalInverse *
				concatenated_transformation * bones.at(i)->offset_matrix);
		}
	}
}

void AnimatedModel::Update()
{
	UpdateBoneMatsVector();
}