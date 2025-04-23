#include "LevelGen.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include <array> 
#include <iostream>


std::array<std::array<int, 5>, 5> LevelLayout = { {

    {1, 0, 2, 0, 1},
    {1, 0, 3, 0, 1},
    {2, 5, 2, 5, 2},
    {0, 0, 3, 0, 0},
    {1, 0, 2, 0, 1}
} };
std::array<std::array<int, 5>, 5> Level0 = { {

    {1, 0, 0, 0, 2},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {2, 0, 0, 0, 1}
} };
std::array<std::array<int, 5>, 5> Level1 = { {

    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 2, 2, 2, 1}
} };
std::array<std::array<int, 5>, 5> Level2 = { {

    {2, 1, 0, 1, 2},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {2, 1, 0, 1, 2}
} };
std::array<std::array<int, 5>, 5> Level3 = { {

    {1, 1, 0, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 0, 1, 1}
} };
std::array<std::array<int, 5>, 5> Level4 = { {

    {1, 2, 0, 2, 1},
    {1, 0, 0, 0, 1},
    {0, 0, 1, 0, 0},
    {1, 0, 0, 0, 1},
    {1, 2, 0, 2, 1}
} };
std::array<std::array<int, 5>, 5> Level5 = { {

    {1, 1, 2, 1, 1},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {1, 1, 2, 1, 1}
} };


LevelGen::LevelGen()
{
    /*m_ShaderProg = scene->GetShader(m_ShaderName)->GetProg();
    m_texture = scene->GetTexture(m_TexName)->GetTexID();
    m_model = scene->GetModel(m_ModelName);*/
    

	m_scale = glm::vec3(0.5f, 0.5f, 0.5f);

    /*glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 offset = glm::vec3(5.0f, 0.0f, 5.0f);
    glm::vec3 newOrigin = origin + offset;
    glm::vec3 newOrigin2 = origin - offset;
    generateLevel(Level1, newOrigin);
    generateLevel(Level2, newOrigin2);
    generateLevel(Level3, origin);*/

	GenerateGrid(LevelLayout);
}

LevelGen::~LevelGen()
{
	locations1.clear();
    locations2.clear();
}


void LevelGen::generateLevel(const std::array<std::array<int, 5>, 5>& levelMatrix, glm::vec3& origin) {
    int rows = levelMatrix.size();
    int cols = levelMatrix[0].size();
    float offsetX = cols / 2;
    float offsetZ = rows / 2;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {//0 represents no wall
            if (levelMatrix[i][j] == 1) { // if its 1 it represents a wall with texture 1
                glm::vec3 newPos(j - offsetX + origin.x, origin.y, i - offsetZ + origin.z);
                std::cout << "Calculated position: (" << newPos.x << ", " << newPos.y << ", " << newPos.z << ")\n";
                locations1.push_back(newPos); // Push the dereferenced wall object
            }
            if (levelMatrix[i][j] == 2) { // if its 2 it represents a wall with texture 2
                glm::vec3 newPos(j - offsetX + origin.x, origin.y, i - offsetZ + origin.z);
                std::cout << "Calculated position: (" << newPos.x << ", " << newPos.y << ", " << newPos.z << ")\n";
                locations2.push_back(newPos); // Push the dereferenced wall object
            }
        }
    }    
}
void LevelGen::GenerateGrid(const std::array<std::array<int, 5>, 5>& levelLayout)
{
	int rows = levelLayout.size();
	int cols = levelLayout[0].size();
    float offsetX = (cols - 1) * 2.5f;
    float offsetZ = (rows - 1) * 2.5f;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            ;
            if (levelLayout[i][j] == 0) { // if its 0 generate level 0 object
                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
                generateLevel(Level0, newOrigin);
            } 
			if (levelLayout[i][j] == 1) { // if its 1 generate level 1 object
                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
				generateLevel(Level1, newOrigin);
			}
            if (levelLayout[i][j] == 2) { // if its 2 generate level 2 object
                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
                generateLevel(Level2, newOrigin);
            }
            if (levelLayout[i][j] == 3) { // if its 3 generate level 3 object
                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
                generateLevel(Level3, newOrigin);
            }
            if (levelLayout[i][j] == 4) { // if its 4 generate level 4 object
                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
                generateLevel(Level4, newOrigin);
            }
            if (levelLayout[i][j] == 5) { // if its 5 generate level 5 object
                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
                generateLevel(Level5, newOrigin);
            }
		}
	}
}

void LevelGen::Render()
{
	
	ExampleGO::addNormalMap(ExampleGO::m_NormalMap);
    for (const auto& location : locations1)
    {
		ExampleGO::addTexture(m_texList[0]);
		
        m_worldMatrix = glm::translate(mat4(1.0), vec3(location));
        m_worldMatrix = glm::scale(m_worldMatrix, glm::vec3(m_scale));
        ExampleGO::PreRender();
        ExampleGO::Render();
    }

    for (const auto& location : locations2)
    {
        ExampleGO::addTexture(m_texList[1]);
        ExampleGO::setTexScale(m_ShaderProg, m_texScale);
        m_worldMatrix = glm::translate(mat4(1.0), vec3(location));
        m_worldMatrix = glm::scale(m_worldMatrix, glm::vec3(m_scale));
        ExampleGO::PreRender();
        ExampleGO::Render();
    }

    
}


const std::vector<vec3>& LevelGen::getLocations1() const
{
    return locations1;
}

const std::vector<vec3>& LevelGen::getLocations2() const
{
    return locations2;
}

void LevelGen::clearLevel()
{
    locations1.clear();
    locations2.clear();
}