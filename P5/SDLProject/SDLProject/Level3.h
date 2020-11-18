#include "Scene.h"

class Level3 : public Scene {
private:
    int *l3_lives;
public:
    Level3(int *lives);
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
