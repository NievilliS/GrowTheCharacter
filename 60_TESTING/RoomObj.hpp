
class roomobj
{
private:
    int x;
    int y;
    int w = 1;
    int h = 1;
    void *parent_ptr;

public:
    roomobj(void *_parent_ptr);
    roomobj(void *_parent_ptr, int _x, int _y);

    virtual void render(void *_console_engine_ptr);
    virtual void physics(unsigned long long _tick);
};
