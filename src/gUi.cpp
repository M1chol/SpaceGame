#include "engine.h"

#pragma region uiRoundedRect

uiRoundedRect::uiRoundedRect(Scene *scene, int newRes, float radius) : Object(scene) 
{
    this->radius = radius;
    if(newRes % 4 != 0){
        log(LOG_WARN) << name << " - resolution is not a multiple of 4\n";
        newRes = newRes - newRes % 4;
        if(newRes < 4){
            newRes = 4;
        }
    }
    this->resolution = newRes;
    color = {255, 255, 255, 255};
    borderColor = {0, 0, 0, 255};
    vertices = new SDL_Vertex[this->resolution];
    nrOfIndexes = resolution + (resolution - 3) * 2;
    indexes = generateIndexes();
    for(int i = 0; i < nrOfIndexes; i++){
        std::cout << indexes[i] << " ";
    }
    log(LOG_INFO) << "uiRoundedRect created\n";
}

void uiRoundedRect::render(){
    Object::render();
    for(int i=0; i < resolution; i++){
        SDL_Vertex point = getPosition(i);
        point.position.x += pos.x;
        point.position.y += pos.y;
        vertices[i] = point;
    }
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    if(SDL_RenderGeometry(gRenderer, NULL, vertices, resolution, indexes, nrOfIndexes) < 0){
        std::cout << "Error rendering geometry: " << SDL_GetError() << std::endl;
    }

    for(int i=0; i < resolution; i++){
        SDL_RenderDrawPoint(gRenderer, vertices[i].position.x, vertices[i].position.y);
    }

}

SDL_Vertex uiRoundedRect::getPosition(int index){
    float angle = 2 * M_PI / resolution * index;
    return {sin(angle) * radius, cos(angle) * radius, color};
}

int *uiRoundedRect::generateIndexes(){
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

    uiRoundedRect *button = new uiRoundedRect(uiScene, 24, 100);
    button->setName("button");
    button->move({(double)SCREEN_WIDTH / 2, (double)SCREEN_HEIGHT / 2});
};
