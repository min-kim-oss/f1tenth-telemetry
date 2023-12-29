#include "ble_gatt_client/ble_gatt_client.hpp"

BleGattClient::BleGattClient(std::shared_ptr<rclcpp::Node> nh, int* fd1, int* fd2) : nh_(nh)
{
    //초가화 함수
    this->fd1 = fd1;
    this->fd2 = fd2;

    this->pre_count = 0;
    this->cur_count = 0;
    this->hex_cycle_count = 0;

    // ble 통신을 통해 bluetootch ctl 에게 보낼 cmd 초기화
    arduino_uuid = "9C:D3:FB:4F:2A:CE";
    gatt_attribute_uuid = "00002101-0000-1000-8000-00805f9b34fb\n";
    connection_cmd = "connect " + arduino_uuid + "\n";
    menu_cmd = "menu gatt\n";
    select_attribute_cmd = "select-attribute " + gatt_attribute_uuid + "\n";
    notify_cmd = "notify on\n";

    //specify key words for checking child process's progress
    child_ready_check_cmd = "Agent registered";
    connection_check_cmd = "Connection successful"; 
    menu_check_cmd = "export";
    select_attribute_check_cmd ="[Arduino:/service";
    notify_check_cmd="Notify started";


    //return값 체크 추가
    this-> checkChildProgress(child_ready_check_cmd);
    
    this->sendBleCommand(connection_cmd, connection_check_cmd, fd1);
    cout << "parent : check connect cmd" << endl;
        
    this->sendBleCommand(menu_cmd, menu_check_cmd, fd1);
    cout << "parent : check menu cmd" << endl;
    
    this->sendBleCommand(select_attribute_cmd, select_attribute_check_cmd,fd1);
    cout << "parent : check selsect attribute cmd" << endl;
    
    this->sendBleCommand(notify_cmd, notify_check_cmd,fd1);
    cout << "parent : check notify cmd" << endl;
    
    
}

BleGattClient::~BleGattClient()
{

}

bool BleGattClient::sendBleCommand(string ble_cmd, string check_cmd, int* fd1)
{
    //write return 값 -> system 함수 호출시 에러는 필수로 확인할것
    write(fd1[1], ble_cmd.c_str(), ble_cmd.length());
    //코딩할시에는 함수의 원형을 정하고 return 값을 미리 정하여 규율화 할것
    checkChildProgress(check_cmd);
}

bool BleGattClient::checkChildProgress(string check_cmd){
    memset(buf,0,sizeof(char)*1024);    
    while (read(fd2[0], buf, sizeof(buf)) != 0) {  
        
        if (strlen(buf) != 0 /*true*/){
            std::cout << "income size: " << strlen(buf)<< std::endl; // bluetoothctl 출력문 저장 
            std::cout << buf << std::endl; 
        }             
        string stringed_buf(buf);
        if (stringed_buf.find(check_cmd) == string::npos) {
            memset(buf,0,sizeof(char)*1024);
            continue;
        }
        else{
            cout<< "cmd progress checked" << endl;
            memset(buf,0,sizeof(char)*1024);
            return true;
        }
    }    
}

void BleGattClient::getGattValue()
{
    cout<<"start getting value" << endl;
    while (1) {
        char check_word[] = "Value:";
        regex re("\\s(\\w{2})\\s\\w{2}\\s\\w{2}\\s\\w{2}\\s");
        memset(buf,0,sizeof(char)*1024);
        read(fd2[0], buf, sizeof(buf));
        //cout<< buf << endl;
        ptr = strstr(buf, check_word);
        if (ptr != NULL) {
            string stringfy_buf(buf);
            if (regex_search(stringfy_buf, match, re)){
                cout<< buf << endl;
                strcpy(hex_num,string(match[1]).c_str());
                pre_count = cur_count;
                cur_count = strtol(hex_num, NULL,16);
                if(cur_count < pre_count) {
                    hex_cycle_count += 1;
                }
                cout<<"total count :"<< hex_cycle_count*255 + cur_count << endl;
            }
        }
    }
}