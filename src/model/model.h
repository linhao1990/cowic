/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//model.h
#ifndef _MODEL_H
#define _MODEL_H

#include <iostream>
#include <string>

#include "bit_array.h"

using std::string;
using std::cout;
using std::endl;

class Model{
public:
    Model();
    virtual void updateColumn(const string& columnStr, int column) = 0;
    virtual string dump() const = 0;
    virtual void parse(string& dumpStr) = 0;
    /** Return compressed form of word.
     *  Side effect: will update model for future.
     */
    virtual BitArray compressColumn(const string& columnStr, int column) = 0; 
    virtual int decompressColumn(BinaryCode& code, string& columnStr, int column) const = 0;
    virtual string getModelName() const = 0 ;
    virtual ~Model();
private:
};

#endif
