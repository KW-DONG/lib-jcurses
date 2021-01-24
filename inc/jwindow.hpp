#ifndef __JWINDOW_HPP
#define __JWINDOW_HPP
#include <ncurses.h>
#include <cstdlib>
#include <cstdio>
#include <string>

#define PROPERTY_R(variable,type) type get_##variable(void) {return variable;}
#define PROPERTY_W(variable,type) void set_##variable(type value) {variable=value;}

#define PROPERTY_RW(variable,type)\
        PROPERTY_R(variable,type)\
        PROPERTY_W(variable,type)

#define FLAG(flag)\
        void set_##flag##_bit(void)     {flag = true;}\
        void reset_##flag##_bit(void)   {flag = false;}\
        bool get_##flag##_bit(void)     {return flag;}

#define STR2FLT(str)    atof (str)
#define STR2INT(str)    atoi (str)
#define FLT2STR(flt)\
        ({\
            char strBuff[21];\
            sprintf(strBuff,"%.2f",flt);\
            strBuff;\
        })

#define INT2STR(i)\
        ({\
            char strBuff[21];\
            sprintf(strBuff,"%d",i);\
            strBuff;\
        })

/*************Flags****************/
extern int upper_mf;
extern int bottom_mf;

void uprint(const char* text);
void bprint(const char* text, int line);


#define U_CLEAR         uprint("                  ")
#define B_CLEAR         bprint("                  ",2)

#define U_PRINT(text)\
            U_CLEAR;\
            uprint(text);\
            refresh()

#define B_PRINT(text,line)\
            bprint("                  ",line);\
            bprint(text,line);\
            refresh()

/**
 * @brief JCurses initialization function
 * @note call it as initialization
 */
void curses_init(void);

void curses_exit(void);

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

    void post_frame(void);            /*show title and box*/

    void terminal_print(const char* text);

    PROPERTY_R(x,int32_t);
    PROPERTY_R(y,int32_t);
    PROPERTY_R(w,int32_t);
    PROPERTY_R(h,int32_t);
    PROPERTY_R(base_window,WINDOW*);
    
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
    JWindow(startX,startY,height,width,title),jrefresh(true){}
    ~JApp(){}
    virtual void display(){}
    FLAG(jrefresh);

private:
    bool jrefresh;
};

#endif