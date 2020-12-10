#include "Scene.h"

class MainWorld : public Scene {
private:
    int *mw_level;
public:
    MainWorld(int *level);
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
