#include "lidarAnalize.h"


void convertAngularToAxial(lidarAnalize_t* data, int count, position_t position){
    for(int i = 0; i< count; i++){
        if(data[i].valid){
            data[i].x = data[i].dist*cos(data[i].angle*DEG_TO_RAD);
            data[i].y = -data[i].dist*sin(data[i].angle*DEG_TO_RAD);
        }
    }
}

void printLidarAxial(lidarAnalize_t* data, int count){
    for(int i = 0; i< count; i++){
        if(data[i].valid){
            printf("x : %d \ty : %d\n",data[i].x,data[i].y);
        }
        else{
            printf("non valid\n");
        }
    }
}

void printAngular(lidarAnalize_t* data, int count){
    for(int i = 0; i< count; i++){
        if(data[i].valid){
            printf("theta: %03.2f \tDist: %08.2f\n",data[i].angle,data[i].dist);
        }
        else{
            printf("non valid\n");
        }
    }
}