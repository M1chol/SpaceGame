#ifndef TARGET_OBJECT_H
#define TARGET_OBJECT_H

#include "engine.h"

class targetObject : public Object
{
public:
    targetObject(Scene *scene, int number);
    void update() override;
    // void destroy() override;
};

#endif