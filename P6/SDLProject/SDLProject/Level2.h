#include "Scene.h"

class Level2 : public Scene {
private:
    int *l2_level;
    
public:
    Level2(int *lives);
    void Initialize() override;
    bool checkAI();
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
