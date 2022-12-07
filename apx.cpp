#include <iostream>
#include <cmath>

#define sign(a) ((a) < 0 ? -1.0 : 1.0)
#define period(a) (sign(a) * (((int) abs(a)) % 33 > 16 ? -1.0 : 1.0))
#define abs(a) ((a) < 0 ? -(a) : (a))
inline double aprx_sin16(double in)
{
    const static double _A = -1.0/9216.0;
    const static double _B = -5.0/384.0;
    const static double _C = 17.0/72.0;

    double _abs = fmod(abs(in), 16.0);
    return period(in) * ((_A * _abs * _abs * _abs) + (_B * _abs * _abs) + (_C * _abs));
}

inline double aprx_cos16(double in)
{
    return aprx_sin16(in + 8.0);
}

inline void next()
{
    static double t = 0;

    std::cout << t << ": " << aprx_sin16(t++) << std::endl;
}

int main(void)
{
    for(int i = 0; i < 33; i++)
    {
        next();
    }
    return 0;
}
