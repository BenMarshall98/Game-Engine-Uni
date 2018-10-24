#pragma once

#include <string>

using namespace std;

class iComponent
{
private:

public:
	virtual ~iComponent()
	{
	}
	virtual string GetComponentName() = 0;
	virtual void Swap() = 0;
};

