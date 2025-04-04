#pragma once

#include "core.h"
#include "AIModel.h"

class Cube: public AIModel{

private:

	

public:

	Cube();
	~Cube();

	void Render();
	void Load(ifstream& _file);

};