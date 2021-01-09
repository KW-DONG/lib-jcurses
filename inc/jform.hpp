#ifndef __JFORM_HPP
#define __JFORM_HPP
#include "jmenu.hpp"
#include "jwindow.hpp"
#include <form.h>
#include <string>

typedef int32_t (*Field_Pull_Callback)(std::string&);

typedef int32_t (*Field_Push_Callback)(std::string&);

class JField : public JWidget
{
public:
    JField(const char* title):
    JWidget(title),mPull(NULL),mPush(NULL),pushMessageList(NULL),pullMessageList(NULL){}

    ~JField(){}

    void Set_Push(Field_Push_Callback pushCallback, event_feedback_t* messageList)
    {
        mPush = pushCallback;
    }

    void Set_Pull(Field_Pull_Callback pullCallback, event_feedback_t* messageList)
    {
        mPull = pullCallback;
    }

    const char* Pull(std::string& p)
    {
        if (pullMessageList!=NULL)
        {
            return get_feedback(mPull(p),pullMessageList);
        }
        else
        {
            mPull(p);
            return "Pull";
        }
    }

    const char* Push(std::string& p)
    {
        if (pushMessageList!=NULL)
        {
            return get_feedback(mPush(p),pushMessageList);
        }
        else
        {
            mPush(p);
            return "Push";
        }
    }

    Field_Pull_Callback Get_Pull(void)
    {
        return mPull;
    }

    Field_Push_Callback Get_Push(void)
    {
        return mPush;
    }

private:

    int32_t (*mPush)(std::string&);

    int32_t (*mPull)(std::string&);

    event_feedback_t* pushMessageList;

    event_feedback_t* pullMessageList;

};

/**
 * +--------[Title]--------+
 * |+----------++---------+|
 * ||          ||         ||
 * ||  Labels  ||   Form  ||
 * ||          ||  Fields ||
 * ||          ||         ||
 * |+----------++---------+|
 * +-----------------------+
 */
class JForm : public JApp
{
public:

    JForm(int32_t startX, int32_t startY, uint32_t height, uint32_t width, const char* title):
    JApp(startX,startY,height,width,title),mFieldList(NULL),jmenu_last(NULL),mFieldNum(0){}

    ~JForm(){}

    virtual void display(void);

    void Close_Form(void);

    void Set_Fields(JField** fieldList, int32_t num)
    {
        mFieldList = fieldList;
        mFieldNum = num;
    }

    void Update(void);          /*Save and update parameters*/

protected:

    void Create_Form(void);

private:

    WINDOW* mFormWindow;        /*the window that associate the form*/

    WINDOW* mLabelWindow;

    FORM* mForm;                /*the form list*/

    FIELD** mFields;

    JField** mFieldList;
    
    JMenu* jmenu_last;

    int32_t mFieldNum;

    int32_t selected;
};

#define JFORM(objName,strTitle)                 Jform objName(30,2,20,40,strTitle)
#define FORM_SET_FIELD(formName,fieldPtrs...)\
        JField* formName##list[] = {fieldPtrs};\
        formName.Set_Fields(formName##list[],ARRAY_SIZE(formName##list[]))



#endif