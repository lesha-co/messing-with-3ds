//
// Created by lichevsky on 03.09.16.
//

#include "vector_utils.h"

using namespace std;

vector<string> strip_empty_strings(vector<string> input){
    vector<string> result;
    for(string s : input){
        if(s != ""){
            result.push_back(s);
        }
    }
    return result;
}