#include "Wanderer.h"


Wanderer::Wanderer()
{

    loadWalls(file);
    CalcWallPos();

}

Wanderer::~Wanderer()
{
}

void Wanderer::Tick(float deltaTime)
{
	ExampleGO::Tick(deltaTime);
    static enum class State { Idle, Moving, Reached } currentState = State::Idle;
    //finite state machine
    if (fileLoaded) {

        switch (currentState)
        {
        case State::Idle:
            calculateWanderDirection();
            if (availWallLoc.size() > 0) {
                int randomIndex = rand() % availWallLoc.size();
                glm::vec3 newTarget = availWallLoc[randomIndex];

                // Ensure it doesn't go back to the previous position
                if (newTarget != m_prevPos) {
                    m_targetPos = newTarget;
                    currentState = State::Moving;
                }
            }
            else {
                //std::cout << "No available positions to wander to." << std::endl;
            }
            break;

        case State::Moving:
        {
            glm::vec3 direction = m_targetPos - m_pos;
            float distance = glm::length(direction);

            if (distance > 0.1f) { // Threshold to stop moving
                direction /= distance; // Normalize the direction
                m_pos += direction * deltaTime; // Move towards the target
            }
            else {
                m_prevPos = m_pos; // Update previous position
                m_pos = m_targetPos; // Snap to target position
                currentState = State::Reached;
            }
        }
        break;

        case State::Reached:
            currentState = State::Idle; // Transition back to Idle to calculate a new target
            break;
        }
    }
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
        //std::cout << "Reading line: " << line << std::endl; // Debug output

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
            //std::cout << "Level name: " << currentName << std::endl; // Debug output
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

                /*std::cout << "Parsed row " << rowIdx << ": ";
                for (int i = 0; i < colIdx; ++i) {
                    std::cout << currentLevel[rowIdx][i] << " ";
                }
                std::cout << std::endl;
                */
                rowIdx++;
            }
        }
    }

    // Save the last level if the file ends without a closing bracket  
    if (readingLevel && rowIdx > 0) {
        if (currentName == "LevelLayout") {
            LevelLayout.push_back(currentLevel);
        }
        else {
            levels.push_back(currentLevel);
        }
    }

    if (file.eof()) {
        fileLoaded = true;
        std::cout << "<<<<<<<<<<<File Read complete>>>>>>>>>>>>>>\n";
    }

    std::cout << "Final LevelLayout: " << std::endl;
    for (const auto& level : LevelLayout) {
        for (const auto& row : level) {
            for (const auto& cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
    }
	CalcWallPos();
}
   

void Wanderer::CalcWallPos()
{
    glm:vec3 origin(0.0f, 0.0f, 0.0f);
    int rows = levelLayout.size();
    int cols = levelLayout[0].size();
    float offsetX = (cols - 1) * 2.5f;
    float offsetZ = (rows - 1) * 2.5f;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            glm::vec3 newOrigin(origin.x + j * 5.0f - offsetX, origin.y, origin.z + i * 5.0f - offsetZ);
            if (levelLayout[i][j] >= 0 && levelLayout[i][j] < levels.size()) {
                std::cout << "Generating floor: " << levelLayout[i][j]
                    << " at Origin: (" << newOrigin.x << ", " << newOrigin.y << ", " << newOrigin.z << ")" << std::endl;
					Wallloc.push_back(newOrigin);
            }

        }
    }

    
}
void Wanderer::calculateWanderDirection()
{
    // Get the position from the GameObject  
    glm::vec3 m_pos = GetPos();
    // Store current position as the previous position  
    m_prevPos = m_pos;

    int gridX = static_cast<int>(m_pos.x);
    int gridZ = static_cast<int>(m_pos.z);

    // Define a larger radius for wandering  
    int wanderRadius = 2; // Increase radius to explore further  

    // Define the positions around the current position within the radius  
    std::vector<glm::vec3> localPositions;

    for (int offsetZ = -wanderRadius; offsetZ <= wanderRadius; ++offsetZ) {
        for (int offsetX = -wanderRadius; offsetX <= wanderRadius; ++offsetX) {
            int localX = gridX + offsetX;
            int localZ = gridZ + offsetZ;

            // Ensure the local position is within the 5x5 grid bounds  
            if (localX >= 0 && localX < 5 && localZ >= 0 && localZ < 5) {
                localPositions.emplace_back(localX, 0.0f, localZ);
            }
        }
    }

    availWallLoc.clear(); // Clear previous available positions  

    for (const auto& pos : localPositions) {
        bool isWall = false; // Flag to check if the position is a wall  
        for (const auto& wall : Wallloc) {
            if (pos.x == wall.x && pos.z == wall.z) {
                // Wall found at this position  
                isWall = true; // Mark as a wall  
                break; // No need to check further walls  
            }
        }
        if (!isWall && pos != m_prevPos) {
            // No wall at this position and not the previous position, add to available positions  
            availWallLoc.push_back(glm::vec3(pos.x, m_pos.y, pos.z));
        }
    }
}
  
