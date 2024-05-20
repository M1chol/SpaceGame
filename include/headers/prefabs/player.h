class PlayerObject : public Object
{
    public:
        PlayerObject();
        ~PlayerObject();
        void update() override;
    private:
        SDL_Event b;
};