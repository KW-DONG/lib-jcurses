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

    JItem(const char*title):JWidget(title),event(NULL),mMessageList(NULL),jmenu(NULL),japp(NULL){}

    ~JItem(){}

    void set_event(Item_Sel_Callback func, event_feedback_t* messageList)
    {
        event = func;

        mMessageList = messageList;
    }

    const char* selected(T* menuPtr)
    {
        if (mMessageList!=NULL)
        {
           return get_feedback(event(menuPtr),mMessageList); 
        }
        else
        {
            event(menuPtr);
            return "Selected";
        }
    }

    PROPERTY_RW(japp,JApp*);
    PROPERTY_RW(jmenu,T*);
    PROPERTY_R(event,Item_Sel_Callback);

private:

    T* jmenu;
    JApp* japp;
    int32_t (*event)(T* menuPtr);      /*write an event here*/
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

    PROPERTY_RW(jmenu_last,JMenu*);
    PROPERTY_R(jitems,JItem<JMenu>**);
    PROPERTY_R(window_menu,WINDOW*);
    PROPERTY_R(menu,MENU*);
    PROPERTY_R(items,ITEM**);
    PROPERTY_R(item_num,int32_t);

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
    void switch_f(JMenu* newMenu)
    {
        clear();
        unpost_menu(mCurrentMenu->get_menu());
        newMenu->set_jmenu_last(mCurrentMenu);
        mCurrentMenu = newMenu;
        set_jrefresh_bit();
    }

    void switch_b(void)
    {
        clear();
        unpost_menu(mCurrentMenu->get_menu());
        mCurrentMenu = mCurrentMenu->get_jmenu_last();
        set_jrefresh_bit();
    }

    void refresh_menu(void)
    {
        mCurrentMenu->post_frame();
        mCurrentMenu->post();
    }

    void run_japp(JApp* app);

    void menu_recursion(void);

    void menu_sort(void);
    
    void create(void);

    void menu_delete(void);

private:
    JMenu* mCurrentMenu;    //current menu displayed
    JMenu* mThisMenu;       //base menu
    std::vector<JMenu*> mTree;
};

#define JITEM(objName,strTitle)                     JItem<JMenu> objName (strTitle)
#define ITEM_SET_MENU(itemName,menuName)            itemName.set_jmenu(&menuName)
#define ITEM_SET_APP(itemName,appName)              itemName.set_japp(&appName)
#define ITEM_SET_EVENT(itemName,eventName,strErr)   itemName.set_event(eventName,strErr)

#define JMENU(objName,strTitle)                     JMenu objName (30,2,20,40,strTitle)
#define JMENU_BASE(objName,strTitle)                JBaseMenu objName (30,2,20,40,strTitle)
#define MENU_SET_ITEM(menuName,itemPtrs...)\
        JItem<JMenu>* menuName##_list[] = {itemPtrs};\
        menuName.set_jitems(menuName##_list,ARRAY_SIZE(menuName##_list))

#endif

