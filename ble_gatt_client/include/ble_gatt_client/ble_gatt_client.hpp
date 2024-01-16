#include "rclcpp/rclcpp.hpp"
#include <unistd.h> //c POSIX 함수 read와 write 사용
#include <stdlib.h> //C 표준 유틸리티 함수들을 모아놓은 헤더파일이다.
#include <stdio.h> //(표준입출력 라이브러리)의 약어로써, C 언어의 표준 라이브러리 함수의 매크로 정의, 상수, 여러 형의 입출력 함수
#include <string.h>
#include <regex>
#include <fcntl.h>
#include <sys/types.h>
#include <std_msgs/msg/float64.hpp>
#include "telemetry_interfaces/msg/telemetry_count.hpp"


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
        string connection_cmd;
        string menu_cmd;
        string select_attribute_cmd;
        string notify_cmd;

        //key words for checking child process's progress
        string child_ready_check_cmd;
        string connection_check_cmd;
        string menu_check_cmd;
        string select_attribute_check_cmd;
        string notify_check_cmd;

        smatch match;
        char buf[1024];
        char* ptr; 
        char hex_num[2];

        //rclcpp::Publisher<telemetry_interfaces::msg::TelemetryCount>::SharedPtr count_pub_;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr count_test_pub_;

    public:
        BleGattClient(shared_ptr<rclcpp::Node> nh, int* fd1, int* fd2);
        ~BleGattClient();
        bool sendBleCommand(string ble_cmd, string check_cmd, int* fd1);
        bool checkChildProgress(string check_cmd);
        void getGattValue();
        void count_publisher(int count);

};