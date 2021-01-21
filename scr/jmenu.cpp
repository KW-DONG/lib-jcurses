#include "jmenu.hpp"
#include <stdlib.h>
#include <algorithm>

void JMenu::create(void)
{    
    items = (ITEM**)new ITEM* [item_num+1];
    
    int32_t length;

    for(int i = 0; i < item_num; ++i)
    {
        length = jitems[i]->get_width();
        items[i] = new_item(jitems[i]->get_title()," ");
        if (length>mLengthMax)   mLengthMax = length+2;
    }
    items[item_num] = new_item((char*)NULL,(char*)NULL);
    menu = new_menu((ITEM**)items);
}

void JMenu::post(void)
{
    window_menu = derwin(get_base_window(),get_h()-3,mLengthMax,2,(get_w()-mLengthMax)/2);

    set_menu_win(menu, get_base_window());

    set_menu_sub(menu, window_menu);

    set_menu_format(menu,get_h()-4,1);

    set_menu_mark(menu,"->");

    post_menu(menu);
}


void JMenu::close(void)
{
    unpost_menu(menu);
    free_menu(menu);
    menu = NULL;
    for (int32_t i = 0; i < item_num; ++i)
    {
        free_item(items[i]);
    }
    delete[] items;
    free_menu(menu);
    //free(items);
    items = NULL;

    delwin(window_menu);
    delwin(get_base_window());
}

void JBaseMenu::display(void)
{
    int c = 0;
    JMenu* first = mCurrentMenu;
    menu_recursion();
    //Sort_Menu();
    create();
    mCurrentMenu = first;
    while(c != KEY_F(2))
    {
        get_refresh_update();
        
        if (get_jrefresh_bit())
        {
            refresh_menu();
            reset_jrefresh_bit();
        }

        wrefresh(mCurrentMenu->get_base_window());
        c = wgetch(mCurrentMenu->get_base_window());
        
        switch (c)
        {
        case KEY_DOWN:
            menu_driver(mCurrentMenu->get_menu(), REQ_DOWN_ITEM);
            break;
        
        case KEY_UP:
            menu_driver(mCurrentMenu->get_menu(), REQ_UP_ITEM);
            break;

        case KEY_NPAGE:
            menu_driver(mCurrentMenu->get_menu(), REQ_SCR_DPAGE);
            break;

        case KEY_PPAGE:
            menu_driver(mCurrentMenu->get_menu(), REQ_SCR_UPAGE);
            break;
        
        case KEY_LEFT:
            if (mCurrentMenu->get_jmenu_last()!=NULL)
            switch_b();
            break;
        
        case KEY_RIGHT:
            {
                ITEM *cur;
                cur = current_item(mCurrentMenu->get_menu());

                if (mCurrentMenu->get_jitems()[cur->index]->get_event()!=NULL)
                U_PRINT(mCurrentMenu->get_jitems()[cur->index]->selected(mCurrentMenu));

                JMenu* nextMenu;
                nextMenu = mCurrentMenu->get_jitems()[cur->index]->get_jmenu();

                if (nextMenu!=NULL) switch_f(nextMenu);
                else
                {
                    JApp* nextApp;
                    nextApp = mCurrentMenu->get_jitems()[cur->index]->get_japp();
                    if (nextApp!=NULL)      run_japp(nextApp);
                }
                refresh(); 
            }
            break;
        }
    }

    while(mCurrentMenu->get_jmenu_last()!=NULL)
    {
        switch_b();
        refresh_menu();
    }
    mCurrentMenu->close();
}

void JBaseMenu::run_japp(JApp* app)
{
    keypad(mCurrentMenu->get_base_window(),FALSE);
    unpost_menu(mCurrentMenu->get_menu());
    app->display();
    clear();
    set_jrefresh_bit();
}

void JBaseMenu::menu_recursion(void)
{
    int i = 0;
    int num = mCurrentMenu->get_item_num();
    JMenu* next = NULL;
    JMenu* last = NULL;
    mTree.push_back(mCurrentMenu);
    for (i=0;i<num;i++)
    {
        next = mCurrentMenu->get_jitems()[i]->get_jmenu();
        if (next != NULL)
        {
            last = mCurrentMenu;
            mCurrentMenu = next;
            mCurrentMenu->set_jmenu_last(last);
            menu_recursion();
            mCurrentMenu = last;
        }
    }  
}

void JBaseMenu::menu_sort(void)
{
    JMenu* temp;

    for (int i = 0; i<mTree.size();i++)
    {
        for (int n = 0; n<mTree.size()-1;n++)
        {
            if (mTree[n]->get_item_num()>mTree[n+1]->get_item_num())
            {
                temp = mTree[n];
                mTree[n] = mTree[n+1];
                mTree[n+1] = temp;
            }
        }
    }
}


void JBaseMenu::create(void)
{
    for (int i = 0; i<mTree.size();i++)
    {
        mTree[i]->create();  
    }
}



