#include "Scene.h"

class Level2 : public Scene {
private:
    int *l2_lives;
public:
    Level2(int *lives);
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
