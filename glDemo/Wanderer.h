#ifndef WANDERER_H  
#define WANDERER_H  

#include "ExampleGO.h" 
#include <fstream>
#include <sstream>  
#include <iostream>  
#include <vector>

class Wanderer : public ExampleGO {  
public:  
  // Constructor  
  Wanderer();  

  // Destructor  
  virtual ~Wanderer();  

  // Update method for wandering behavior  
  void Tick(float deltaTime) override;  


  

private:  
  // Helper method to calculate random movement  
  void calculateWanderDirection(); 
  void loadWalls(const std::string& filename);
  void CalcWallPos();

  // Variables for wandering logic  
  float wanderRadius;  
  float wanderDistance;  
  float wanderJitter;  
  glm::vec2 currentDirection;

  std::array<std::array<int, 5>, 5> levelLayout;
  std::vector<std::array<std::array<int, 5>, 5>> LevelLayout;
  std::vector<std::string> levelNames;
  std::vector<std::array<std::array<int, 5>, 5>> levels;
  std::vector<vec3> Wallloc;
   
  glm::vec3 m_prevPos;
  glm::vec3 m_targetPos;
    
};  

#endif // WANDERER_H  
