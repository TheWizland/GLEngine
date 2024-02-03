#pragma once
class RenderFlags
{
private:
	bool internallyLit = false; //1 if false, -1 if true
public:
	void setInternallyLit(bool internallyLit) { this->internallyLit = internallyLit; }
	bool getInternallyLit() { return internallyLit; }
};

