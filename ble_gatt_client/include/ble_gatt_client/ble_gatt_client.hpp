#include "rclcpp/rclcpp.hpp"
#include <unistd.h> //c POSIX 함수 read와 write 사용
#include <stdlib.h> //C 표준 유틸리티 함수들을 모아놓은 헤더파일이다.
#include <stdio.h> //(표준입출력 라이브러리)의 약어로써, C 언어의 표준 라이브러리 함수의 매크로 정의, 상수, 여러 형의 입출력 함수
#include <string.h>
#include <regex>

using namespace std;

class BleGattClient
{
    private:
        shared_ptr<rclcpp::Node> nh_;

        int* fd1;
        int *fd2;

        /*측정된 회전수의 전체 회전수를 계산하기 위한 변수*/
        int pre_count;
        int cur_count;
        int hex_cycle_count;

        /*ble 통신을 통해 보낼 cmd*/
        string arduino_uuid;
        string gatt_attribute_uuid;
        string connect_cmd;
        string menu_cmd;
        string select_attribute_cmd;
        string notify_cmd;

        smatch match;
        char buf[1024];
        char* ptr; 
        char hex_num[2];

        

    public:
        BleGattClient(shared_ptr<rclcpp::Node> nh, int* fd1, int* fd2);
        ~BleGattClient();
        void sendBleCommand(string ble_cmd, int* fd1);
        void getGattValue();

};