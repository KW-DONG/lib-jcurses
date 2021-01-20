#include "jform.hpp"
#include "snake.hpp"
#include <string>
#include <unistd.h>
#include <fcntl.h>

int32_t Item12_Event(JMenu* ptr);
int32_t Item13_Event(JMenu* ptr);
int32_t Item14_Event(JMenu* ptr);
int32_t Form_Com(void);

int intNum = 100;
float floatNum = 100.12;
bool boolNum = true;

JFIELD_TRANS_I(int_field,intNum);
JFIELD_TRANS_F(float_field,floatNum);
JFIELD_TRANS_I(bool_field,boolNum);

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0) U_PRINT("Fork Error");
    else if (pid == 0)
    {
        //execve()




    }

    
    JMENU_BASE(baseMenu,"main");
    JMENU(menu1,"menu1");
    JMENU(menu13,"menu13");
    JFORM(form1,"Test Form");

    JSnake snake(30,2,20,40,"snake");

    JITEM(item1,"Next Menu");
    JITEM(item2,"Snake");
    JITEM(item3,"Item3");
    JITEM(item4,"Item4");
    JITEM(item5,"Item5");
    JITEM(item11,"Test Form");
    JITEM(item12,"Print Integer");
    JITEM(item13,"Print Float");
    JITEM(item14,"Print Bool");
    JITEM(item15,"Next Menu");
    JITEM(item131,"aaa");
    JITEM(item132,"bbb");
    JITEM(item133,"ccc");
    JITEM(item134,"ddd");

    JFIELD(int_field,"Integer:");
    JFIELD(float_field,"Float:");
    JFIELD(bool_field,"Bool:");

    ITEM_SET_MENU(item1,menu1);
    ITEM_SET_APP(item2,snake);
    ITEM_SET_APP(item11,form1);
    ITEM_SET_EVENT(item12,Item12_Event,NULL);
    ITEM_SET_EVENT(item13,Item13_Event,NULL);
    ITEM_SET_EVENT(item14,Item14_Event,NULL);
    ITEM_SET_MENU(item15,menu13);

    FORM_SET_FIELD(form1,&int_field,&float_field,&bool_field);
    FORM_SET_IPC(form1,Form_Com);
    MENU_SET_ITEM(baseMenu,&item1,&item2,&item3,&item4,&item5);
    MENU_SET_ITEM(menu1,&item11,&item12,&item13,&item14,&item15);
    MENU_SET_ITEM(menu13,&item131,&item132,&item133,&item134);
    
    j_init();
    baseMenu.display();
    refresh();
    endwin();
    return 0;
}

int32_t Item12_Event(JMenu* ptr)    {   j_print(INT2STR(intNum));}

int32_t Item13_Event(JMenu* ptr)    {   j_print(FLT2STR(floatNum));}

int32_t Item14_Event(JMenu* ptr)    {   j_print(INT2STR(boolNum));}

int32_t Form_Com(void)
{

}