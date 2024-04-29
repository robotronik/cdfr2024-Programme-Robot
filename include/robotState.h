#pragma once 


typedef enum {
    BLUE,
    YELLOW
} colorTeam_t;

class robotState
{
private:
    
public:
    colorTeam_t colorTeam;
    int collide;
public:
    robotState();
    ~robotState();
};

