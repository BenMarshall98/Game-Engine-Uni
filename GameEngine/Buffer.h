#pragma once

#include "Source.h"

class Buffer
{
public:
	Buffer();
	~Buffer();

	virtual Source * GenerateSource() = 0;
};