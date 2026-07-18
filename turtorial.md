#Setup Raspi
Ket noi Pi voi man hinh, nhap mat khau:
    yugihata
Ket noi Wifi
Bat terminal:
    hostname -I
Lay IP cua Pi
Ket noi USB-C cua Pixhawk voi Pi
Ket noi TELEM2 cua Pixhawk voi Pi qua USB-TTL Adapter nhu hinh anh (TX-TX, RX-RX, GND-GND)


#Laptop
Bat cong cu quan ly Terminal len
    terminator
Chia terminal ra cac khung: Ctrl + Shift + E, Ctrl + Shift + O
Truy cap vao Pi qua ssh
    ssh thanh@10.136.62.91
    yugihata
Terminal1: Pi
    cd Micro-XRCE-DDS-Agent/build
    ./MicroXRCEAgent serial --dev /dev/ttyUSB0 -b 921600 -v 4

Terminal2: Pi
    mavlink-routerd -e 10.136.152.89:14550 /dev/ttyACM0:921600
    mavlink-routerd -e 192.168.0.101:14550 /dev/ttyACM0:921600

#QGC:
Kiem tra pin, radio config, actuactor config
Kiem tra param:
    UXRCE_DDS_CONFIG = TELEM2
    SER_TEL2_BAUD = 912600
    MAV_1_CONFIG, MAV_0_CONFIG, MAV_2_CONFIG khong duoc la TELEM2 (vi UXRCE_DDS dung TELEM2 roi)
reboot neu thay doi param hoac config
vao console, kiem tra ket noi:
    uxrce_dds_client status
neu running, connect thi la OK
neu ko, kiem tra lai config, param va reboot

Vay la ta da thiet lap xong cau noi PX4-ROS2: Micro-XRCE-DDS-Agent tren Pi va UXRCE_DDS_Client tren PX4

#Sample project: Arm and disarm
Terminal3: Pi
    source /opt/ros/kilted/setup.bash
    cd uav_simulation
    colcon build
    source install/setup.bash
    ros2 run drone_control my_node
    

