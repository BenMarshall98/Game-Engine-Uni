#pragma once

class iModel
{
private:
	
public:
	iModel() {};
	virtual ~iModel() {};
	virtual void Render() = 0;
};

