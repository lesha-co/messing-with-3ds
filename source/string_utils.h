//
// Created by lichevsky on 03.09.16.
//

#ifndef COLORED_TEXT_STRING_UTILS_H
#define COLORED_TEXT_STRING_UTILS_H


#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>

const int TEXT_JUSIFY_LEFT = 0;
const int TEXT_JUSIFY_RIGHT = 1;
const int TEXT_JUSIFY_CENTER = 2;
const int TEXT_JUSIFY_BOTH = 3;
using  namespace std;
string fmt(vector<string>, size_t, int, bool );
string trim(string input);
string fmt_text(string , size_t , int );
string string_join(vector<string>, string );
#endif //COLORED_TEXT_STRING_UTILS_H
