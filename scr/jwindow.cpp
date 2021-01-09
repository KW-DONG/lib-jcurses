#include "jwindow.hpp"
#include <stdlib.h>
#include <cstring>

void j_init()
{
    initscr();
    //curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr,TRUE);
}

void JWindow::post_frame(void)
{
    /*display title*/
    int length = strlen(title);
    float temp = (w - length)/ 2;

    /*create window*/
    base_window = newwin(h,w,y,x);

    wborder(base_window, '|', '|', '-', '-', '+', '+', '+', '+');

    mvwprintw(base_window, 0, (int)temp-1, "[%s]", title);

    keypad(base_window, TRUE);
}

const char* JWidget::get_feedback(int32_t feedback, event_feedback_t* messageList)
{
    static const char noFeedback[] = "no feedback";

    int32_t messageNum = sizeof(messageList);

    for (int32_t i = 0; i < messageNum; i++)
    {
        if (feedback == i)
        {
            return messageList[i].message;
        }
    }
    return noFeedback;
}

void j_print(const char* content)
{
    mvprintw(0,0,content);
}

