//
// Created by lichevsky on 03.09.16.
//


#include "console_debug.h"
void error(string text){
    printf("\x1b[30;41m[ FAIL ]\x1b[0m %s\n", text.c_str() );
}