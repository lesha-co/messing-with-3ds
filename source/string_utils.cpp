//
// Created by lichevsky on 03.09.16.
//
#include <iostream>
#include <cstdio>
#include <algorithm>
#include "string_utils.h"
#include "vector_utils.h"
#include "console_debug.h"

using namespace std;
const string SPACE = " ";

namespace patch {
    template < typename T > std::string to_string( const T& n ) {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

string string_join(vector<string> input, string delim){
    std::stringstream ss;
    for(size_t i = 0; i < input.size(); ++i)
    {
        if(i != 0)
            ss << delim;
        ss << input[i];
    }
    return ss.str();
}
string repeat(string s, size_t times) {
    std::stringstream ss;
    for(size_t i = 0; i < times; i++)
        ss << s;
    return ss.str();
}
string trim_beginning(string input){
    for(size_t i = 0; i< input.length(); i++){
        if(!isspace(input.at(i))){
            return input.substr(i);
        }
    }
    return "";
}
string trim_tail(string input){
    for(size_t i = input.length()-1; i>=0; i--){
        if(!isspace(input.at(i))){
            return input.substr(0, i+1);
        }
    }
    return "";
}
string trim(string input){
    return trim_beginning(trim_tail(input));
}
vector<string> split(const string &str, string delimiter) {
    string trimmed = trim(str);
    vector<string> elems;

    size_t last = 0;
    size_t next = 0;
    while ((next = trimmed.find(delimiter, last)) != string::npos) {
        elems.push_back(trimmed.substr(last, next-last));
        last = next + 1;
    }
    elems.push_back(trimmed.substr(last));
    return elems;
}
size_t get_length(vector<string> vector1, string delimiter, bool trailing_delimiter){
    if(vector1.size() == 0) {
        //printf("vector %x EMPTY\n ", &vector1);
        return 0;
    }
    size_t total_length = 0;
    for(string s: vector1){
        total_length+= s.size();
        if(
                s != vector1.back() ||
                (s == vector1.back() && trailing_delimiter)
        ){
            total_length+= delimiter.size();
        }
    }
    //printf("asked length, vector %x size %d, length %d\n", &vector1, vector1.size(), total_length);
    return total_length;
}

vector<vector<string>> split_text(string input, size_t maxwidth){
    //error("in split_text...");
    vector<string> words = split(input, SPACE);
    //error(string("words split, #words: ") + patch::to_string(words.size()));
    vector<vector<string>> text;
    vector<string> last;
    for(string word : words) {

        size_t len = get_length(last, SPACE, true);
        if( (len + word.length()) <= maxwidth){
            last.push_back(word);
            //printf("adding to string, new width %d\n%s\n",
            //       get_length(last, SPACE, false),
            //       string_join(last, SPACE).c_str());
        } else {
            //error("NEW STRING");

            text.push_back(last);
            vector<string> newOne;
            last = newOne;
            last.push_back(word);
        }
    }
    if(last.size()){
        text.push_back(last);
    }
    return text;
}
string fmt_paragraph(string input, size_t width, int justify){
    error("calling split_text...");
    vector<vector<string>> text = split_text(input, width);
    error(string("split text called, ") + patch::to_string(text.size()) + string(" lines") );

    vector<string> lines;
    for (vector<string> line: text){
        //error(string("parsing line ") + string_join(line, " "));
        int j = justify;
        if(line == text.back()){
            j = TEXT_JUSTIFY_LEFT;
        }
        string s = fmt(line, width, j, true);
        if(s.length() != width){
            error("string not full! " + patch::to_string(s.length()) +"\n====\n"+ s);
        }
        lines.push_back(s);
    }
    return string_join(lines, "");
}
string fmt_text(string input, const size_t width, const int justify){
    vector<string> paragraphs = split(input, "\n");
    for (size_t i = 0; i<paragraphs.size(); i++){
        paragraphs[i] = fmt_paragraph(trim(paragraphs[i]), width, justify);
    }

    return string_join(paragraphs, "\n");
}
string fmt(vector<string> words, size_t width, int justify, bool tolerate_long_strings){
    if(get_length(words, SPACE, false) > width){
        if(tolerate_long_strings){
            return string_join(words, SPACE);
        } else {
            error(string("string too long ") + string_join(words, " ").c_str());

        }

    }

    switch (justify){
        case TEXT_JUSTIFY_LEFT:{

            string r= string_join(words, SPACE);
            size_t add_spaces = width-r.length();
            string spaces= repeat(SPACE, add_spaces);
            return r+spaces;
        }

        case TEXT_JUSTIFY_RIGHT:{
            string r= string_join(words, SPACE);
            size_t add_spaces = width-r.length();
            string spaces= repeat(SPACE, add_spaces);
            return spaces+r;
        }
        case TEXT_JUSTIFY_CENTER:{
            string r= string_join(words, SPACE);
            size_t add_spaces = width-r.length();
            size_t add_spaces_left = add_spaces/2;
            size_t add_spaces_right = add_spaces-add_spaces_left;
            string spaces_left = repeat(SPACE, add_spaces_left);
            string spaces_right = repeat(SPACE, add_spaces_right);
            return spaces_left + string_join(words, SPACE) + spaces_right;
        }
        case TEXT_JUSTIFY_BOTH:{
            string result = "";
            words = strip_empty_strings(words);
            size_t spaces_count = words.size() -1;
            if (spaces_count == 0){
                // we have only one string;
                return fmt(words, width, TEXT_JUSTIFY_LEFT, tolerate_long_strings);
            } else {
                size_t total_no_space_chars = 0;

                // we calculating total amount of chars that are not spaces
                for(string word: words){
                    total_no_space_chars += word.size();
                }
                // everything else is a space
                size_t add_spaces = width - total_no_space_chars;

                size_t space_size = add_spaces / spaces_count;
                size_t extra_spaces = add_spaces - space_size*spaces_count;
                //printf("letters %d  sp_count %d  sp_sz %d extra %d\n", total_no_space_chars, spaces_count, space_size, extra_spaces);
                for(size_t i = 0; i< words.size(); i++){
                    size_t cur_space_size = space_size;
                    if (i < extra_spaces){
                        cur_space_size = space_size+1;
                    }

                    result += words.at(i);
                    if(i< words.size()-1){
                        result+= repeat(SPACE, cur_space_size);
                    }
                }
            }
            return result;
        }
        default:
            error(string("illegal justify parameter"));

    }
    return string_join(words, SPACE);
}

string colorize(int foreground, int background, string text){
    std::ostringstream stringStream;
    stringStream << "\x1b[4" << background << ";3" << foreground << "m" << text << "\x1b[0m";
    return stringStream.str();
}
string colorize(int foreground, string text){
    return colorize(foreground, COLOR_BLACK, text);
}