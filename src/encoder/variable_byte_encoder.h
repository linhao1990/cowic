/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//variable_byte_encoder.h

#ifndef _VARIABLE_BYTE_ENCODER_H_
#define _VARIABLE_BYTE_ENCODER_H_

#include "number_encoder.h"

/* Variable byte (VB) encoding uses an integral number of bytes to encode a gap. 
 * The last 7 bits of a byte are ``payload'' and encode part of the gap. 
 * The first bit of the byte is a continuation bit . It is set to 1 for the last byte 
 * of the encoded gap and to 0 otherwise.
 * */
class VariableByteEncoder : public NumberEncoder{
public:
    BitArray encode(unsigned int num) const;
    int decode(BinaryCode& code, unsigned int& num) const;
private:
};

#endif
