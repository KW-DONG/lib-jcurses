#ifndef __JWINDOW_HPP
#define __JWINDOW_HPP
#include <ncurses.h>

/**
 * @brief JCurses initialization function
 * @note call it as initialization
 */
void JInit(void);

void JPrint(const char* content);

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
    x(startX),y(startY),h(height),w(width),mTitle(title)
    {
        mBaseWindow = newwin(h,w,y,x);
    }
    ~JWindow()
    {
        //wborder(mBaseWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');
        //wrefresh(mBaseWindow);
        //delwin(mBaseWindow);
    }

    int32_t Get_X(void) {return x;}

    int32_t Get_Y(void) {return y;}

    int32_t Get_W(void) {return w;}

    int32_t Get_H(void) {return h;}

    WINDOW* Get_Base_Window(void)   {return mBaseWindow;}

    void Show(void);            /*show title and box*/

    void Terminal_Print(const char* text);

private:
    
    WINDOW* mBaseWindow;        /*the main window*/

    const int32_t h;            /*current window height*/
    
    const int32_t w;            /*current window width*/
    
    const int32_t x;            /*current window start position*/
    
    const int32_t y;            /*current window start position*/
    
    const char* mTitle;         /*current menu title and item title*/

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
    JWidget(const char* title):mTitle(title),mWidth(0)
    {
        for (;title[mWidth]!= '\0';mWidth++);
    }
    ~JWidget(){}

    const char* Get_Title(void)
    {
        return mTitle;
    }
    const int32_t Get_Width(void)
    {
        return mWidth;
    }

protected:
    const char* Get_Feedback(int32_t feedback, event_feedback_t* messageList);

private:
    const char* mTitle;
    int32_t mWidth;
};

class JApp : public JWindow
{
public:
    JApp(int32_t startX, int32_t startY, uint32_t height, uint32_t width, const char* title):
    JWindow(startX,startY,height,width,title),refreshBit(true),clearFlag(false){}
    ~JApp(){}

    virtual void Display(){}

    void Base_Print(const char* content)
    {
        mvprintw(LINES-2,0,content);
        clearFlag = true;
    }

    void Base_Clear(void)
    {
        Base_Print("                                    ");
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