//
// Created by lichevsky on 06.09.16.
//

#ifndef INC_3DS_MESS_METRICS_H
#define INC_3DS_MESS_METRICS_H

#include <cstdlib>

const size_t BPP = 3;


// Top screen
const size_t TOP_COLUMNS = 50;
const size_t TOP_LINES = 30;

const size_t TOP_WIDTH = 400;
const size_t TOP_HEIGHT = 240;

const size_t TOP_PIXELS = TOP_WIDTH * TOP_HEIGHT;
const size_t TOP_MEMSIZE = TOP_PIXELS*BPP;

// Bottom screen
const size_t BOTTOM_COLUMNS = 40;
const size_t BOTTOM_LINES = 30;

const size_t BOTTOM_WIDTH = 400;
const size_t BOTTOM_HEIGHT = 240;

const size_t BOTTOM_PIXELS = BOTTOM_WIDTH * BOTTOM_HEIGHT;
const size_t BOTTOM_MEMSIZE = TOP_PIXELS*BPP;

#endif //INC_3DS_MESS_METRICS_H
