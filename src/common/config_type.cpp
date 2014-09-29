/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//config_type.cpp

#include <iostream>

#include "string.h"
#include "config_type.h"


template <>
const char* EnumName<ModelType>::List[] =
{
    "WordDict",
    "PhraseDict",
    "ColumnWised"
};

template <>
const char* EnumName<DictEncoderType>::List[] =
{
    "Huffman",
    "Canonical"
};

template <>
const char* EnumName<FreshEncoderType>::List[] =
{
    "Plain",
    "Auxiliary"
};

template <>
const char* EnumName<NumberEncoderType>::List[] =
{
    "Unary",
    "Gamma",
    "Delta"
};

template <>
const char* EnumName<ColumnModelType>::List[] =
{
    "WordDict",
    "PhraseDict",
    "Timestamp",
    "IP",
    "Number",
    "LongNumber",
    "FixPrecisionNumber"
};

template <>
const char* EnumName<DictColumnModelType>::List[] =
{
    "WordDict",
    "PhraseDict"
};

template <>
const char* EnumName<SpecifyColumnModelType>::List[] =
{
    "Timestamp",
    "IP",
    "Number",
    "FixPrecisionNumber"
};

template<typename EnumType>
EnumType ConvertStringToEnum(const char* pStr)
{
    EnumType fooEnum = static_cast<EnumType>(-1);
    int count = sizeof(EnumName<EnumType>::List) /
        sizeof(EnumName<EnumType>::List[0]);
    int i = 0;
    for(; i < count; ++i)
    {
        if (strcmp(pStr, EnumName<EnumType>::List[i]) == 0)
        {
            fooEnum = static_cast<EnumType>(i);
            break;
        }
    }
    if(i == count){
        std::cerr << "\nError: config file error! '" << pStr << "' cannot be parsed into an enumerate type!" << '\n';
        exit(EXIT_FAILURE);
    }
    return fooEnum;
}

//explicit template instantiation
template ModelType ConvertStringToEnum<ModelType>(const char* pStr);
template DictEncoderType ConvertStringToEnum<DictEncoderType>(const char* pStr);
template FreshEncoderType ConvertStringToEnum<FreshEncoderType>(const char* pStr);
template NumberEncoderType ConvertStringToEnum<NumberEncoderType>(const char* pStr);
template ColumnModelType ConvertStringToEnum<ColumnModelType>(const char* pStr);
template DictColumnModelType ConvertStringToEnum<DictColumnModelType>(const char* pStr);
template SpecifyColumnModelType ConvertStringToEnum<SpecifyColumnModelType>(const char* pStr);
