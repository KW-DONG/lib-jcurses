#include "ipc_cfg.h"
#include <cstdlib>
#include <iostream>

ipc_data_t ipc_data;

int main(int argc, char* argv[])
{
    int pipe_r_fd, pipe_w_fd;
    ipc_data.run = true;
    ipc_data.func = true;

    std::cout << "backstage start" << std::endl;

    pipe_r_fd = open(FIFO_F2B_NAME,O_RDONLY|O_NONBLOCK);

    std::cout << "back f2b opened" << std::endl;

    pipe_w_fd = open(FIFO_B2F_NAME,O_WRONLY|O_NONBLOCK);

    std::cout << "back b2f opened" << std::endl;

    while (1)
    {
        std::cout << "write:" << write(pipe_w_fd,&ipc_data,IPC_BUFFER_SIZE) << std::endl;
       
        std::cout << "running" << std::endl;
        if (ipc_data.func)
        {
            if (ipc_data.boolNum == true)   ipc_data.boolNum = false;
            else                            ipc_data.boolNum = true;
            if (ipc_data.floatNum > 500.0)  ipc_data.floatNum = 10.0;
            else                            ipc_data.floatNum = ipc_data.floatNum + 0.3;
            if (ipc_data.intNum > 200)      ipc_data.intNum = 0;
            else                            ipc_data.intNum ++;

        }

        if ((read (pipe_r_fd,&ipc_data,IPC_BUFFER_SIZE))!=IPC_BUFFER_SIZE)
        {
            std::cout << "read fail" << std::endl;
        }
 
        sleep(1);
    }
    std::cout << "backstage finished" << std::endl;

    close(pipe_r_fd);
    close(pipe_w_fd);
    exit(0);
}
