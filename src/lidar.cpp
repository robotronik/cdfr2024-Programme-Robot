#include "lidar.h"

void lidarDelete(void);
static ILidarDriver* drv;
static sl_result     op_result;

bool lidarSetup(const char* serialPort ,int baudrate){

    drv = *createLidarDriver();

    if (!drv) {
        fprintf(stderr, "insufficent memory, exit\n");
        exit(-2);
    }

    sl_lidar_response_device_info_t devinfo;
    bool connectSuccess = false;
    IChannel* _channel;
    //sl_result     op_result;

    _channel = (*createSerialPortChannel(serialPort, baudrate));
    if (SL_IS_OK((drv)->connect(_channel))) {
        op_result = drv->getDeviceInfo(devinfo);

        if (SL_IS_OK(op_result)) 
        {
            connectSuccess = true;
        }
        else{
            delete drv;
            drv = NULL;
        }
    }  


    if (!connectSuccess) {
        (fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n", serialPort));
        lidarDelete();
        return false;
    }

    // print out the device serial number, firmware and hardware version number..
    printf("SLAMTEC LIDAR S/N: ");
    for (int pos = 0; pos < 16 ;++pos) {
        printf("%02X", devinfo.serialnum[pos]);
    }

    printf("\n"
            "Firmware Ver: %d.%02d\n"
            "Hardware Rev: %d\n"
            , devinfo.firmware_version>>8
            , devinfo.firmware_version & 0xFF
            , (int)devinfo.hardware_version);



    // check health...
    if (!checkSLAMTECLIDARHealth(drv)) {
        lidarDelete();
        return false;
    }
    

    drv->setMotorSpeed();
    // start scan...
    drv->startScan(0,1);


}

void lidarStop(void){
    drv->stop();
	delay(200);
    drv->setMotorSpeed(0);
    lidarDelete();
}

void lidarDelete(void){
    if(drv) {
        delete drv;
        drv = NULL;
    }
}


void lidarPrint(void){
    sl_lidar_response_measurement_node_hq_t nodes[8192];
    size_t   count = _countof(nodes);
    
    op_result = drv->grabScanDataHq(nodes, count);

    if (SL_IS_OK(op_result)) {
        drv->ascendScanData(nodes, count);
        for (int pos = 0; pos < (int)count ; ++pos) {
            printf("%s theta: %03.2f Dist: %08.2f Q: %d \n", 
                (nodes[pos].flag & SL_LIDAR_RESP_HQ_FLAG_SYNCBIT) ?"S ":"  ", 
                (nodes[pos].angle_z_q14 * 90.f) / 16384.f,
                nodes[pos].dist_mm_q2/4.0f,
                nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);
        }
    }
}


bool checkSLAMTECLIDARHealth(ILidarDriver * drv)
{
    sl_result     op_result;
    sl_lidar_response_device_health_t healthinfo;

    op_result = drv->getHealth(healthinfo);
    if (SL_IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
        printf("SLAMTEC Lidar health status : %d\n", healthinfo.status);
        if (healthinfo.status == SL_LIDAR_STATUS_ERROR) {
            fprintf(stderr, "Error, slamtec lidar internal error detected. Please reboot the device to retry.\n");
            // enable the following code if you want slamtec lidar to be reboot by software
            // drv->reset();
            return false;
        } else {
            return true;
        }

    } else {
        fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
        return false;
    }
}