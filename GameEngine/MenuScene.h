#pragma once

#include "iScene.h"
#include <vector>
#include "MenuItem.h"
#include "glm/glm.hpp"

class MenuScene : public iScene
{
private:
	std::vector<MenuItem *> menuItems;
	glm::vec3 backgroundColour = glm::vec3(0);

public:
	MenuScene();
	~MenuScene();

	void Update() override;
	void Render() override;

	void Load() override;
	void Close() override;

	void Resize(int width, int height) override;
};

