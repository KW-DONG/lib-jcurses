#include "jform.hpp"
#include "snake.hpp"
#include <string>
#include <ipc_cfg.h>
#include <iostream>
#include <pthread.h>
#include <limits.h>

int32_t Item12_Event(JMenu* ptr);
int32_t Item13_Event(JMenu* ptr);
int32_t Item14_Event(JMenu* ptr);
ipc_data_t ipc_data;
pthread_mutex_t ipc_mtx;

#define DATA_NUM (PIPE_BUF/sizeof(ipc_data))

P_JFIELD_TRANS_I(int_field,ipc_data.intNum,&ipc_mtx);
P_JFIELD_TRANS_F(float_field,ipc_data.floatNum,&ipc_mtx);
P_JFIELD_TRANS_I(bool_field,ipc_data.boolNum,&ipc_mtx);

int pipe_w_fd,pipe_r_fd,ipc_flag;

void* terminal_display(void* args);
void* data_transfer(void* args);

int main()
{
    pid_t pid;

    pthread_t display_thread, transfer_thread;

    if (access (FIFO_F2B_NAME,F_OK)==-1){  if(mkfifo(FIFO_F2B_NAME,S_IRUSR|S_IWUSR|S_IWGRP)!=0) U_PRINT("Could not create fifo f2b"); }
    if (access (FIFO_B2F_NAME,F_OK)==-1){  if(mkfifo(FIFO_B2F_NAME,S_IRUSR|S_IWUSR|S_IWGRP)!=0) U_PRINT("Could not create fifo b2f");}
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

    ipc_flag = 1;

    //pthread_create(&display_thread,NULL,terminal_display,NULL);
    pthread_create(&transfer_thread,NULL,data_transfer,NULL);

    std::cout << "thread created" << std::endl;

    while(1)
    {
        sleep(1);
        std::cout << "front:" << ipc_data.floatNum << std::endl;
    }

    //pthread_join(display_thread,NULL);
    pthread_join(transfer_thread,NULL);

    

    close(pipe_r_fd);
    close(pipe_w_fd);

    return 0;
}

void* terminal_display(void* args)
{
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
    MENU_SET_ITEM(baseMenu,&item1,&item2,&item3,&item4,&item5);
    MENU_SET_ITEM(menu1,&item11,&item12,&item13,&item14,&item15);
    MENU_SET_ITEM(menu13,&item131,&item132,&item133,&item134);
    
    //j_init();
    //baseMenu.display();
    refresh();
    endwin();
}

int32_t Item12_Event(JMenu* ptr)    {   B_PRINT(INT2STR(ipc_data.intNum),2);}

int32_t Item13_Event(JMenu* ptr)    {   B_PRINT(FLT2STR(ipc_data.floatNum),2);}

int32_t Item14_Event(JMenu* ptr)    {   B_PRINT(INT2STR(ipc_data.boolNum),2);}

void* data_transfer(void* args)
{
    int n;
    while(ipc_flag==1)
    {
        pthread_mutex_lock(&ipc_mtx);
        write (pipe_w_fd,&ipc_data,IPC_BUFFER_SIZE);
        for (int i;i<DATA_NUM;i++)read(pipe_r_fd,&ipc_data,IPC_BUFFER_SIZE);
        //n = read(pipe_r_fd,&ipc_data,IPC_BUFFER_SIZE);
        //std::cout << "front read:" << n << std::endl;
        //std::cout << "front:" << ipc_data.intNum << std::endl;
        pthread_mutex_unlock(&ipc_mtx);
        //std::cout << ipc_data.intNum << std::endl;
        
        //std::cout << ipc_data.boolNum << std::endl;
        //U_PRINT(FLT2STR(ipc_data.floatNum));
        //sleep(1);
    }
}
