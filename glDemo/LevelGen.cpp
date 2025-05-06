#include "LevelGen.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include <array> 
#include <iostream>
#include <sstream>



//std::array<std::array<int, 5>, 5> LevelLayout = { {
//
//    {1, 0, 2, 0, 1},
//    {1, 0, 3, 0, 1},
//    {2, 5, 2, 5, 2},
//    {0, 0, 3, 0, 0},
//    {1, 0, 2, 0, 1}
//} };
//std::array<std::array<int, 5>, 5> Level0 = { {
//
//    {1, 0, 0, 0, 2},
//    {0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0},
//    {2, 0, 0, 0, 1}
//} };
//std::array<std::array<int, 5>, 5> Level1 = { {
//
//    {1, 0, 0, 0, 1},
//    {1, 0, 0, 0, 1},
//    {1, 0, 0, 0, 1},
//    {1, 0, 0, 0, 1},
//    {1, 2, 2, 2, 1}
//} };
//std::array<std::array<int, 5>, 5> Level2 = { {
//
//    {2, 1, 0, 1, 2},
//    {1, 0, 0, 0, 1},
//    {0, 0, 0, 0, 0},
//    {1, 0, 0, 0, 1},
//    {2, 1, 0, 1, 2}
//} };
//std::array<std::array<int, 5>, 5> Level3 = { {
//
//    {1, 1, 0, 1, 1},
//    {1, 0, 0, 0, 1},
//    {1, 0, 0, 0, 1},
//    {1, 0, 0, 0, 1},
//    {1, 1, 0, 1, 1}
//} };
//std::array<std::array<int, 5>, 5> Level4 = { {
//
//    {1, 2, 0, 2, 1},
//    {1, 0, 0, 0, 1},
//    {0, 0, 1, 0, 0},
//    {1, 0, 0, 0, 1},
//    {1, 2, 0, 2, 1}
//} };
//std::array<std::array<int, 5>, 5> Level5 = { {
//
//    {1, 1, 2, 1, 1},
//    {1, 0, 0, 0, 1},
//    {0, 0, 0, 0, 0},
//    {1, 0, 0, 0, 1},
//    {1, 1, 2, 1, 1}
//} };


LevelGen::LevelGen()
{
    /*m_ShaderProg = scene->GetShader(m_ShaderName)->GetProg();
    m_texture = scene->GetTexture(m_TexName)->GetTexID();
    m_model = scene->GetModel(m_ModelName);*/
    

	m_scale = glm::vec3(0.5f, 0.5f, 0.5f);
    //floorLayout = {};
    /*glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 offset = glm::vec3(5.0f, 0.0f, 5.0f);
    glm::vec3 newOrigin = origin + offset;
    glm::vec3 newOrigin2 = origin - offset;
    generateLevel(Level1, newOrigin);
    generateLevel(Level2, newOrigin2);
    generateLevel(Level3, origin);*/
    LoadLevels("Levels.txt");
    generateFloor(glm::vec3(0.0f, -1.0f, 0.0f), floorLayout);
	GenerateGrid();
}

LevelGen::~LevelGen()
{
	locations1.clear();
    locations2.clear();
}



