//
// Created by lichevsky on 04.09.16.
//

#ifndef INC_3DS_MESS_HISTOGRAM_ACC_H
#define INC_3DS_MESS_HISTOGRAM_ACC_H


#include <cstdio>
#include <map>
using namespace std;
class histogram_acc {
public:
    histogram_acc();
    void put(size_t);
    map<size_t , size_t > get();

private:
    map<size_t , size_t > store;
};


#endif //INC_3DS_MESS_HISTOGRAM_ACC_H
