/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//bit_array.cpp

#include <iostream>
#include "bit_array.h"
using std::cout;
using std::endl;

BitArray::BitArray(){
}

void BitArray::appendBitsInChar(const unsigned char c, unsigned int validBitNum){
    for(unsigned int bitIndex = 0; bitIndex < validBitNum; bitIndex++){
        bool bit = c & (0x80 >> bitIndex);
        mArray.push_back(bit);
    }
}

void BitArray::appendBitsInStr(const string& binaryCode, unsigned int bitNum){
    mArray = vector<bool>(bitNum, false);

    unsigned int base = 0;
    for(auto it = binaryCode.begin(); it != binaryCode.end(); ++it){
        unsigned char c = *it;
        unsigned char checkBit = 0x1 << 7;
        for(unsigned int i = 0; i < 8; i++){
            if(c & checkBit)
                mArray[base + i] = true;
            checkBit = checkBit >> 1;
        }
        base += 8;
    }
}

BitArray::BitArray(const string& binaryCode){
    appendBitsInStr(binaryCode, binaryCode.size() * 8);
}

BitArray::BitArray(const string& binaryCode, unsigned int bitNum){
    appendBitsInStr(binaryCode, bitNum);
}

BitArray::BitArray(unsigned char c){
    appendBitsInChar(c);
}

BitArray::BitArray(unsigned int num) : mArray(32, false){
    unsigned int checkBit = 0x1 << 31;
    for(unsigned int i = 0; i < 32; i++){
        if(num & checkBit)
            mArray[i] = true;
        checkBit = checkBit >> 1;
    }
}

BitArray::BitArray(const BitArray& rhs) : mArray(rhs.mArray){
}

BitArray::BitArray(BitArray&& rhs) : mArray(){
    *this = std::move(rhs);
}

BitArray& BitArray::operator=(const BitArray& rhs){
    mArray = rhs.mArray;
    return *this;
}

BitArray& BitArray::operator=(BitArray&& rhs){
    if(this != &rhs){
        mArray = std::move(rhs.mArray);
        rhs.mArray = vector<bool>();
    }
    return *this;
}

void BitArray::push_back(bool b){
    mArray.push_back(b);
}

size_t BitArray::size() const{
    return mArray.size();
}

bool BitArray::operator[](int index) const{
    return get(index);
}

bool BitArray::get(int index) const{
    return mArray[index];
}

inline bool isEndOfChar(int index){
    return index % 8 == 7;
}

/* If @bitNumInChar is not 8, padding (8 - @bitNumInChar) 0 to c
 */
inline char paddingZero(char c, int bitNumInChar){
    return c << (8 - bitNumInChar);
}
/* Append the last character @c to @str,
 * */
void appendLastChar(string& str, char c, int bitNumOfLastChar){
    if(bitNumOfLastChar == 0)
        return;
    c = paddingZero(c, bitNumOfLastChar);
    str += c;
}

string BitArray::to_binary_str() const{
    string ret = "";
    unsigned char c = 0;
    for(unsigned int i = 0; i < size(); i++){
        c = c << 1 | get(i);
        if(isEndOfChar(i)){
            ret += c;
            c = 0;
        } 
    }
    appendLastChar(ret, c, size() % 8);
    return ret;
}

void BitArray::erase(int start, int length){
    mArray.erase(mArray.begin() + start, mArray.begin() + start + length);
}

string BitArray::to_plain_str() const{
    string ret = "";
    for(unsigned int i = 0; i < size(); i++){
        ret += get(i) ? '1' : '0';
    }
    return ret;
}

bool BitArray::topWithPop(){
    bool ret = mArray.front();
    mArray.erase(mArray.begin());
    return ret;
}

BitArray BitArray::parseFrom(const string& plainStr){
    BitArray ret;
    for(unsigned int i = 0; i < plainStr.size(); i++){
        bool b = (plainStr[i] == '1');
        ret.push_back(b);
    }
    return ret;
}

BitArray& BitArray::operator += (const BitArray& otherCode){
    mArray.reserve(mArray.size() + otherCode.mArray.size());
    mArray.insert(mArray.end(), otherCode.mArray.begin(), otherCode.mArray.end());
    return *this;
}

const BitArray operator+(const BitArray& code1, const BitArray& code2){
    return BitArray(code1) += code2;
}

BitArray getBinaryWithoutHighestOne(unsigned int num){
    BitArray ret(num);
    // remove redudant head zero and first 1
    int firstZeroPos = 0;
    int size = ret.size();
    while(firstZeroPos < size && !ret[firstZeroPos]){
        firstZeroPos++;
    }
    ret.erase(0, firstZeroPos + 1);
    return ret;
}

BitArray getRightmostBits(unsigned int num, unsigned int bitNum){
    string plainStr = "";
    for(int i = 0; i < bitNum; i++){
        char lastBit = (num % 2) ? '1' : '0';
        plainStr.insert(plainStr.begin(), lastBit);
        num = num / 2;
    }
    return BitArray::parseFrom(plainStr);
}

unsigned char decodeChar(BinaryCode& code){
    unsigned char ret = 0;
    for(int i = 0; i < 8; i++){
        ret = (ret << 1) + code.getFirstBitMoveToNext();
    }
    return ret;
}

int decodeChar(BinaryCode& code, unsigned char& c){
    if(code.remainSize() < 8)
        return -1;
    c = decodeChar(code);
    return 0;
}

unsigned int decodeInt(BinaryCode& code){
    unsigned int ret = 0;
    for(int i = 0; i < 32; i++){
        ret = (ret << 1) + code.getFirstBitMoveToNext();
    }
    return ret;
}

int decodeInt(BinaryCode& code, unsigned int& num){
    if(code.remainSize() < 32)
        return -1;
    num = decodeInt(code);
    return 0;
}

int decodePlainStr(BinaryCode& code, string& word, unsigned int length){
    if(code.remainSize() < 8 * length)
        return -1;
    word = "";
    for(unsigned int i = 0; i < length; i++){
        unsigned char c = 0;
        for(unsigned int j = 0; j < 8; j++){
            c = (c << 1) + code.getFirstBitMoveToNext();
        }
        word += c;
    }
    return 0;
}
