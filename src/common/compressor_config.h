/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//compressor_config.h

#ifndef _COMPRESSOR_CONFIG_H_
#define _COMPRESSOR_CONFIG_H_

#include <string>
#include <vector>
#include <memory>

#include "config_type.h"

using std::string;
using std::vector;
using std::shared_ptr;

class SpecifyConfig{
public:
    SpecifyConfig(int columnVal, SpecifyColumnModelType modelTypeVal, const string& argVal);
    int getColumn() const;
    SpecifyColumnModelType getModelType() const;
    string getArg() const;
    static SpecifyConfig parse(const string& str);
private:
    int column;
    SpecifyColumnModelType modelType;
    string arg;
};

typedef struct{
    ModelType modelType;
    DictEncoderType dictEncoderType;
    FreshEncoderType freshEncoderType;
    NumberEncoderType numberEncoderType;
    DictColumnModelType dictColumnModelType;
    vector<SpecifyConfig> specifyConfigs;
} CompressorConfiguration;

class CompressorConfigSingleton{
public:
    static shared_ptr<CompressorConfigSingleton> getInstance();
    /* @return value corresponding this key. '' if key is not found.
     * */
    ModelType getModelType();
    DictEncoderType getDictEncoderType();
    FreshEncoderType getFreshEncoderType();
    NumberEncoderType getNumberEncoderType();
    DictColumnModelType getDictColumnModelType();
    SpecifyConfig* getSpecifyConfig(int column);
private:
    CompressorConfigSingleton();
    static shared_ptr<CompressorConfigSingleton> mInstance;
    static const string filename;
    static CompressorConfiguration config;
};

#endif
