/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//bit_array.h
#ifndef _BIT_ARRAY_H
#define _BIT_ARRAY_H

#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

using std::cout;
using std::endl;

class BitArray{
public:
    BitArray();
    /** All bits in binaryCode is valid. Push back each bit in the string.
     */
    BitArray(const string& binaryCode);
    /* Only the first bitNum bits in the string is valid. The remaining bits will be ignored.
     */
    BitArray(const string& binaryCode, unsigned int bitNum);
    BitArray(unsigned char c);
    BitArray(unsigned int num);
    BitArray(const BitArray& rhs);
    BitArray(BitArray&& rhs);
    BitArray& operator=(const BitArray& rhs);
    BitArray& operator=(BitArray&& rhs);
    void push_back(bool b);
    size_t size() const;
    bool get(int index) const;
    bool operator[](int index) const;
    /** transform the bits into binary string. organize 8 bits into a char.
     *  padding 0 if there are not enough bits.
     */
    string to_binary_str() const;
    /** transform the bits into plain str, each bit is represent by a char '0' or '1'.
     */
    string to_plain_str() const;
    void erase(int start, int length);
    /* The user shall gurantee that size() > 0, otherwise it will causes undefined behaviour.
     * */
    bool topWithPop();
    BitArray& operator +=(const BitArray& otherCode);
    static BitArray parseFrom(const string& plainStr);
private:
    vector<bool> mArray;
    void appendBitsInChar(unsigned char c, unsigned int validBitNum = 8);
    void appendBitsInStr(const string& binaryCode, unsigned int bitNum);
};

const BitArray operator+(const BitArray& code1, const BitArray& code2);
// used in unary code
BitArray getBinaryWithoutHighestOne(unsigned int num);
BitArray getRightmostBits(unsigned int num, unsigned int bitNum);


//A wrapper of BitArray with pos point to the locaction has been decoded.
//So we do not need to modify code while decode.
class BinaryCode{
public:
    BinaryCode(BitArray&& codeVal) : code(std::move(codeVal)), codeLen(code.size()), pos(0){}
    inline bool finish() const{
        return remainSize() == 0;
    }
    inline bool getFirstBitMoveToNext(){
        return code[pos++];
    }
    inline size_t remainSize() const{
        return codeLen - pos;
    }
    const BitArray code;
    const size_t codeLen;
    int pos;
};

unsigned char decodeChar(BinaryCode& code);
int decodeChar(BinaryCode& code, unsigned char& c);
unsigned int decodeInt(BinaryCode& code);
int decodeInt(BinaryCode& code, unsigned int& num);
int decodePlainStr(BinaryCode& code, string& word, unsigned int length);

#endif
