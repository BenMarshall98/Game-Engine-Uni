#pragma once
class iScene
{
public:
	iScene();
	~iScene();

	virtual void Update() = 0;
	virtual void Render() = 0;
};

