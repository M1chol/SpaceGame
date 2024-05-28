class PlayerObject : public Object
{
public:
    PlayerObject(Scene *scene);
    ~PlayerObject();
    void update() override;

private:
    SDL_Event b;
};