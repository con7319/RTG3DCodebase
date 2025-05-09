#include "ExampleGO.h"
#include "Scene.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"


class LevelGen : public ExampleGO {
public:
    LevelGen();
    ~LevelGen();
    void Render();
    void generateLevel(const std::array<std::array<int, 5>, 5>& levelMatrix, glm::vec3& origin);
    void generateLevelNeg(const std::array<std::array<int, 5>, 5>& levelMatrix, glm::vec3& origin);
    void GenerateGrid();
    void generateFloor(const glm::vec3& origin, const std::array<std::array<int, 5>, 5>& floorLayout);
    const std::vector<vec3>& getLocations1() const;
    const std::vector<vec3>& getLocations2() const;
    const std::vector<vec3>& getLocations3() const;
	void LoadLevels(const std::string& filename);

    

    std::string m_ModelName = "CUBEHIGH";
    std::string m_ShaderName = "";
    std::string m_TexName = "";
	float m_texScale = 4.0f;


       
private:
    glm::vec3 m_Floorscale;

    std::vector<vec3> locations1;
    std::vector<vec3> locations2;
    std::vector<vec3> locations3;
    std::array<std::array<int, 5>, 5> levelLayout;
    std::vector<std::array<std::array<int, 5>, 5>> LevelLayout;
    std::vector<std::string> levelNames; 
    std::vector<std::array<std::array<int, 5>, 5>> levels;
    void clearLevel();
};
