//
// Created by lichevsky on 06.09.16.
//

#ifndef INC_3DS_MESS_METRICS_H
#define INC_3DS_MESS_METRICS_H

#include <cstdlib>
#include <3ds/gfx.h>

const size_t BPP = 3;



class ScreenDefinition{
public:
    ScreenDefinition(size_t WIDTH, size_t HEIGHT,
                    size_t COLUMNS, size_t ROWS,
                    gfxScreen_t HANDLE);
    size_t getWIDTH() const;
    size_t getHEIGHT() const;
    size_t getCOLUMNS() const;
    size_t getROWS() const;
    size_t getPIXELS() const;
    size_t getMEMSIZE() const;
    gfxScreen_t getHANDLE() const;
private:
    size_t WIDTH;
    size_t HEIGHT;
    size_t COLUMNS;
    size_t ROWS;
    gfxScreen_t HANDLE;
};

const ScreenDefinition TOP_SCREEN = ScreenDefinition(
        400,240,50,30,GFX_TOP
);
const ScreenDefinition BOTTOM_SCREEN = ScreenDefinition(
        320,240,40,30,GFX_BOTTOM
);


#endif //INC_3DS_MESS_METRICS_H
