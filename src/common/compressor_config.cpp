/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//compressor_config.cpp

#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "compressor_config.h"
#include "helper.h"
#include "ini.h"

SpecifyConfig::SpecifyConfig(int columnVal, SpecifyColumnModelType modelTypeVal, const string& argVal)
    : column(columnVal), modelType(modelTypeVal), arg(argVal){
}

int SpecifyConfig::getColumn() const{
    return column;
}

SpecifyColumnModelType SpecifyConfig::getModelType() const{
    return modelType;
}

string SpecifyConfig::getArg() const{
    return arg;
}

SpecifyConfig SpecifyConfig::parse(const string& str){
    vector<string> detailStrs = split(str, ';');
    if(detailStrs.size() < 2){
        std::cerr << "\nError: config file error! '" << str << "' is not legal SpecifyColumnModel configuration!" << '\n';
        exit(EXIT_FAILURE);
    }
    int column = atoi(detailStrs[0].c_str());
    SpecifyColumnModelType modelType = ConvertStringToEnum<SpecifyColumnModelType>(detailStrs[1].c_str());
    string arg = "";
    if(detailStrs.size() > 2){
        arg = detailStrs[2];
        //trim first " and last "
        arg = arg.substr(1, arg.length() - 2);
    }
    return SpecifyConfig(column, modelType, arg);
}

const string CompressorConfigSingleton::filename = "config.ini";
shared_ptr<CompressorConfigSingleton> CompressorConfigSingleton::mInstance = NULL;
CompressorConfiguration CompressorConfigSingleton::config;

CompressorConfigSingleton::CompressorConfigSingleton(){
}

static int handler(void* user, const char* section, const char* name,
                   const char* value)
{
    CompressorConfiguration* pconfig = (CompressorConfiguration*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("config", "Model")) {
        pconfig->modelType = ConvertStringToEnum<ModelType>(value);
    } else if (MATCH("config", "DictEncoder")) {
        pconfig->dictEncoderType = ConvertStringToEnum<DictEncoderType>(value);
    } else if (MATCH("config", "FreshEncoder")) {
        pconfig->freshEncoderType= ConvertStringToEnum<FreshEncoderType>(value);
    } else if (MATCH("config", "NumberEncoder")) {
        pconfig->numberEncoderType = ConvertStringToEnum<NumberEncoderType>(value);
    } else if (MATCH("config", "DictColumnModel")) {
        pconfig->dictColumnModelType = ConvertStringToEnum<DictColumnModelType>(value);
    } else if (MATCH("config", "SpecifyColumnModel")) {
        vector<string> modelStrs = split(value, '|');
        for(auto it = modelStrs.begin(); it != modelStrs.end(); it++){
            pconfig->specifyConfigs.push_back(SpecifyConfig::parse(*it));
        }
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

shared_ptr<CompressorConfigSingleton> CompressorConfigSingleton::getInstance(){
    if(mInstance == NULL){
        // init instance. read config file, setup map
        mInstance.reset(new CompressorConfigSingleton());

        if (ini_parse(filename.c_str(), handler, &CompressorConfigSingleton::config) < 0) {
            printf("Can't load '%s'\n", filename.c_str());
            exit(EXIT_FAILURE);
        }
    }
    return mInstance;
}

ModelType CompressorConfigSingleton::getModelType(){
    return config.modelType;
}

DictEncoderType CompressorConfigSingleton::getDictEncoderType(){
    return config.dictEncoderType;
}

FreshEncoderType CompressorConfigSingleton::getFreshEncoderType(){
    return config.freshEncoderType;
}

NumberEncoderType CompressorConfigSingleton::getNumberEncoderType(){
    return config.numberEncoderType;
}

DictColumnModelType CompressorConfigSingleton::getDictColumnModelType(){
    return config.dictColumnModelType;
}

SpecifyConfig* CompressorConfigSingleton::getSpecifyConfig(int column){
    for(unsigned int i = 0; i < config.specifyConfigs.size(); i++){
        if(column == config.specifyConfigs[i].getColumn())
            return &config.specifyConfigs[i];
    }
    return NULL;
}
