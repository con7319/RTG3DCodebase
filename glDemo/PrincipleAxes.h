#pragma once

#include "core.h"
#include "AIModel.h"


class CGPrincipleAxes: public AIModel  {

private:

	

public:

	CGPrincipleAxes();
	~CGPrincipleAxes();

	void Render();
	void Load(ifstream& _file);
};
