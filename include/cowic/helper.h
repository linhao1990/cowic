//helper.h

#ifndef _HELPER_H_
#define _HELPER_H_

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

#include "bit_array.h"

using std::string;
using std::vector;
using std::ifstream;
using std::unordered_map;

void split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
string int2Str(int num);
void replaceAll(std::string& str, const std::string& from, const std::string& to);
void parseDict(const string& str, unordered_map<string, int>& dict);
string cutFirstLine(string& dumpStr);
int parseFirstLine2Int(string& dumpStr);
string cutNByte(string& str, int byteLen);
string cutParseDatafile(int& offset, int byteNum, ifstream& datafile);
/* Index file mark the begin and len info of each entry.
 * Each line contains info of one entry.
 * Format:
 * <BITNUM>
 * */
string formatIndex(int byteNum);

class AuxiliaryFilename{
public:
    static void initFilename(const string& filename);
    static string getNextFilename();
    static string baseFilename;
    static int column;
};

#endif
