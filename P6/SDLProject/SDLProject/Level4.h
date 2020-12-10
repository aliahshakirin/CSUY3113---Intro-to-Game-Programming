#include "Scene.h"

class Level4 : public Scene {
private:
    int *l4_level;
    
public:
    Level4(int *lives);
    void Initialize() override;
    bool checkAI();
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
