#ifndef __IPC_CFG_H
#define __IPC_CFG_H
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

//data type
typedef struct _ipc_data_t
{
    float floatNum;
    int intNum;
    bool boolNum;
    bool run;
    bool func;
}ipc_data_t;

#define IPC_BUFFER_SIZE sizeof(ipc_data_t)

//fifo details
#define FIFO_F2B_NAME   "/tmp/f2b_fifo"
#define FIFO_B2F_NAME   "/tmp/b2f_fifo"

#define FRONT_PATH  "../build/front"
#define FRONT_ARGV  "./front"

#define BACK_PATH   "../build/back"
#define BACK_ARGV   "./back"

char *env_init[] = {"PATH=/bin", NULL};




#endif