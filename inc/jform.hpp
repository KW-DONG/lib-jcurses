#ifndef __JFORM_HPP
#define __JFORM_HPP
#include "jmenu.hpp"
#include "jwindow.hpp"
#include <form.h>
#include <string>
#include <pthread.h>

typedef int32_t (*Field_Pull_Callback)(std::string&);

typedef int32_t (*Field_Push_Callback)(std::string&);

typedef int32_t (*Form_IPC)(void);

class JField : public JWidget
{
public:
    JField(const char* title):
    JWidget(title),mPull(NULL),mPush(NULL),pushMessageList(NULL),pullMessageList(NULL){}

    ~JField(){}

    void set_push(Field_Push_Callback pushCallback, event_feedback_t* messageList)  {   mPush = pushCallback;}

    void set_pull(Field_Pull_Callback pullCallback, event_feedback_t* messageList)  {   mPull = pullCallback;}

    const char* pull(std::string& p)
    {
        if (pullMessageList!=NULL)  return get_feedback(mPull(p),pullMessageList);
        else
        {
            mPull(p);
            return "Pull";
        }
    }

    const char* push(std::string& p)
    {
        if (pushMessageList!=NULL)  return get_feedback(mPush(p),pushMessageList);
        else
        {
            mPush(p);
            return "Push";
        }
    }

    Field_Pull_Callback get_pull(void)  {   return mPull;   }

    Field_Push_Callback get_push(void)  {   return mPush;   }

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

    void close(void);

    void set_fields(JField** fieldList, int32_t num)
    {
        mFieldList = fieldList;
        mFieldNum = num;
    }

    void update(void);          /*Save and update parameters*/

protected:
    void create(void);

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

#define JFORM(objName,strTitle)                 JForm objName(30,2,20,40,strTitle)

#define JFIELD_TRANS_F(objName,data)\
        int32_t objName##_push(std::string& text)   {data = STR2FLT(text.c_str());}\
        int32_t objName##_pull(std::string& text)   {text.assign(FLT2STR(data));}

#define JFIELD_TRANS_I(objName,data)\
        int32_t objName##_push(std::string& text)   {data = STR2INT(text.c_str());}\
        int32_t objName##_pull(std::string& text)   {text.assign(INT2STR(data));}

#define P_JFIELD_TRANS_F(objName,data,mtxPtr)\
        int32_t objName##_push(std::string& text)\
            {\
                pthread_mutex_lock(mtxPtr);\
                data = STR2FLT(text.c_str());\
                pthread_mutex_unlock(mtxPtr);\
            }\
        int32_t objName##_pull(std::string& text)\
            {\
                pthread_mutex_lock(mtxPtr);\
                text.assign(FLT2STR(data));\
                pthread_mutex_unlock(mtxPtr);\
            }

#define P_JFIELD_TRANS_I(objName,data,mtxPtr)\
        int32_t objName##_push(std::string& text)\
            {\
                pthread_mutex_lock(mtxPtr);\
                data = STR2INT(text.c_str());\
                pthread_mutex_unlock(mtxPtr);\
            }\
        int32_t objName##_pull(std::string& text)\
            {\
                pthread_mutex_lock(mtxPtr);\
                text.assign(INT2STR(data));\
                pthread_mutex_unlock(mtxPtr);\
            }

#define JFIELD(objName, strTitle)\
        JField objName(strTitle);\
        objName.set_pull(objName##_pull,NULL);\
        objName.set_push(objName##_push,NULL)

#define FORM_SET_FIELD(formName,fieldPtrs...)\
        JField* formName##_list[] = {fieldPtrs};\
        formName.set_fields(formName##_list,ARRAY_SIZE(formName##_list))



#endif