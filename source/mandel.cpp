//
// Created by lichevsky on 04.09.16.
//
#include <3ds/types.h>
#include <cstring>
#include "mandel.h"

using namespace std;

template<class T>
T length(complex<T> c){
    return sqrt( c.imag()*c.imag()+c.real()*c.real());
}


size_t iter(complex<double> c, size_t limit, double radius){
    complex<double> z  (0,0);
    size_t i = 0;
    for(; i< limit; i++){
        z = z*z + c;
        if (length(z) > radius){
            break;
        }
    }
    return i;
}

vector<double> arrange(size_t n, double from, double to){
    double difference = to-from;
    double section_length = difference / n;
    vector<double> v(n, 0);
    for (size_t i = 0; i < n; ++i) {
        v[i] = from + i * section_length + 0.5 * section_length;
    }
    return v;
}
vector<bgr_pixel> prepareGFXPallet(size_t grades){
    vector<bgr_pixel> p;
    vector<bgr_pixel> key_points = {
            //  b     g     r
            {0x00 ,0x00, 0x00},
            {0x00 ,0x00, 0xFF},
            {0x00 ,0xFF, 0xFF},
            {0x00 ,0xFF, 0x00},
            {0xFF ,0xFF, 0x00},
            {0xFF ,0x00, 0x00},
            {0xFF ,0x00, 0xFF},
            {0xFF ,0xFF, 0xFF},
    };
    for (size_t i = 0; i < key_points.size()-1; ++i) {
        bgr_pixel current = key_points[i];
        bgr_pixel next = key_points[i+1];
        // b
        s8 diff_b = next.b - current.b;
        u8 step_b  =  (u8)((double)diff_b/(double)grades);
        s8 diff_g = next.g - current.g;
        u8 step_g  =  (u8)((double)diff_g/(double)grades);
        s8 diff_r = next.r - current.r;
        u8 step_r  =  (u8)((double)diff_r/(double)grades);
        for (s8 j = 0; j < grades; ++j) {
            p.push_back({current.b + step_b*j,
                         current.g + step_g*j,
                         current.r + step_r*j});
        }
    }
    return p;
}

vector<string> preparePallet(bool useColors){
    vector<string> pallet;
    if(useColors){
        vector<size_t> colors=  {
                0, // black
                1, // red
                3, // yellow
                2, // green
                6, // cyan
                4, // blue
                5, // magenta
                7 // white
        };
        vector<string> grades = {
                " ", "\xB0","\xB1","\xB2"
        };

        for (size_t i = 0; i < colors.size() - 1; ++i) {
            for (size_t j = 0; j < grades.size(); ++j) {
                std::ostringstream stringStream;
                stringStream << "\x1b[4" << colors[i] << ";3" << colors[i+1] << "m" << grades[j] << "\x1b[0m";
                pallet.push_back(stringStream.str());
            }
        }

    }
    else pallet = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    return pallet;
}

histogram_acc histogram(vector<vector<size_t>> input){
    histogram_acc h;
    for(vector<size_t> line: input)
        for(size_t value: line){
            h.put(value);
        }
    return h;
};

map<size_t ,size_t> buildPalletMappings(histogram_acc h, size_t pallet_size){
    size_t totalEntries = 0;
    size_t entries_counter = 0;
    map<size_t ,size_t> hist = h.get();
    map<size_t ,size_t> mapping;
    for(auto iterator = hist.begin(); iterator != hist.end(); iterator++){
        totalEntries+=iterator->second;
    }
    //printf("total hist %d\n", totalEntries);
    size_t entries_per_pallet = totalEntries/pallet_size;

    for(auto iterator = hist.begin(); iterator != hist.end(); iterator++){
        entries_counter+=iterator->second;
        size_t selected =entries_counter/entries_per_pallet;
        if(selected == pallet_size){
            selected = pallet_size-1;
        }
        mapping[iterator->first] = selected;
    }

    return mapping;

}

vector<vector<size_t>> getField(fieldDef field, size_t max_iter, double radius){
    vector<vector<size_t>> iters (field.nIM, vector<size_t>(field.nRE, 0));
    vector<double> REs = arrange(field.nRE, field.RE_from, field.RE_to);
    vector<double> IMs = arrange(field.nIM, field.IM_from, field.IM_to);

    for (size_t i = 0; i < field.nIM; ++i) {
        for (size_t j = 0; j < field.nRE; ++j) {
            complex<double> c (REs[j], IMs[i]);

            iters[i][j] = iter(c, max_iter, radius);
            if(iters[i][j] >  max_iter){
                printf("\nerror\n");
            }
        }
    }
    return iters;
}