#pragma once

#include "core.h"
#include "Model.h"

class Cube: public Model{

private:

	

public:

	Cube();
	~Cube();

	void Render();
	void Load(ifstream& _file);

};