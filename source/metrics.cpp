#include "metrics.h"

ScreenDefinition::ScreenDefinition(size_t WIDTH, size_t HEIGHT,
                                 size_t COLUMNS, size_t ROWS,
                                 gfxScreen_t HANDLE) {
    this->WIDTH = WIDTH;
    this->HEIGHT = HEIGHT;
    this->COLUMNS = COLUMNS;
    this->ROWS = ROWS;
    this->HANDLE = HANDLE;
}

size_t ScreenDefinition::getWIDTH() const {
    return WIDTH;
}

size_t ScreenDefinition::getHEIGHT() const {
    return HEIGHT;
}

size_t ScreenDefinition::getCOLUMNS() const {
    return COLUMNS;
}

size_t ScreenDefinition::getROWS() const {
    return ROWS;
}
size_t ScreenDefinition::getPIXELS() const {
    return WIDTH*HEIGHT;
}
size_t ScreenDefinition::getMEMSIZE() const {
    return WIDTH*HEIGHT*BPP;
}
gfxScreen_t ScreenDefinition::getHANDLE() const {
    return HANDLE;
}
