//
// Created by lichevsky on 03.09.16.
//

#ifndef COLORED_TEXT_STRING_UTILS_H
#define COLORED_TEXT_STRING_UTILS_H


#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>

const int TEXT_JUSTIFY_LEFT = 0;
const int TEXT_JUSTIFY_RIGHT = 1;
const int TEXT_JUSTIFY_CENTER = 2;
const int TEXT_JUSTIFY_BOTH = 3;
const int COLOR_BLACK = 0;
const int COLOR_RED = 1;
const int COLOR_GREEN = 2;
const int COLOR_YELLOW = 3;
const int COLOR_BLUE = 4;
const int COLOR_MAGENTA = 5;
const int COLOR_CYAN = 6;
const int COLOR_WHITE = 7;
using  namespace std;
string fmt(vector<string>, size_t, int, bool );
string trim(string input);
string fmt_text(string , size_t , int );
string string_join(vector<string>, string );
string colorize(int foreground, int background, string text);
string colorize(int foreground, string text);
#endif //COLORED_TEXT_STRING_UTILS_H
