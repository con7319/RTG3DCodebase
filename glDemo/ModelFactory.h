#pragma once
#include <string>
class AIModel;

//models are the things owned by Game Objects that are rendered at their location
//base factory to create them given a TYPE
class ModelFactory
{
public:

	static AIModel* makeNewModel(std::string _type);
};

