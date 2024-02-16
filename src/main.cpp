#include <iostream>
#include "fonction.h"
#include "sl_lidar.h" 
#include "sl_lidar_driver.h"

int main() {

    printf("Ultra simple LIDAR data grabber for SLAMTEC LIDAR.\n"
           "Version: %s\n", SL_LIDAR_SDK_VERSION);

    int result = add(6, 7);
    std::cout << "Result: " << result << std::endl;
    return 0;
}