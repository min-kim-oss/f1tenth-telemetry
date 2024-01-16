#include "ble_gatt_client/ble_gatt_client.hpp"
#include <sys/wait.h> //  부모 프로세스가 자식프로세스 판단하는 wait 함수를 사용하기 위해서

void quit(int sig)
{
    rclcpp::shutdown();
    exit(0);
}


int main(int argc, char** argv)
{
    int fd1[2], fd2[2];
    pid_t pid;
    int num = 0;
    //int status;

    // create pipe fd1 for sending cmd to child process
    if ( pipe(fd1)  == -1){
        perror("pipe fd1 creating error");
        exit(1);
    }

    // 자식프로세스에서 부모프로세스로 cmd 의 결과로 출력된 출력문을 보내줄 파이프 fd2 를 생성
    // create pipe fd2 for sending cmd result to parent process
    if( pipe(fd2) == -1 ){
        perror("pipe fd2 creating error");
        exit(1);
    }

    signal(SIGINT,quit);

    switch(pid = fork()){
        /*case for fork error*/
        case -1: 
            perror("fork error");
            exit(1);
            break;
        /*child process*/
        case 0:  
            num++;

            //자식의 표준 입력을 pipe fd1[0] 와 연결
            if(fd1[0] != 0){
                dup2(fd1[0],0); 
                close(fd1[1]);
            }

            //자식의 표준 출력을 pipe fd2[1] 와 연결
            if(fd2[1] != 1){
                dup2(fd2[1],1); 
                close(fd2[0]);
            }
            // 자식 프로세스에서는 bluetoothctl 프로그램이 돌아가며 부모 프로세스에서 보낸 cmd 에 응답
            execlp("bluetoothctl", "bluetoothctl", NULL);
            return 0;

        /*parent process*/
        default: 
            rclcpp::init(argc, argv);
            auto node = rclcpp::Node::make_shared("ble_gatt_client");
            BleGattClient bleGattClient(node, fd1, fd2);
            signal(SIGINT,quit);
            bleGattClient.getGattValue();

            rclcpp::shutdown();
            return 0;
    }
}