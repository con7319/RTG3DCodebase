#include "LevelGen.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include <array> 
#include <iostream>


std::array<std::array<int, 5>, 5> Level1 = { {

    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1}
} };

LevelGen::LevelGen(Scene* scene)
{
    m_ShaderProg = scene->GetShader(m_ShaderName)->GetProg();
    m_texture = scene->GetTexture(m_TexName)->GetTexID();
    m_model = scene->GetModel(m_ModelName);

    generateLevel(Level1);
}

LevelGen::~LevelGen()
{

}

void LevelGen::generateLevel(const std::array<std::array<int, 5>, 5>& levelMatrix) {

    clearLevel();
    int rows = levelMatrix.size();
    int cols = levelMatrix[0].size();
    float offsetX = cols / 2.0f;
    float offsetZ = rows / 2.0f;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (levelMatrix[i][j] == 1) { // if its 1 it represents a wall
                glm::vec3 newPos(j - offsetX, 1, i - offsetZ);
                std::cout << "Calculated position: (" << newPos.x << ", " << newPos.y << ", " << newPos.z << ")\n";
                ExampleGO* wall = new ExampleGO(); // Create a new Model instance
                wall->SetModel(m_model); // Copy the model data
                wall->SetPos(newPos); // Set the position of the wall
                Walls.push_back(*wall); // Push the dereferenced wall object
                std::cout << "Model added at position: (" << wall->GetPos().x << ", " << wall->GetPos().y << ", " << m_model->GetPos().z << ")\n";
            }
        }
    }
}




const std::vector<ExampleGO>& LevelGen::getWalls() const
{
    return Walls;
}

void LevelGen::clearLevel()
{
	Walls.clear();
}