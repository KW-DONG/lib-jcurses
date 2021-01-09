#ifndef __JWINDOW_HPP
#define __JWINDOW_HPP
#include <ncurses.h>

#define PROPERTY_R(variable,type) type get_##variable(void) {return variable;}
#define PROPERTY_W(variable,type) void set_##variable(type value) {variable=value;}

#define PROPERTY_RW(variable,type)\
        PROPERTY_R(variable,type)\
        PROPERTY_W(variable,type)

/**
 * @brief JCurses initialization function
 * @note call it as initialization
 */
void j_init(void);

void j_print(const char* content);

/**
 *                 w
 *      +-------[Title]-------+
 *      |                     |
 *      |                     |
 *      |                     |
 *      |                     |
 *      |                     |
 *      +---------------------+
 * @brief Window base class includes size and location
 */
class JWindow
{
public:
    JWindow(int32_t startX, int32_t startY, int32_t height, int32_t width, const char* title):
    x(startX),y(startY),h(height),w(width),title(title)
    {
        base_window = newwin(h,w,y,x);
    }
    ~JWindow()
    {
        //wborder(base_window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
        //wrefresh(base_window);
        //delwin(base_window);
    }

    PROPERTY_R(x,int32_t);

    PROPERTY_R(y,int32_t);

    PROPERTY_R(w,int32_t);

    PROPERTY_R(h,int32_t);

    PROPERTY_R(base_window,WINDOW*);

    void post_frame(void);            /*show title and box*/

    void terminal_print(const char* text);

private:
    
    WINDOW* base_window;        /*the main window*/

    const int32_t h;            /*current window height*/
    
    const int32_t w;            /*current window width*/
    
    const int32_t x;            /*current window start position*/
    
    const int32_t y;            /*current window start position*/
    
    const char* title;         /*current menu title and item title*/

};

typedef struct 
{
    int32_t num;
    const char* message;
}event_feedback_t;

/**
 * @brief widget base class
 */
class JWidget
{
public:
    JWidget(const char* mTitle):title(mTitle),width(0)
    {
        for (;title[width]!= '\0';width++);
    }
    ~JWidget(){}

    PROPERTY_R(title,const char*);

    PROPERTY_R(width,int32_t);

protected:
    const char* get_feedback(int32_t feedback, event_feedback_t* messageList);

private:
    const char* title;
    int32_t width;
};

class JApp : public JWindow
{
public:
    JApp(int32_t startX, int32_t startY, uint32_t height, uint32_t width, const char* title):
    JWindow(startX,startY,height,width,title),refreshBit(true),clearFlag(false){}
    ~JApp(){}

    virtual void display(){}

    void base_print(const char* content)
    {
        mvprintw(LINES-2,0,content);
        clearFlag = true;
    }

    void Base_Clear(void)
    {
        base_print("                                    ");
        refresh();
        Reset_Clear_Flag();
    }

    void Set_Refresh_Bit(void)  {refreshBit = true;}

    void Reset_Refresh_Bit(void){refreshBit = false;}

    void Set_Clear_Flag(void)   {clearFlag = true;}

    void Reset_Clear_Flag(void) {clearFlag = false;}

    bool Get_Refresh_Bit(void)  {return refreshBit;}

    bool Get_Clear_Flag(void)   {return clearFlag;}

private:

    bool refreshBit;

    bool clearFlag;

};



#endif