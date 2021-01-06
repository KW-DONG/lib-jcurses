#ifndef __SNAKE_HPP
#define __SNAKE_HPP

#include "jwindow.hpp"

class JSnake : public JApp
{
public:
    JSnake(int32_t startX, int32_t startY, uint32_t height, uint32_t width, const char* title):
    JApp(startX,startY,height,width,title){}
    ~JSnake(){}
    virtual void Display(void);
};

#endif