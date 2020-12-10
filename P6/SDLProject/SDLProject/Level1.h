#include "Scene.h"

class Level1 : public Scene {
private:
    int *l1_level;
    
public:
    Level1(int *lives);
    void Initialize() override;
    bool checkAI();
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
