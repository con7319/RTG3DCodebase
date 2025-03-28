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
    void GenerateGrid(const std::array<std::array<int, 5>, 5>& levelLayout);
    const std::vector<vec3>& getLocations() const;

    std::string m_ModelName = "CUBEHIGH";
    std::string m_ShaderName = "TEXDIR";
    std::string m_TexName = "STARS2";


private:
   

    std::vector<vec3> locations;
    void clearLevel();
};
