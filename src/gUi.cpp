#include "engine.h"

#pragma region uiSphere

uiSphere::uiSphere(Scene *scene, int newRes, float newRad) : Object(scene) 
{
    radius = newRad;
    resolution = newRes;
    rotation = M_PI / resolution;
    color = {255, 255, 255, 255};
    borderColor = {0, 0, 0, 255};
    vertices = new SDL_Vertex[this->resolution];
    nrOfIndexes = resolution + (resolution - 3) * 2;
    indexes = generateIndexes();
    log(LOG_INFO) << "uiRoundedRect created\n";
}

void uiSphere::render(){
    Object::render();
    for(int i=0; i < resolution; i++){
        SDL_Vertex point = getSpherePosition(i);
        point.position.x += pos.x;
        point.position.y += pos.y;
        vertices[i] = point;
    }
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderGeometry(gRenderer, NULL, vertices, resolution, indexes, nrOfIndexes);
}

SDL_Vertex uiSphere::getSpherePosition(int index){
    float angle = 2 * M_PI / resolution * index + rotation;
    return {sin(angle) * radius, cos(angle) * radius, color};
}

int *uiSphere::generateIndexes(){
    int *indexes = new int[nrOfIndexes];
    int j = 0;
    indexes[j++] = 0;
    indexes[j++] = 1;
    indexes[j++] = 2;

    for(int i=2; i < resolution - 1; i++){
        indexes[j++] = 0;
        indexes[j++] = i;
        indexes[j++] = i+1;
    }

    return indexes;
}


#pragma endregion

#pragma region uiRoundedRect

uiRoundedRect::uiRoundedRect(Scene *scene, int newRes, float radius, float width, float height) : uiSphere(scene, newRes, radius)
{
    resolution = newRes;
    this->width = width;
    this->height = height;
    log(LOG_INFO) << "uiRoundedRect created\n";
}

void uiRoundedRect::render(){
    Object::render();

    Vect centers[4] = {
        {width - radius, height - radius},
        {width - radius, radius},
        {radius, radius},
        {radius, height - radius}
    };

    for(int i=0; i < resolution; i++){
        SDL_Vertex point = getSpherePosition(i);
        point.position.x += pos.x + centers[i * 4 / resolution].x;
        point.position.y += pos.y + centers[i * 4 / resolution].y;
        vertices[i] = point;
    }
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderGeometry(gRenderer, NULL, vertices, resolution, indexes, nrOfIndexes);
}

#pragma endregion

MainMenu::MainMenu()
{
    Scene *uiScene = addScene("UI");
    uiScene->setDrawPriority(1);
    Object *title = new Object(uiScene);
    title->setName("title");
    title->move({(double)SCREEN_WIDTH / 2, (double)SCREEN_HEIGHT / 4});
    TextComponent *text = new TextComponent("Scrap Wars", {0, 0}, fontSans, title);
    CustomUpdateComponent *update = new CustomUpdateComponent(title);
    update->setUpdateFunction([](CustomUpdateComponent *comp) -> bool
                              {
        static double angle = 0;
        static bool increasing = true;

        if (increasing)
        {
            angle += .1;
            if (angle >= 10)
            increasing = false;
        }
        else
        {
            angle -= .1;
            if (angle <= -10)
            increasing = true;
        }

        comp->getParent()->rotate(angle);
        return true; });

    text->setScale(2.0);

    // uiSphere *sphere = new uiSphere(uiScene, 12, 100);
    // sphere->setName("sphere");
    // sphere->move({(double)SCREEN_WIDTH / 2, (double)SCREEN_HEIGHT * 2/ 3});

    uiRoundedRect *button = new uiRoundedRect(uiScene, 12, 40, 500, 300);
    button->setName("button");
    button->move({(double)SCREEN_WIDTH / 2, (double)SCREEN_HEIGHT / 2});
    // CustomUpdateComponent *updateButton = new CustomUpdateComponent(button);
    // updateButton->setUpdateFunction([](CustomUpdateComponent *comp) -> bool
    // {
    //     static float width = 0;
    //     static bool increasing = false;
    //     if(increasing){
    //         width += .1;
    //         if(width > 50)
    //             increasing = false;
    //     }else{
    //         width -= .1;
    //         if(width < -50)
    //             increasing = true;
    //     }
    //     static_cast<uiRoundedRect *>(comp->getParent())->setRoration(width);   ;
    //     return true;
    // });
};
