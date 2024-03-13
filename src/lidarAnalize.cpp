#include "lidarAnalize.h"


void convertAngularToAxial(lidarAnalize_t* data, int count, position_t position){
    for(int i = 0; i< count; i++){
        if(data[i].valid){
            data[i].x = data[i].dist*cos((data[i].angle+position.teta)*DEG_TO_RAD) + position.x;
            data[i].y = -data[i].dist*sin((data[i].angle+position.teta)*DEG_TO_RAD) + position.y;
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


void maxDistance(lidarAnalize_t* data, int count,int& maxX, int maxY){
    maxX = 0;
    maxY = 0;
    for(int i = 0; i< count; i++){
        if(abs(data[i].x) > maxX)
            maxX = abs(data[i].x);
        if(abs(data[i].y) > maxY)
            maxY = abs(data[i].y);
    }
}


void pixelArtPrint(lidarAnalize_t* data, int count,int sizeX,int sizeY,int scale,position_t position){
    
    // if(scale< 1){
    //     int maxX, maxY;
    //     maxDistance(data, count, maxX, maxY);
    //     int scaleX = (maxX*2)/sizeX;
    //     int scaleY = (maxY*2)/sizeY;
    //     scale = scaleX>scaleY? scaleX: scaleY;
    // }

    char* matriceAffichage;
    matriceAffichage = (char*)malloc(sizeX * sizeY * sizeof(char));
    
    //initMatrice
    for(int i = 0; i<sizeX * sizeY; i++){
        matriceAffichage[i] = ' ';
    }

    int posix, posiy;
    for(int i = 0; i<count; i++){
        if(data[i].valid == true){
            posix = data[i].x/scale + sizeX/2;
            posiy = data[i].y/scale + sizeY/2;
            if(posix<sizeX && posix>=0 && posiy<sizeY && posiy>=0)
                matriceAffichage[posix + sizeX * posiy] = 'X';
            else{
                if(posix>=sizeX)
                    posix = sizeX-1;
                if(posix<0)
                    posix = 0;
                if(posiy>=sizeY)
                    posiy = sizeY-1;
                if(posiy<0)
                    posiy = 0;
                matriceAffichage[posix + sizeX * posiy] = 'W';
            }
        }
    }

    //fill

    int positionRoboty = position.x/scale + sizeX/2;
    int positionRobotx = position.y/scale + sizeY/2;

    for(int i = 0; i<sizeX; i++){
        bool bValid = false;
        for(int j = positionRoboty; j<sizeY; j++){
            int posX = MAP(j,positionRoboty,sizeY,positionRobotx,i);
            if(matriceAffichage[posX + sizeX * j] != ' '){
                bValid = true;
            }                
            if(bValid == true){
                matriceAffichage[posX + sizeX * j] = 'X';
            }
        }
    }

    for(int i = 0; i<sizeX; i++){
        bool bValid = false;
        for(int j = positionRoboty; j>=0; j--){
            int posX = MAP(j,positionRoboty,0,positionRobotx,i);
            if(matriceAffichage[posX + sizeX * j] != ' '){
                bValid = true;
            }                
            if(bValid == true){
                matriceAffichage[posX + sizeX * j] = 'X';
            }
        }
    }


    for(int j = 0; j<sizeY; j++){
        bool bValid = false;
        for(int i = positionRobotx; i<sizeX; i++){
            int posY = MAP(i,positionRobotx,sizeX,positionRoboty,j);
            if(matriceAffichage[i + sizeX * posY] != ' '){
                bValid = true;
            }                
            if(bValid == true){
                matriceAffichage[i + sizeX * posY] = 'X';
            }
        }
    }

    for(int j = 0; j<sizeY; j++){
        bool bValid = false;
        for(int i = positionRobotx; i>=0; i--){
            int posY = MAP(i,positionRobotx,0,positionRoboty,j);
            if(matriceAffichage[i + sizeX * posY] != ' '){
                bValid = true;
            }                
            if(bValid == true){
                matriceAffichage[i + sizeX * posY] = 'X';
            }
        }
    }
    
    //add border
    int posiyPos, posiyNeg;
    for(int i = 0; i<sizeX; i+=(sizeX/scale)+1){
        posix = i;
        posiyPos = 1500/scale + sizeY/2;
        posiyNeg = (-1500)/scale + sizeY/2;
        if(posix<sizeX && posix>=0 && posiyPos<sizeY && posiyPos>=0 && posiyNeg<sizeY && posiyNeg>=0)
            matriceAffichage[posix + sizeX * posiyPos] = 'Z';
            matriceAffichage[posix + sizeX * posiyNeg] = 'Z';
    }
    int posixPos, posixNeg;
    for(int i = 0; i<sizeY; i+=(sizeY/scale)+1){
        posixNeg = (-1000)/scale + sizeX/2;
        posixPos = 1000/scale + sizeX/2;
        posiy = i;
        if(posixPos<sizeX && posixPos>=0 && posixNeg<sizeX && posixNeg>=0  && posiyNeg<sizeY && posiyNeg>=0)
            matriceAffichage[posixPos + sizeX * posiy] = 'Z';
            matriceAffichage[posiyNeg + sizeX * posiy] = 'Z';
    }


    //A B
    // O
    //C D
    // posix = 1000/scale + sizeX/2;
    // posiy = 1000/scale + sizeY/2;
    // matriceAffichage[posix + sizeX * posiy] = 'A';
    // posix = 1000/scale + sizeX/2;
    // posiy = (-1000)/scale + sizeY/2;
    // matriceAffichage[posix + sizeX * posiy] = 'B';
    // posix = -1000/scale + sizeX/2;
    // posiy = 1000/scale + sizeY/2;
    // matriceAffichage[posix + sizeX * posiy] = 'C';
    // posix = -1000/scale + sizeX/2;
    // posiy = -1000/scale + sizeY/2;
    // matriceAffichage[posix + sizeX * posiy] = 'D';

    posix = position.x/scale + sizeX/2;
    posiy = position.y/scale + sizeY/2;
    if(posix<sizeX && posix>=0 && posiy<sizeY && posiy>=0)
        matriceAffichage[posix + sizeX * posiy] = 'O';

    //print
    for(int x = sizeX-1; x>=0; x--){
        for(int y = sizeY-1; y>=0; y--){
            printf("%c%c",matriceAffichage[x + y * sizeX],matriceAffichage[x + y * sizeX]);
        }
        printf("\n");
    }

    free(matriceAffichage);
}