#pragma once


class tableState
{
private:
    /* data */
    bool planteStockFull[6];
    bool JardiniereFull[6];
    bool zoneFull[6];
    bool panneauSolaireRotate[9];

public:
    tableState(/* args */);
    ~tableState();
};

