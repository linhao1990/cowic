/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//config_type.h

#ifndef _CONFIG_TYPE_H_
#define _CONFIG_TYPE_H_

template <typename EnumType>
struct EnumName
{
    static const char* List[];
};

enum class ModelType{
    WordDict,
    PhraseDict,
    ColumnWised
};
template <>
const char* EnumName<ModelType>::List[];

enum class DictEncoderType{
    Huffman,
    Canonical
};
template <>
const char* EnumName<DictEncoderType>::List[];

enum class FreshEncoderType{
    Plain,
    Auxiliary
};
template <>
const char* EnumName<FreshEncoderType>::List[];

enum class NumberEncoderType{
    Unary,
    Gamma,
    Delta
};
template <>
const char* EnumName<NumberEncoderType>::List[];

enum class ColumnModelType{
    WordDict,
    PhraseDict,
    Timestamp,
    IP,
    Number,
    LongNumber,
    FixPrecisionNumber
};
template <>
const char* EnumName<ColumnModelType>::List[];

enum class DictColumnModelType{
    WordDict,
    PhraseDict,
};
template <>
const char* EnumName<DictColumnModelType>::List[];

enum class SpecifyColumnModelType{
    Timestamp,
    IP,
    Number,
    LongNumber,
    FixPrecisionNumber
};
template <>
const char* EnumName<SpecifyColumnModelType>::List[];

template<typename EnumType>
EnumType ConvertStringToEnum(const char* pStr);

#endif