void LevelGen::LoadLevels(const std::string& filename)  
{  
  std::ifstream file(filename);  
  if (!file.is_open()) {  
      std::cerr << "Failed to open file: " << filename << std::endl;  
      return;  
  }  

  std::string line;  
  std::string currentName;  
  std::array<std::array<int, 5>, 5> currentLevel = {};
  int rowIdx = 0;  
  bool readingLevel = false;  
  std::vector<std::array<std::array<int, 5>, 5>> layoutVec;  
 

  levels.clear();  

  while (std::getline(file, line)) {  
      // Trim leading and trailing whitespace  
      line.erase(0, line.find_first_not_of(" \t"));  
      line.erase(line.find_last_not_of(" \t") + 1);  

      if (line.empty()) {  
          continue; // Skip empty lines  
      }  

      if (line[0] == '[') {  
          // Start of a new level definition  
          if (readingLevel && rowIdx > 0) {  
              // Save the previous level if we were reading one  
              if (currentName == "LevelLayout") {  
                  layoutVec.push_back(currentLevel);  
              }  
              else if (currentName == "FloorLayout") { // Added for FloorLayout  
                  floorLayout = currentLevel;  
              }  
              else {  
                  levels.push_back(currentLevel);  
              }  
          }  
          readingLevel = true;  
          rowIdx = 0;  
          currentLevel = {}; // Reset the level to avoid overlapping data  
      }  
      else if (line[0] == ']') {  
          // End of level definition  
          if (readingLevel && rowIdx > 0) {  
              if (currentName == "LevelLayout") {  
                  layoutVec.push_back(currentLevel);  
              }  
              else if (currentName == "FloorLayout") { // Added for FloorLayout  
                  floorLayout = currentLevel;  
              }  
              else {  
                  levels.push_back(currentLevel);  
              }  
          }  
          readingLevel = false;  
      }  
      else if (line.find("NAME:") != std::string::npos) {  
          // Extract level name  
          currentName = line.substr(line.find("NAME:") + 5);  
          // Trim leading and trailing whitespace from name  
          currentName.erase(0, currentName.find_first_not_of(" \t"));  
          currentName.erase(currentName.find_last_not_of(" \t") + 1);  
      }  
      else if (readingLevel && line.find('{') != std::string::npos) {  
          // This is a row of level data  
          if (rowIdx < 5) { // Ensure we don't exceed the array bounds  
              std::stringstream ss(line);  
              char c;  
              int colIdx = 0;  

              // Skip the opening brace  
              ss >> c; // '{'  

              // Read values separated by commas  
              while (colIdx < 5 && ss >> c) {  
                  if (isdigit(c)) {  
                      currentLevel[rowIdx][colIdx++] = c - '0';  
                  }  
                  else if (c == ',') {  
                      // Skip the comma and continue  
                      continue;  
                  }  
                  else if (c == '}') {  
                      // End of row  
                      break;  
                  }  
              }  

              rowIdx++;  
          }  
      }  
  }  

  // If the file doesn't end with a ']' on its own line, check if we need to add the last level  
  if (readingLevel && rowIdx > 0) {  
      if (currentName == "LevelLayout") {  
          layoutVec.push_back(currentLevel);  
      }  
      else if (currentName == "FloorLayout") { // Added for FloorLayout  
          floorLayout = currentLevel;  
      }  
      else {  
          levels.push_back(currentLevel);  
      }  
  }  

  file.close();  

  // If we parsed a LevelLayout, use it for GenerateGrid  
  if (!layoutVec.empty()) {  
      levelLayout = layoutVec[0];  
  }  

  // Use the parsed FloorLayout for generateFloor  
 
}

void LevelGen::generateFloor(const glm::vec3& origin, const std::array<std::array<int, 5>, 5>& floorLayout) 
{
   int rows = floorLayout.size();
   int cols = floorLayout[0].size();
   float offsetX = (cols - 1) * 2.5f;
   float offsetZ = (rows - 1) * 2.5f;

   for (int i = 0; i < rows; ++i) {
       for (int j = 0; j < cols; ++j) {
           glm::vec3 newOrigin(origin.x + j * 5.0f - offsetX, origin.y, origin.z + i * 5.0f - offsetZ);
           if (floorLayout[i][j] >= 0 && floorLayout[i][j] < levels.size()) {
               std::cout << "Generating floor: " << floorLayout[i][j]
                   << " at Origin: (" << newOrigin.x << ", " << newOrigin.y << ", " << newOrigin.z << ")" << std::endl;
               generateLevel(levels[floorLayout[i][j]], newOrigin);
           }
       }
   }
}



