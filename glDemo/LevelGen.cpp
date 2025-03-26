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

	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

    generateLevel(Level1);
    Render();
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
                locations.push_back(newPos); // Push the dereferenced wall object


            }
        }
    }
   
}

void LevelGen::Render()
{
 
    for (const auto& location : locations)
    {
        m_worldMatrix = glm::translate(mat4(1.0), vec3(location));
        m_worldMatrix = glm::scale(m_worldMatrix, glm::vec3(m_scale));
        GameObject::PreRender();
        ExampleGO::Render();
    }
    
}


const std::vector<vec3>& LevelGen::getLocations() const
{
    return locations;
}

void LevelGen::clearLevel()
{
    locations.clear();
}