#include "Scene.h"

class Level3 : public Scene {
private:
    int *l3_level;
    
public:
    Level3(int *lives);
    void Initialize() override;
    bool checkAI();
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
