#ifndef __JMENU_HPP
#define __JMENU_HPP
#include <menu.h>
#include <vector>
#include "jwindow.hpp"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define MIDDLE_WIN_X(x,w) (x+w/2)

template <class T>
class JItem : public JWidget
{
public:
    typedef int32_t (*Item_Sel_Callback)(T* menuPtr);

    JItem(const char*title):JWidget(title),itemEvent(NULL),mMessageList(NULL),mNextMenu(NULL),mNextApp(NULL){}

    ~JItem(){}

    void Set_Event(Item_Sel_Callback func, event_feedback_t* messageList)
    {
        itemEvent = func;

        mMessageList = messageList;
    }

    void Set_Next_Menu(T* next)
    {
        mNextMenu = next;
    }

    void Set_Next_App(JApp* next)
    {
        mNextApp = next;
    }

    const char* Selected(T* menuPtr)
    {
        if (mMessageList!=NULL)
        {
           return get_feedback(itemEvent(menuPtr),mMessageList); 
        }
        else
        {
            itemEvent(menuPtr);
            return "Selected";
        }
    }

    T* Get_Next_Menu()
    {
        return mNextMenu;
    }

    JApp* Get_Next_App()
    {
        return mNextApp;
    }

    Item_Sel_Callback Get_Event(void)
    {
        return itemEvent;
    }

private:

    T* mNextMenu;

    JApp* mNextApp;

    int32_t (*itemEvent)(T* menuPtr);      /*write an event here*/

    event_feedback_t* mMessageList;
};


/**
 * +--------[Title]--------+
 * |    +----------+       |
 * |    |          |       |
 * |    |  Menus   |       |
 * |    |          |       |
 * |    |          |       |
 * |    +----------+       |
 * +-----------------------+
 */

class JMenu : public JWindow
{
public:

    JMenu(int32_t startX, int32_t startY, uint32_t height, uint32_t width, const char* title):
    JWindow(startX,startY,height,width,title),jitems(NULL),item_num(0),initFlag(true),mLengthMax(10){}

    ~JMenu(){}

    void create(void);

    void post(void);

    void close(void);

    virtual void set_jitems(JItem<JMenu>** itemList, int32_t num)
    {
        jitems = itemList;
        item_num = num;
    }

    void set_jmenu_last(JMenu* menu)
    {
        jmenu_last = menu;
    }

    JMenu* get_jmenu_last(void)
    {
        return jmenu_last;
    }

    JItem<JMenu>** get_jitems(void)
    {
        return jitems;
    }

    WINDOW* set_window_menu(void)
    {
        return window_menu;
    }

    MENU* get_menu(void)
    {
        return menu;
    }

    ITEM** get_items(void)
    {
        return items;
    }

    int32_t get_item_num(void)
    {
        return item_num;
    }


protected:

    WINDOW*         window_menu;        /*the window that associate the menu*/

    MENU*           menu;              /*the menu list*/

    ITEM**          items;             /*item list used to allocate memory*/
    
    JItem<JMenu>**  jitems;
    
    int32_t         item_num;

    JMenu*          jmenu_last;

    bool            initFlag;

    int32_t         mLengthMax;

};

class JBaseMenu : public JApp
{
public:
    JBaseMenu(int32_t startX, int32_t startY, uint32_t height, uint32_t width, const char* title):
    JApp(startX,startY,height,width,title),mThisMenu(NULL)
    {
        mThisMenu = (JMenu*)new JMenu(startX,startY,height,width,title);
        mCurrentMenu = mThisMenu;
    }

    ~JBaseMenu()
    {
        delete mThisMenu;
        mThisMenu = NULL;
    }

    void set_jitems(JItem<JMenu>** list, int32_t num)
    {
        mThisMenu->set_jitems(list,num);
    }

    virtual void display(void);

protected:
    void Switch_Forward(JMenu* newMenu)
    {
        clear();
        unpost_menu(mCurrentMenu->get_menu());
        newMenu->set_jmenu_last(mCurrentMenu);
        mCurrentMenu = newMenu;
        Set_Refresh_Bit();
    }

    void Switch_Backward(void)
    {
        clear();
        unpost_menu(mCurrentMenu->get_menu());
        mCurrentMenu = mCurrentMenu->get_jmenu_last();
        Set_Refresh_Bit();
    }

    void Refresh_Menu(void)
    {
        mCurrentMenu->post_frame();
        mCurrentMenu->post();
    }

    void Run_App(JApp* app);

    void Menu_Recursion(void);

    void Sort_Menu(void);
    
    void create(void);

    void Delete_Menu(void);

private:
    JMenu* mCurrentMenu;    //current menu displayed

    JMenu* mThisMenu;       //base menu

    std::vector<JMenu*> mTree;
};

#define JITEM(objName,strTitle)                     JItem<JMenu> objName (strTitle)
#define ITEM_SET_MENU(itemName,menuName)            itemName.Set_Next_Menu(&menuName)
#define ITEM_SET_APP(itemName,appName)              itemName.Set_Next_App(&appName)
#define ITEM_SET_EVENT(itemName,eventName,strErr)   itemName.Set_Event(eventName,strErr)

#define JMENU(objName,strTitle)                     JMenu objName (30,2,20,40,strTitle)
#define JMENU_BASE(objName,strTitle)                JBaseMenu objName (30,2,20,40,strTitle)
#define MENU_SET_ITEM(menuName,itemPtrs...)\
        JItem<JMenu>* menuName##list[] = {itemPtrs};\
        menuName.set_jitems(menuName##list,ARRAY_SIZE(menuName##list))

#endif

