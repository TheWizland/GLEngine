#pragma once
class RenderFlags
{
public:
	//Assumed that the Renderer will implement these flags.
	bool internallyLit = false;
	bool heightMapped = false;
	bool hasShadows = true; //Are shadows cast on this object?
	bool castsShadows = true; //Does this object cast shadows on other objects?
	bool visible = true;
};

