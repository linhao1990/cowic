/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//spliter.cpp

#include "spliter.h"

const string openChars = "([{'\"";
const string closeChars = ")]}'\"";
const unsigned char columnDelimiter = ' ';
const unsigned char escape = '\\';
const unsigned int MAX_WORD_LEN = 255;
const unsigned int MAX_NUMBER_LEN = 3;

const unsigned char EOL = 128; //end of line
const string EOL_STR(1, EOL);


void checkFirstOpen(size_t& lastOpenPos, unsigned char c){
    size_t pos = openChars.find(c);
    if(pos != string::npos){
        lastOpenPos = pos;
    }
}

void checkMatchClose(size_t& lastOpenPos, unsigned char c){
    if(c == closeChars[lastOpenPos]){
        lastOpenPos = string::npos;
    }
}

void setLastOpenPos(size_t& lastOpenPos, unsigned char c){
    if(lastOpenPos == string::npos){
        checkFirstOpen(lastOpenPos, c);
    } else{
        checkMatchClose(lastOpenPos, c);
    }
}

inline bool isSplittable(const size_t lastOpenPos){
    return lastOpenPos == string::npos;
}

inline bool isColumnDelimiter(unsigned char c){
    return c == columnDelimiter;
}

void Spliter::splitLine2Columns(const string& line, vector<string>& columns){
    size_t lastOpenPos = string::npos;
    bool isDelimiterFound = false;
    unsigned int lastSplitPos = 0;
    for(unsigned int i = 0; i < line.size(); i++){
        unsigned char c = line[i];
        if(c == escape){ //ignore the escape and next character.
            i++;
            continue;
        }
        if(isDelimiterFound && !isColumnDelimiter(c)){
            //split the column and move to next
            columns.push_back(line.substr(lastSplitPos, i - lastSplitPos));
            lastSplitPos = i;
            isDelimiterFound = false;
        } 
        setLastOpenPos(lastOpenPos, c);

        if(isSplittable(lastOpenPos) && isColumnDelimiter(c)){
            isDelimiterFound = true;
        }
    }
    if(lastSplitPos < line.size()){
        columns.push_back(line.substr(lastSplitPos));
    }
}

enum class CharacterType{
    Number,
    Alphabet,
    Special
};

CharacterType characterType(unsigned char c){
    if(c >= '0' && c <= '9')
        return CharacterType::Number;
    else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        return CharacterType::Alphabet;
    else
        return CharacterType::Special;
}

inline bool reachMaxNumberLength(CharacterType type, unsigned int pos, unsigned int lastPos){
    return type == CharacterType::Number && pos - lastPos >= MAX_NUMBER_LEN;
}

inline bool reachMaxWordLengh(unsigned int pos, unsigned int lastPos){
    return pos - lastPos >= MAX_WORD_LEN;
}

void Spliter::splitColumn2Words(const string& column, vector<string>& words){
    if(column.size() == 0)
        return;
    unsigned int lastSplitPos = 0;
    CharacterType lastType = characterType(column[0]);
    for(unsigned int i = 1; i < column.size(); i++){
        CharacterType currType = characterType(column[i]);
        if(lastType!= currType || reachMaxNumberLength(currType, i, lastSplitPos) || reachMaxWordLengh(i, lastSplitPos)){
            words.push_back(column.substr(lastSplitPos, i - lastSplitPos));
            lastSplitPos = i;
        }
        lastType = currType;
    }
    if(lastSplitPos < column.size()){
        words.push_back(column.substr(lastSplitPos));
    }
}

bool Spliter::isNewColumn(size_t& lastOpenPos, const string& word){
    //TODO try some aggresive improvement
    for(unsigned int i = 0; i < word.size(); i++){
        unsigned char c = word[i];
        if(c == escape){ //ignore the escape and next character.
            i++;
            continue;
        }
        setLastOpenPos(lastOpenPos, c);
        if((isSplittable(lastOpenPos) && isColumnDelimiter(c)) || c == EOL)
            return true;
    }
    return false;
}
