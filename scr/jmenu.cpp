#include "jmenu.hpp"
#include <stdlib.h>

void JMenu::Create_Menu(void)
{    
    if (initFlag == true)
    {
        mItems = (ITEM**)new ITEM* [mItemNum+1];
    
        int32_t length;

        for(int i = 0; i < mItemNum; ++i)
        {
            length = mItemList[i]->Get_Width();
            mItems[i] = new_item(mItemList[i]->Get_Title()," ");
            if (length>mLengthMax)   mLengthMax = length+2;
        }

        mMenu = new_menu((ITEM**)mItems);

        initFlag = false;
    }

    mMenuWindow = derwin(Get_Base_Window(),Get_H()-3,mLengthMax,2,(Get_W()-mLengthMax)/2);

    set_menu_win(mMenu, Get_Base_Window());

    set_menu_sub(mMenu, mMenuWindow);

    set_menu_format(mMenu,Get_H()-4,1);

    set_menu_mark(mMenu,"->");

    post_menu(mMenu);

}

void JMenu::Close_Menu(void)
{
    unpost_menu(mMenu);
    free_menu(mMenu);
    mMenu = NULL;
    for (int32_t i = 0; i < mItemNum; ++i)
    {
        free_item(mItems[i]);
    }
    delete[] mItems;
    free_menu(mMenu);
    //free(mItems);
    mItems = NULL;

    delwin(mMenuWindow);
    delwin(Get_Base_Window());
}

void JBaseMenu::Display(void)
{
    int c = 0;
    while(c != KEY_F(2))
    {
        if (Get_Refresh_Bit())
        {
            Refresh_Menu();
            Reset_Refresh_Bit();
        }

        wrefresh(mCurrentMenu->Get_Base_Window());
        c = wgetch(mCurrentMenu->Get_Base_Window());
        
        switch (c)
        {
        case KEY_DOWN:
            menu_driver(mCurrentMenu->Get_Menu_List(), REQ_DOWN_ITEM);
            if (Get_Clear_Flag())    Base_Clear();
            break;
        
        case KEY_UP:
            menu_driver(mCurrentMenu->Get_Menu_List(), REQ_UP_ITEM);
            if (Get_Clear_Flag())    Base_Clear();
            break;

        case KEY_NPAGE:
            menu_driver(mCurrentMenu->Get_Menu_List(), REQ_SCR_DPAGE);
            break;

        case KEY_PPAGE:
            menu_driver(mCurrentMenu->Get_Menu_List(), REQ_SCR_UPAGE);
            break;
        
        case KEY_LEFT:
            if (mCurrentMenu->Get_Last_Menu()!=NULL)
            Switch_Backward();
            Base_Clear();
            break;
        
        case KEY_RIGHT:
            {
                ITEM *cur;
                cur = current_item(mCurrentMenu->Get_Menu_List());

                if (mCurrentMenu->Get_Item_List()[cur->index]->Get_Event()!=NULL)
                Base_Print(mCurrentMenu->Get_Item_List()[cur->index]->Selected(mCurrentMenu));

                JMenu* nextMenu;
                nextMenu = mCurrentMenu->Get_Item_List()[cur->index]->Get_Next_Menu();

                if (nextMenu!=NULL) Switch_Forward(nextMenu);
                else
                {
                    JApp* nextApp;
                    nextApp = mCurrentMenu->Get_Item_List()[cur->index]->Get_Next_App();
                    if (nextApp!=NULL)
                    {
                        Run_App(nextApp);
                    }
                    
                }
                refresh(); 
            }
            break;
        }
    }

    while(mCurrentMenu->Get_Last_Menu()!=NULL)
    {
        Switch_Backward();
        Refresh_Menu();
    }
    mCurrentMenu->Close_Menu();
}

void JBaseMenu::Run_App(JApp* app)
{
    keypad(mCurrentMenu->Get_Base_Window(),FALSE);
    unpost_menu(mCurrentMenu->Get_Menu_List());
    app->Display();
    clear();
    Set_Refresh_Bit();
}



