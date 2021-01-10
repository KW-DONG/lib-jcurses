#include "jform.hpp"
#include <cstring>


void JForm::create(void)
{
    int32_t length = 10;
    int16_t lengthMax = length;

    mFields = new FIELD* [mFieldNum+1];

    keypad(get_base_window(),TRUE);

    for (int i = 0; i < mFieldNum; ++i)
    {
        mFields[i] = new_field(1,10,1+i,1,0,0);
        
        field_opts_off(mFields[i], O_AUTOSKIP);

        set_field_just(mFields[i],JUSTIFY_CENTER);

        set_field_back(mFields[i],A_UNDERLINE);

        set_field_type(mFields[i],TYPE_NUMERIC);                /*numerical inputs only*/

        length = strlen(mFieldList[i]->get_title());

        if (length > lengthMax) lengthMax = length;   
    }

    mFormWindow = derwin(get_base_window(),get_h()-4,get_w()/2-4,2,(get_w()/2));

    mLabelWindow = derwin(get_base_window(),get_h()-4,lengthMax,2,get_w()/2-lengthMax-1);

    std::string q;

    for (int i = 0; i < mFieldNum; ++i)
    {
        mvwprintw(mLabelWindow,1+i,1,mFieldList[i]->get_title());
        mFieldList[i]->pull(q);

        set_field_buffer(mFields[i],0,q.c_str());
        j_print(q.c_str());
        refresh();
    }

    mFields[mFieldNum] = NULL;

    mForm = new_form(mFields);

    set_form_win(mForm, get_base_window());

    set_form_sub(mForm, mFormWindow);

    post_form(mForm);

}

void JForm::close(void)
{
    selected = 0;

    keypad(get_base_window(),FALSE);
    unpost_form(mForm);

    for (int32_t i = 0; i < mFieldNum; i++)
    free_field(mFields[i]);

    delete[] mFields;
    mFields = NULL;

    free_form(mForm);
    delwin(mFormWindow);
    delwin(get_base_window());
}

void JForm::display(void)
{
    post_frame();
    create();
    wrefresh(get_base_window());
    form_driver(mForm,REQ_END_FIELD);
    int c = 0;
    while (c != KEY_LEFT)
    {
        c = wgetch(get_base_window());

        switch (c)
        {
            case KEY_DOWN:
                form_driver(mForm,REQ_NEXT_FIELD);
                form_driver(mForm,REQ_END_FIELD);
                base_clear();
                break;
            case KEY_UP:
                form_driver(mForm,REQ_PREV_FIELD);
                form_driver(mForm,REQ_END_FIELD);
                base_clear();
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
                update();
                base_print("Saved");
                refresh();
                break;
            default:
                form_driver(mForm,c);
                break;
        }
        wrefresh(mFormWindow);
    }
    close();
}

void JForm::update(void)
{
    std::string data;

    for (int i = 0; i < mFieldNum; i++)
    {
        data.assign(field_buffer(mFields[i],0));
        mFieldList[i]->push(data);
        mFieldList[i]->pull(data);
        set_field_buffer(mFields[i],0,data.c_str());
    }
}



