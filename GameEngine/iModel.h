#pragma once

class iModel
{
private:
	iModel(void) {};
public:
	virtual void Render(void) = 0;
	virtual ~iModel() = 0;
};

