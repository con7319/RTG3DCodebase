#include "Wanderer.h"


Wanderer::Wanderer()
{

	
	
	string file = "Levels.txt";
	loadWalls(file);
}

Wanderer::~Wanderer()
{
}

void Wanderer::Tick(float deltaTime)
{


}
void Wanderer::loadWalls(const std::string& filename)
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
                    LevelLayout.push_back(currentLevel);
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
                    LevelLayout.push_back(currentLevel);
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
	CalcWallPos();
}
void Wanderer::CalcWallPos()
{
    // Calculate the wall positions based on the level layout
    for (int i = 0; i < levelLayout.size(); ++i) {
        for (int j = 0; j < levelLayout[i].size(); ++j) {
            if (levelLayout[i][j] == 1) {
                // Wall found, calculate its position
                glm::vec3 wallPos(j * 5.0f, 0.0f, i * 5.0f);
                // Store or use the wall position as needed
				Wallloc.push_back(wallPos);
            }
        }
    }
}
void Wanderer::calculateWanderDirection()  
{  
  
   // Get the position from the Gameobject  
   glm::vec3 m_pos = GetPos();
   //store current pos as a previous pos
   glm::vec3 m_prevPos = m_pos;

   int gridX = static_cast<int>(m_pos.x);  
   int gridY = static_cast<int>(m_pos.z);  

   // Define the 3x3 local positions around the current position  
   std::vector<glm::vec2> localPositions;  

   for (int offsetY = -1; offsetY <= 1; ++offsetY) {  
       for (int offsetX = -1; offsetX <= 1; ++offsetX) {  
           int localX = gridX + offsetX;  
           int localY = gridY + offsetY;  

           // Ensure the local position is within the 5x5 grid bounds  
           if (localX >= 0 && localX < 5 && localY >= 0 && localY < 5) {  
               localPositions.emplace_back(localX, localY);  
           }  
       }  
   }  

   // Debug or use the local positions as needed  
   for (const auto& pos : localPositions) {  
	   for (const auto& wall : Wallloc) {
		   if (pos.x == wall.x && pos.y == wall.y) {
			   // Wall found at this position
			   
		   }
		   else {
			   // No wall at this position, you can move here
			   //add to available positions
		   }
	   }
   }  
}
