#include "Scene.h"

class Level1 : public Scene {
private:
    int *l1_lives;
public:
    Level1(int *lives);
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
