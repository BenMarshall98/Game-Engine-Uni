#pragma once

#include <string>

using namespace std;

class iComponent
{
private:
	iComponent() {};
public:
	virtual ~iComponent() = 0;
	virtual string GetComponentName() = 0;
	virtual void Swap() = 0;
};

