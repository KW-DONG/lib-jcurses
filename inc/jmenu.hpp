#ifndef __JMENU_HPP
#define __JMENU_HPP
#include <menu.h>
#include <array>
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
           return Get_Feedback(itemEvent(menuPtr),mMessageList); 
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
    JWindow(startX,startY,height,width,title),mItemList(NULL),mItemNum(0),initFlag(true),mLengthMax(10){}

    ~JMenu(){}

    void Create_Menu(void);

    void Close_Menu(void);

    virtual void Set_Items(JItem<JMenu>** itemList, int32_t num)
    {
        mItemList = itemList;
        mItemNum = num;
    }

    void Set_Last_Menu(JMenu* menu)
    {
        mLastMenu = menu;
    }

    JMenu* Get_Last_Menu(void)
    {
        return mLastMenu;
    }

    JItem<JMenu>** Get_Item_List(void)
    {
        return mItemList;
    }

    WINDOW* Get_Menu_Win(void)
    {
        return mMenuWindow;
    }

    MENU* Get_Menu_List(void)
    {
        return mMenu;
    }

    ITEM** Get_Items(void)
    {
        return mItems;
    }

    int32_t Get_Item_Num(void)
    {
        return mItemNum;
    }

protected:

    WINDOW*         mMenuWindow;        /*the window that associate the menu*/

    MENU*           mMenu;              /*the menu list*/

    ITEM**          mItems;             /*item list used to allocate memory*/
    
    JItem<JMenu>**  mItemList;
    
    int32_t         mItemNum;

    JMenu*          mLastMenu;

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

    void Set_Items(JItem<JMenu>** list, int32_t num)
    {
        mThisMenu->Set_Items(list,num);
    }

    virtual void Display(void);

protected:
    void Switch_Forward(JMenu* newMenu)
    {
        clear();
        unpost_menu(mCurrentMenu->Get_Menu_List());
        newMenu->Set_Last_Menu(mCurrentMenu);
        mCurrentMenu = newMenu;
        Set_Refresh_Bit();
    }

    void Switch_Backward(void)
    {
        clear();
        unpost_menu(mCurrentMenu->Get_Menu_List());
        mCurrentMenu = mCurrentMenu->Get_Last_Menu();
        Set_Refresh_Bit();
    }

    void Refresh_Menu(void)
    {
        mCurrentMenu->Show();
        mCurrentMenu->Create_Menu();
    }

    void Run_App(JApp* app);

private:
    JMenu* mCurrentMenu;

    JMenu* mThisMenu;
};

#define JITEM(objName,strTitle)                     JItem<JMenu> objName (strTitle)
#define ITEM_SET_MENU(itemName,menuName)            itemName.Set_Next_Menu(&menuName)
#define ITEM_SET_APP(itemName,appName)              itemName.Set_Next_App(&appName)
#define ITEM_SET_EVENT(itemName,eventName,strErr)   itemName.Set_Event(eventName,strErr)

#define JMENU(objName,strTitle)                     JMenu objName (30,2,20,40,strTitle)
#define JMENU_BASE(objName,strTitle)                JBaseMenu objName (30,2,20,40,strTitle)
#define MENU_SET_ITEM(menuName,itemPtrs...)\
        JItem<JMenu>* menuName##list[] = {itemPtrs};\
        menuName.Set_Items(menuName##list,ARRAY_SIZE(menuName##list))

#endif

