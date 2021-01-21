#include "jform.hpp"
#include "snake.hpp"
#include <string>
#include <ipc_cfg.h>
#include <iostream>

int32_t Item12_Event(JMenu* ptr);
int32_t Item13_Event(JMenu* ptr);
int32_t Item14_Event(JMenu* ptr);
int32_t named_pipe_write(void);
int32_t named_pipe_read(void);

int intNum = 100;
float floatNum = 100.12;
bool boolNum = true;

JFIELD_TRANS_I(int_field,intNum);
JFIELD_TRANS_F(float_field,floatNum);
JFIELD_TRANS_I(bool_field,boolNum);

ipc_data_t ipc_data;

int pipe_w_fd,pipe_r_fd;

int main()
{
    pid_t pid;

    if (access (FIFO_F2B_NAME,F_OK)==-1)  
    {  
        if(mkfifo(FIFO_F2B_NAME,S_IRUSR|S_IWUSR|S_IWGRP)!=0) U_PRINT("Could not create fifo f2b"); 
    }

    if (access (FIFO_B2F_NAME,F_OK)==-1)
    {
        if(mkfifo(FIFO_B2F_NAME,S_IRUSR|S_IWUSR|S_IWGRP)!=0) U_PRINT("Could not create fifo b2f");
    }

    std::cout << "fifo created" << std::endl;

    if ((pid = fork()) < 0) std::cout << "Fork Error" << std::endl;
    else if (pid == 0)
    {
        if (execve(BACK_PATH,NULL,env_init)<0) std::cout << "Exec Error" << std::endl;
        else std::cout << "Exec Success" << std::endl;
    }
    else std::cout << "Base process" << std::endl;

    sleep(3);

    pipe_w_fd = open(FIFO_F2B_NAME,O_WRONLY|O_NONBLOCK);

    std::cout << "front f2b opened" << std::endl;

    pipe_r_fd = open(FIFO_B2F_NAME,O_RDONLY|O_NONBLOCK);

    std::cout << "front b2f opened" << std::endl;

    JMENU_BASE(baseMenu,"main");
    JMENU(menu1,"menu1");
    JMENU(menu13,"menu13");
    JFORM(form1,"Test Form");

    JSnake snake(30,2,20,40,"snake");

    JITEM(item1,"Next Menu");
    JITEM(item2,"Snake");
    JITEM(item3,"Start Back");
    JITEM(item4,"Stop Back");
    JITEM(item5,"Item5");
    JITEM(item11,"Test Form");
    JITEM(item12,"Print Integer");
    JITEM(item13,"Print Float");
    JITEM(item14,"Print Bool");
    JITEM(item15,"Next Menu");
    JITEM(item131,"aaa");
    JITEM(item132,"bbb");
    JITEM(item133,"ccc");
    JITEM(item134,"ddd");

    JFIELD(int_field,"Integer:");
    JFIELD(float_field,"Float:");
    JFIELD(bool_field,"Bool:");

    ITEM_SET_MENU(item1,menu1);
    ITEM_SET_APP(item2,snake);
    ITEM_SET_APP(item11,form1);
    ITEM_SET_EVENT(item12,Item12_Event,NULL);
    ITEM_SET_EVENT(item13,Item13_Event,NULL);
    ITEM_SET_EVENT(item14,Item14_Event,NULL);
    ITEM_SET_MENU(item15,menu13);

    FORM_SET_FIELD(form1,&int_field,&float_field,&bool_field);
    FORM_SET_IPC(form1,named_pipe_write);
    MENU_SET_ITEM(baseMenu,&item1,&item2,&item3,&item4,&item5);
    MENU_SET_ITEM(menu1,&item11,&item12,&item13,&item14,&item15);
    MENU_SET_ITEM(menu13,&item131,&item132,&item133,&item134);
    baseMenu.set_refresh_update()
    
    //j_init();
    //baseMenu.display();

    std::cout << "into while" << std::endl;
    while(1){sleep(1);};
    std::cout << "out while" << std::endl;
    ipc_data.run = false;
    named_pipe_write();

    refresh();
    endwin();
    close(pipe_r_fd);
    close(pipe_w_fd);
    return 0;
}

int32_t Item12_Event(JMenu* ptr)    {   bprint(INT2STR(intNum));}

int32_t Item13_Event(JMenu* ptr)    {   bprint(FLT2STR(floatNum));}

int32_t Item14_Event(JMenu* ptr)    {   bprint(INT2STR(boolNum));}

int32_t named_pipe_write(void)
{
    write (pipe_w_fd,&ipc_data,IPC_BUFFER_SIZE);
    sleep(1);
}

int32_t named_pipe_read(void)
{
    read (pipe_r_fd,&ipc_data,IPC_BUFFER_SIZE);
    sleep(1);
}


