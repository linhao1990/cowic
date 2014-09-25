//spliter.h

#ifndef _SPLITER_H
#define _SPLITER_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Spliter{
public:
    static void splitLine2Columns(const string& line, vector<string>& columns);
    static void splitColumn2Words(const string& column, vector<string>& words);
    static bool isNewColumn(size_t& lastOpenPos, const string& word);
private:
};

extern const string EOL_STR;
#endif
