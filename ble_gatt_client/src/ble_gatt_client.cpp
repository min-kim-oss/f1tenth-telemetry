#include "ble_gatt_client/ble_gatt_client.hpp"

BleGattClient::BleGattClient(std::shared_ptr<rclcpp::Node> nh, int* fd1, int* fd2) : nh_(nh)
{

    this->fd1 = fd1;
    this->fd2 = fd2;

    this->pre_count = 0;
    this->cur_count = 0;
    this->hex_cycle_count = 0;

    // ble 통신을 통해 bluetootch ctl 에게 보낼 cmd 초기화
    arduino_uuid = "9C:D3:FB:4F:2A:CE";
    gatt_attribute_uuid = "00002101-0000-1000-8000-00805f9b34fb\n";
    connect_cmd = "connect " + arduino_uuid + "\n";
    menu_cmd = "menu gatt\n";
    select_attribute_cmd = "select-attribute " + gatt_attribute_uuid + "\n";
    notify_cmd = "notify on\n";

    sleep(1);
    this->sendBleCommand(connect_cmd, fd1);
    cout << "check connect cmd" << endl;
    sleep(1);
    this->sendBleCommand(menu_cmd, fd1);
    cout << "check menu cmd" << endl;
    sleep(1);
    this->sendBleCommand(select_attribute_cmd, fd1);
    cout << "check selsect attribute cmd" << endl;
    sleep(1);
    this->sendBleCommand(notify_cmd, fd1);
    cout << "check notify cmd" << endl;
}

BleGattClient::~BleGattClient()
{

}

void BleGattClient::sendBleCommand(string ble_cmd, int* fd1)
{
    write(fd1[1], ble_cmd.c_str(), ble_cmd.length());
}

void BleGattClient::getGattValue()
{
    cout<<"start getting value" << endl;
    while (1) {
        //char check_word[] = "Value:";
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