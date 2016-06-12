#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "window.hpp"

class System
{
private:
    Window m_window;
    static void Keyboard(unsigned char, int, int);

public:
    System(int, char**);
    ~System() = default;
};
#endif //SYSTEM_HPP
