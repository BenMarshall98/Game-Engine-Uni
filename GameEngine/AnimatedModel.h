#pragma once
#include "iModel.h"
#include "glm/glm.hpp"
#include "assimp/scene.h"
#include "Shader.h"
#include <vector>
#include <string>

using namespace std;
using namespace glm;
using namespace Assimp;

struct Bone
{
	Bone() {}
	Bone& operator=(const Bone&) = delete;
	Bone(Bone&) = delete;
	string name;
	aiNode* node;
	vector<aiNodeAnim*> animNode;
	Bone* parent;
	mat4 parentTransforms;
	mat4 offsetMatrix;
};

struct Animation
{
	string name;
	vector<aiNodeAnim*> nodesAnim;
	double framesPerSecond;
	double frameDuration;
	double timeDuration;
};

class AnimatedModel : public iModel
{
private:
	unsigned int VAO, EBO;
	unsigned int VBO[5];

	double currentTime;

	vector<vec3> vertex;
	vector<vec2> texture;
	vector<vec3> normal;

	vector<int> indices;

	vector<float> weight;
	vector<unsigned int> ID;

	vector<Bone*> bones;
	vector<Animation*> animations;

	vector<aiNode*> nodes;

	mat4 globalInverse;

	unsigned int FindPosition(Bone * bone) const;
	unsigned int FindRotation(Bone * bone) const;

	vec3 CalcInterpolatedPosition(Bone * bone) const;
	quat CalcInterpolatedRotation(Bone * bone) const;

	unsigned int currentAnimation = -1;;

	vector<mat4> boneMatrix;

	void UpdateBoneMatrixVector();
	void UpdateBoneTransform(Bone * bone);
	mat4 GetBoneParentTransform(Bone * currentBone);
	

	static aiMatrix4x4 GLMMatrixtoAiMatrix(mat4& matrix)
	{
		return aiMatrix4x4(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
			matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
			matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
			matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
	}

public:
	
	static mat4 AiMatrixtoGLMMatrix(aiMatrix4x4& matrix)
	{
		mat4 newMatrix;
		newMatrix[0][0] = matrix.a1;
		newMatrix[1][0] = matrix.b1;
		newMatrix[2][0] = matrix.c1;
		newMatrix[3][0] = matrix.d1;

		newMatrix[0][1] = matrix.a2;
		newMatrix[1][1] = matrix.b2;
		newMatrix[2][1] = matrix.c2;
		newMatrix[3][1] = matrix.d2;

		newMatrix[0][2] = matrix.a3;
		newMatrix[1][2] = matrix.b3;
		newMatrix[2][2] = matrix.c3;
		newMatrix[3][2] = matrix.d3;

		newMatrix[0][3] = matrix.a4;
		newMatrix[1][3] = matrix.b4;
		newMatrix[2][3] = matrix.c4;
		newMatrix[3][3] = matrix.d4;

		return newMatrix;
	}

//Make above privates
	AnimatedModel() : VAO(-1), EBO(-1), VBO(), currentTime(0)
	{
		boneMatrix.resize(100);
	}

	AnimatedModel& operator= (const AnimatedModel&) = delete;
	AnimatedModel(AnimatedModel&) = delete;

	inline void AddAnimation(Animation * animation)
	{
		animations.push_back(animation);
	}

	inline void SetGlobalInverse(mat4 & pGlobalInverse)
	{
		globalInverse = pGlobalInverse;
	}

	inline void AddVertex(vec3 & pVertex)
	{
		vertex.push_back(pVertex);
	}

	inline void AddTexture(vec2 & pTexture)
	{
		texture.push_back(pTexture);
	}

	inline void AddNormal(vec3 & pNormal)
	{
		normal.push_back(pNormal);
	}

	inline void AddIndex(int pIndex)
	{
		indices.push_back(pIndex);
	}

	inline void ResizeIDs(int pSize)
	{
		ID.resize(pSize);
	}

	inline void ResizeWeights(int pSize)
	{
		weight.resize(pSize);
	}

	inline void SetWeightAt(int pIndex, float pWeight)
	{
		weight[pIndex] = pWeight;
	}

	inline void SetIDAt(int pIndex, unsigned int pID)
	{
		ID[pIndex] = pID;
	}

	inline float GetWeightAt(int pIndex)
	{
		return weight[pIndex];
	}

	inline int GetAnimationCount() const
	{
		return animations.size();
	}

	inline Animation * GetAnimation(int pIndex)
	{
		return animations[pIndex];
	}

	inline int GetBoneCount() const
	{
		return bones.size();
	}

	inline void AddBone(Bone * pBone)
	{
		bones.push_back(pBone);
	}

	inline Bone * GetBone(int pIndex)
	{
		return bones[pIndex];
	}

	inline void AddNode(aiNode * pNode)
	{
		nodes.push_back(pNode);
	}

	inline aiNode * GetNode(int pIndex)
	{
		return nodes[pIndex];
	}

	inline int GetNodeCount() const
	{
		return nodes.size();
	}

	~AnimatedModel();

	void Load();
	void Update();

	void Render(Shader * shader) const override;
};

