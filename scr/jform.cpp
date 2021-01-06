#include "jform.hpp"
#include <cstring>


void JForm::Create_Form(void)
{
    int32_t length = 10;
    int16_t lengthMax = length;

    mFields = new FIELD* [mFieldNum+1];

    keypad(Get_Base_Window(),TRUE);

    for (int i = 0; i < mFieldNum; ++i)
    {
        mFields[i] = new_field(1,10,1+i,1,0,0);
        
        field_opts_off(mFields[i], O_AUTOSKIP);

        set_field_just(mFields[i],JUSTIFY_CENTER);

        set_field_back(mFields[i],A_UNDERLINE);

        set_field_type(mFields[i],TYPE_NUMERIC);                /*numerical inputs only*/

        length = strlen(mFieldList[i]->Get_Title());

        if (length > lengthMax) lengthMax = length;   
    }

    mFormWindow = derwin(Get_Base_Window(),Get_H()-4,Get_W()/2-4,2,(Get_W()/2));

    mLabelWindow = derwin(Get_Base_Window(),Get_H()-4,lengthMax,2,Get_W()/2-lengthMax-1);

    std::string q;

    for (int i = 0; i < mFieldNum; ++i)
    {
        mvwprintw(mLabelWindow,1+i,1,mFieldList[i]->Get_Title());
        mFieldList[i]->Pull(q);

        set_field_buffer(mFields[i],0,q.c_str());
        JPrint(q.c_str());
        refresh();
    }

    mFields[mFieldNum] = NULL;

    mForm = new_form(mFields);

    set_form_win(mForm, Get_Base_Window());

    set_form_sub(mForm, mFormWindow);

    post_form(mForm);

}

void JForm::Close_Form(void)
{
    selected = 0;

    keypad(Get_Base_Window(),FALSE);
    unpost_form(mForm);

    for (int32_t i = 0; i < mFieldNum; i++)
    free_field(mFields[i]);

    delete[] mFields;
    mFields = NULL;

    free_form(mForm);
    delwin(mFormWindow);
    delwin(Get_Base_Window());
}

void JForm::Display(void)
{
    Show();
    Create_Form();
    wrefresh(Get_Base_Window());
    form_driver(mForm,REQ_END_FIELD);
    int c = 0;
    while (c != KEY_LEFT)
    {
        c = wgetch(Get_Base_Window());

        switch (c)
        {
            case KEY_DOWN:
                form_driver(mForm,REQ_NEXT_FIELD);
                form_driver(mForm,REQ_END_FIELD);
                Base_Clear();
                break;
            case KEY_UP:
                form_driver(mForm,REQ_PREV_FIELD);
                form_driver(mForm,REQ_END_FIELD);
                Base_Clear();
                break;
            case KEY_NPAGE:
                form_driver(mForm, REQ_SCR_FPAGE);
                break;
            case KEY_PPAGE:
                form_driver(mForm, REQ_SCR_BPAGE);
                break;
            case KEY_BACKSPACE:
                form_driver(mForm,REQ_DEL_PREV);
                break;
            case 10://key_enter
                form_driver(mForm,REQ_END_FIELD);
                Update();
                Base_Print("Saved");
                refresh();
                break;
            default:
                form_driver(mForm,c);
                break;
        }
        wrefresh(mFormWindow);
    }
    Close_Form();
}

void JForm::Update(void)
{
    std::string data;

    for (int i = 0; i < mFieldNum; i++)
    {
        data.assign(field_buffer(mFields[i],0));
        mFieldList[i]->Push(data);
        mFieldList[i]->Pull(data);
        set_field_buffer(mFields[i],0,data.c_str());
    }
}



