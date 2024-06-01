#include "engine.h"

class targetObject : public Object
{
public:
    targetObject(Scene *scene, int number);
    ~targetObject();
    void update() override;
};