//
// Created by lichevsky on 15.09.16.
//
#include <cstring>
#include <iostream>
#include "../string_utils.h"
#include "../metrics.h"
using namespace std;

void run_string_test(){
    string test = "one two three\n four five six seven eight nine ten eleven tvelve thirteen fourteen fifteen sixteen";
    string _3ds = "The handheld offers new features such as the StreetPass and SpotPass tag modes, powered by Nintendo Network;\naugmented reality, using its 3D cameras; and Virtual Console, which allows owners to download and play games originally released on older video game systems. It is also pre-loaded with various applications including these: an online distribution store called Nintendo eShop, a social networking service called Miiverse; an Internet Browser; the Netflix, Hulu Plus and YouTube streaming video services; Nintendo Video; a messaging application called Swapnote (known as Nintendo Letter Box in Europe and Australia); and Mii Maker.";
    string text = fmt_text(_3ds, TOP_COLUMNS, TEXT_JUSTIFY_BOTH);
    cout << text << endl;
}