//void LevelGen::generateLevel(const std::array<std::array<int, 5>, 5>& levelMatrix, glm::vec3& origin) {
//    int rows = levelMatrix.size();
//    int cols = levelMatrix[0].size();
//    float offsetX = cols / 2;
//    float offsetZ = rows / 2;
//    for (int i = 0; i < rows; ++i) {
//        for (int j = 0; j < cols; ++j) {//0 represents no wall
//            if (levelMatrix[i][j] == 1) { // if its 1 it represents a wall with texture 1
//                glm::vec3 newPos(j - offsetX + origin.x, origin.y, i - offsetZ + origin.z);
//                std::cout << "Calculated position: (" << newPos.x << ", " << newPos.y << ", " << newPos.z << ")\n";
//                locations1.push_back(newPos); // Push the dereferenced wall object
//            }
//            if (levelMatrix[i][j] == 2) { // if its 2 it represents a wall with texture 2
//                glm::vec3 newPos(j - offsetX + origin.x, origin.y, i - offsetZ + origin.z);
//                std::cout << "Calculated position: (" << newPos.x << ", " << newPos.y << ", " << newPos.z << ")\n";
//                locations2.push_back(newPos); // Push the dereferenced wall object
//            }
//        }
//    }    
//}
//void LevelGen::GenerateGrid(const std::array<std::array<int, 5>, 5>& levelLayout)
//{
//	int rows = levelLayout.size();
//	int cols = levelLayout[0].size();
//    float offsetX = (cols - 1) * 2.5f;
//    float offsetZ = (rows - 1) * 2.5f;
//    for (int i = 0; i < rows; ++i) {
//        for (int j = 0; j < cols; ++j) {
//            ;
//            if (levelLayout[i][j] == 0) { // if its 0 generate level 0 object
//                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
//                generateLevel(Level0, newOrigin);
//            } 
//			if (levelLayout[i][j] == 1) { // if its 1 generate level 1 object
//                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
//				generateLevel(Level1, newOrigin);
//			}
//            if (levelLayout[i][j] == 2) { // if its 2 generate level 2 object
//                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
//                generateLevel(Level2, newOrigin);
//            }
//            if (levelLayout[i][j] == 3) { // if its 3 generate level 3 object
//                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
//                generateLevel(Level3, newOrigin);
//            }
//            if (levelLayout[i][j] == 4) { // if its 4 generate level 4 object
//                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
//                generateLevel(Level4, newOrigin);
//            }
//            if (levelLayout[i][j] == 5) { // if its 5 generate level 5 object
//                glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);
//                generateLevel(Level5, newOrigin);
//            }
//		}
//	}
//}
void LevelGen::generateLevel(const std::array<std::array<int, 5>, 5>& levelMatrix, glm::vec3& origin) {
    int rows = levelMatrix.size();
    int cols = levelMatrix[0].size();
    float offsetX = cols / 2.0f;
    float offsetZ = rows / 2.0f;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            glm::vec3 newPos(j - offsetX + origin.x, origin.y, i - offsetZ + origin.z);
            if (levelMatrix[i][j] == 1) {
                locations1.push_back(newPos);
            }
            else if (levelMatrix[i][j] == 2) {
                locations2.push_back(newPos);
            }
        }
    }
}

void LevelGen::GenerateGrid() {
   
   int rows = levelLayout.size();  
   int cols = levelLayout[0].size();  
   float offsetX = (cols - 1) * 2.5f;  
   float offsetZ = (rows - 1) * 2.5f;  

   for (int i = 0; i < rows; ++i) {  
       for (int j = 0; j < cols; ++j) {  
           glm::vec3 newOrigin(j * 5.0f - offsetX, 0.0f, i * 5.0f - offsetZ);  
           if (levelLayout[i][j] >= 0 && levelLayout[i][j] < levels.size()) {  
               std::cout << "Generating Level: " << levelLayout[i][j]  
                         << " at Origin: (" << newOrigin.x << ", " << newOrigin.y << ", " << newOrigin.z << ")" << std::endl;  
               generateLevel(levels[levelLayout[i][j]], newOrigin);  
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