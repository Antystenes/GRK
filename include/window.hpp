#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

class Window
{
private:
    int width, height;
    std::string name;
public:
    Window() : width(1280), height(720), name("Yolo") {};
    ~Window() = default;
    void Init();
};
#endif //WINDOW_HPP
