#include "jform.hpp"
#include "snake.hpp"
#include <string>

int32_t Field1_Push(std::string&);
int32_t Field1_Pull(std::string&);

int32_t Field2_Push(std::string&);
int32_t Field2_Pull(std::string&);

int32_t Field3_Push(std::string&);
int32_t Field3_Pull(std::string&);

int32_t Item2_Event(JMenu* ptr);
int32_t Item12_Event(JMenu* ptr);

std::string data1("123456");
std::string data2("654321");

int main()
{
    JMENU_BASE(baseMenu,"main");
    JMENU(menu1,"menu1");
    JMENU(menu13,"menu13");

    JForm form1(30,2,20,40,"form1");
    JSnake snake(30,2,20,40,"snake");

    JITEM(item1,"form menu");
    JITEM(item2,"print field2");
    JITEM(item3,"snake");
    JITEM(item4,"nothing");
    JITEM(item5,"nothing1");
    JITEM(item11,"show form");
    JITEM(item12,"print field1");
    JITEM(item13,"something");
    JITEM(item131,"aaa");
    JITEM(item132,"bbb");
    JITEM(item133,"ccc");
    JITEM(item134,"ddd");

    JField field1("field1:");
    JField field2("field2:");
    JField field3("field3:");

    ITEM_SET_MENU(item1,menu1);
    ITEM_SET_EVENT(item2,Item2_Event,NULL);
    ITEM_SET_APP(item11,form1);
    ITEM_SET_EVENT(item12,Item12_Event,NULL);
    ITEM_SET_APP(item3,snake);
    ITEM_SET_MENU(item13,menu13);

    field1.Set_Pull(Field1_Pull,NULL);
    field1.Set_Push(Field1_Push,NULL);
    field2.Set_Pull(Field2_Pull,NULL);
    field2.Set_Push(Field2_Push,NULL);
    field3.Set_Pull(Field3_Pull,NULL);
    field3.Set_Push(Field3_Push,NULL);

    MENU_SET_ITEM(baseMenu,&item1,&item2,&item3,&item4,&item5);
    MENU_SET_ITEM(menu1,&item11,&item12,&item13);
    MENU_SET_ITEM(menu13,&item131,&item132,&item133,&item134);
    
    JField* form1List[] = {&field1,&field2,&field3};
    form1.Set_Fields(form1List,ARRAY_SIZE(form1List));
    
    JInit();
    baseMenu.Display();
    refresh();
    endwin();
    return 0;
}

int32_t Field1_Push(std::string& text)
{
    data1.assign(text.c_str());
}

int32_t Field1_Pull(std::string& text)
{
    text.assign(data1.c_str());
}

int32_t Field2_Push(std::string&  text)
{    
    data2.assign(text.c_str());
}

int32_t Field2_Pull(std::string&  text)
{
    text.assign(data2.c_str());
}

int32_t Field3_Push(std::string&  text)
{    
    data2.assign(text.c_str());
}

int32_t Field3_Pull(std::string&  text)
{
    text.assign(data2.c_str());
}

int32_t Item2_Event(JMenu* ptr)
{
    JPrint(data2.c_str());
}

int32_t Item12_Event(JMenu* ptr)
{
    JPrint(data1.c_str());
}