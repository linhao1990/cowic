/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//helper.cpp

#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "helper.h"
#include "compressor_factory.h"

void split(const string &s, char delim, vector<string> &elems) {
    std::stringstream ss(s);
    string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

// trim from start
inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

string int2Str(int num){
    std::stringstream sstream;
    sstream<<num;
    return sstream.str();
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void parseDict(const string& str, unordered_map<string, int>& dict){
    dict.clear();
    vector<string> lines = split(str, '\n');
    for(auto it = lines.begin(); it != lines.end(); it++){
        string line = *it;
        size_t pos = line.find_last_of(' ');
        string word = line.substr(0, pos);
        int freq = atoi(line.substr(pos + 1).c_str());
        dict.insert(std::make_pair<string, int>(std::move(word), std::move(freq)));
    }
}

string cutFirstLine(string& dumpStr){
    size_t pos = dumpStr.find('\n');
    string firstLine = dumpStr.substr(0, pos);
    dumpStr = dumpStr.substr(pos + 1);
    return firstLine;
}

int parseFirstLine2Int(string& dumpStr){
    string numStr = cutFirstLine(dumpStr);
    return atoi(numStr.c_str());
}

string cutNByte(string& str, int byteLen){
    string ret = str.substr(0, byteLen);
    str = str.substr(byteLen);
    return ret;
}

const int MAX_LINE_LEN = 4096;

string cutParseDatafile(int& offset, int byteNum, ifstream& datafile){
    char * buffer = new char[MAX_LINE_LEN];

    datafile.seekg(offset);
    datafile.read(buffer, byteNum);
    offset += byteNum;
    string binaryStr(buffer, byteNum);
    delete[] buffer;

    return binaryStr;
}

string formatIndex(int byteNum){
    std::stringstream sstream;
    sstream << byteNum << "\n";
    return sstream.str();
}

string AuxiliaryFilename::baseFilename = "";
int AuxiliaryFilename::column = 0;
void AuxiliaryFilename::initFilename(const string& filename){
    baseFilename = filename;
    column = 0;
}

string AuxiliaryFilename::getNextFilename(){
    ++column;
    std::stringstream sstream;
    sstream<<baseFilename<<"."<<column<<".aux";
    return sstream.str();
}
