#include "ExampleGO.h"
#include "Scene.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"


class LevelGen : public ExampleGO {
public:
    LevelGen(Scene* scene);
    ~LevelGen();
    void Render();
    void generateLevel(const std::array<std::array<int, 5>, 5>& levelMatrix);
    const std::vector<vec3>& getLocations() const;

private:
    std::string m_ModelName = "CUBEHIGH";
    std::string m_ShaderName = "TEXDIR";
    std::string m_TexName = "STARS2";

    std::vector<vec3> locations;
    void clearLevel();
};
