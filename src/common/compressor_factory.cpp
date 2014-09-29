/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//compressor_factory.cpp

#include "compressor_factory.h"

#include "word_dict_model.h"
#include "phrase_dict_model.h"
#include "column_wised_model.h"
#include "timestamp_model.h"
#include "ip_model.h"
#include "number_model.h"
#include "fix_precision_number_model.h"

#include "huffman_encoder.h"
#include "canonical_huffman_encoder.h"

#include "plain_encoder.h"
#include "auxiliary_encoder.h"

#include "unary_encoder.h"
#include "gamma_encoder.h"
#include "delta_encoder.h"

shared_ptr<Model> CompressorFactory::createModel(const ModelType& modelType){
    switch (modelType){
        case ModelType::WordDict:
            return shared_ptr<Model>(new WordDictModel());
        case ModelType::PhraseDict:
            return shared_ptr<Model>(new PhraseDictModel());
        case ModelType::ColumnWised:
            return shared_ptr<Model>(new ColumnWisedModel());
        default:
            return shared_ptr<Model>(new WordDictModel());
    }
}

shared_ptr<Model> CompressorFactory::createModel(const string& modelName){
    ModelType modelType = ConvertStringToEnum<ModelType>(modelName.c_str());
    return createModel(modelType);
}

shared_ptr<Model> CompressorFactory::createColumnModel(const ColumnModelType& columnModelType){
    switch(columnModelType){
        case ColumnModelType::WordDict:
            return shared_ptr<Model>(new WordDictModel());
        case ColumnModelType::PhraseDict:
            return shared_ptr<Model>(new PhraseDictModel());
        case ColumnModelType::Timestamp:
            return shared_ptr<Model>(new TimestampModel());
        case ColumnModelType::IP:
            return shared_ptr<Model>(new IPModel());
        case ColumnModelType::Number:
            return shared_ptr<Model>(new NumberModel<unsigned int>());
        case ColumnModelType::LongNumber:
            return shared_ptr<Model>(new NumberModel<unsigned long long>());
        case ColumnModelType::FixPrecisionNumber:
            return shared_ptr<Model>(new FixPrecisionNumberModel());
        default:
            return shared_ptr<Model>(new WordDictModel());
    }
}

shared_ptr<Model> CompressorFactory::createColumnModel(const string& columnModelName){
    ColumnModelType columnModelType = ConvertStringToEnum<ColumnModelType>(columnModelName.c_str());
    return createColumnModel(columnModelType);
}

shared_ptr<DictionaryModel> CompressorFactory::createDictColumnModel(const DictColumnModelType& modelType){
    switch (modelType){
        case DictColumnModelType::WordDict:
            return shared_ptr<DictionaryModel>(new WordDictModel());
        case DictColumnModelType::PhraseDict:
            return shared_ptr<DictionaryModel>(new PhraseDictModel());
        default:
            return shared_ptr<DictionaryModel>(new WordDictModel());
    }
}

shared_ptr<DictionaryModel> CompressorFactory::createDictColumnModel(const string& dictModelName){
    DictColumnModelType modelType = ConvertStringToEnum<DictColumnModelType>(dictModelName.c_str());
    return createDictColumnModel(modelType);
}

shared_ptr<Encoder> CompressorFactory::createDictEncoder(const DictEncoderType& encoderType){
    switch(encoderType){
        case DictEncoderType::Huffman:
            return shared_ptr<Encoder>(new HuffmanEncoder());
        case DictEncoderType::Canonical:
            return shared_ptr<Encoder>(new CanonicalHuffmanEncoder());
        default:
            return shared_ptr<Encoder>(new HuffmanEncoder());
    }
}

shared_ptr<NumberEncoder> CompressorFactory::createNumberEncoder(const NumberEncoderType& encoderType){
    switch(encoderType){
        case NumberEncoderType::Unary:
            return shared_ptr<NumberEncoder>(new UnaryEncoder());
        case NumberEncoderType::Gamma:
            return shared_ptr<NumberEncoder>(new GammaEncoder());
        case NumberEncoderType::Delta:
            return shared_ptr<NumberEncoder>(new DeltaEncoder());
        default:
            return shared_ptr<NumberEncoder>(new DeltaEncoder());
    }
}

shared_ptr<FreshEncoder> CompressorFactory::createFreshEncoder(const FreshEncoderType& encoderType){
    switch(encoderType){
        case FreshEncoderType::Plain:
            return shared_ptr<FreshEncoder>(new PlainEncoder());
        case FreshEncoderType::Auxiliary:
            return shared_ptr<FreshEncoder>(new AuxiliaryEncoder());
        default:
            return shared_ptr<FreshEncoder>(new PlainEncoder());
    }
}

shared_ptr<Model> CompressorFactory::createModel(){
    ModelType modelType = CompressorConfigSingleton::getInstance()->getModelType();
    return createModel(modelType);
}

shared_ptr<Encoder> CompressorFactory::createDictEncoder(){
    DictEncoderType encoderType = CompressorConfigSingleton::getInstance()->getDictEncoderType();
    return createDictEncoder(encoderType);
}

shared_ptr<NumberEncoder> CompressorFactory::createNumberEncoder(){
    NumberEncoderType encoderType = CompressorConfigSingleton::getInstance()->getNumberEncoderType();
    return createNumberEncoder(encoderType);
}

shared_ptr<FreshEncoder> CompressorFactory::createFreshEncoder(){
    FreshEncoderType encoderType = CompressorConfigSingleton::getInstance()->getFreshEncoderType();
    return createFreshEncoder(encoderType);
}

shared_ptr<Model> CompressorFactory::createColumnModel(int column){
    DictColumnModelType defaultColumnModelType = CompressorConfigSingleton::getInstance()->getDictColumnModelType();
    shared_ptr<DictionaryModel> defaultColumnModelPtr = createDictColumnModel(defaultColumnModelType);
    SpecifyConfig* specifyConfigPtr = CompressorConfigSingleton::getInstance()->getSpecifyConfig(column);
    if(specifyConfigPtr != NULL){
        return createSpecifyColumnModel(*specifyConfigPtr, defaultColumnModelPtr);
    }
    return defaultColumnModelPtr;
}

shared_ptr<Model> CompressorFactory::createSpecifyColumnModel(const SpecifyConfig& specifyConfig, const shared_ptr<DictionaryModel>& defaultColumnModelPtr){
    switch(specifyConfig.getModelType()){
        case SpecifyColumnModelType::Timestamp:
            return shared_ptr<Model>(new TimestampModel(defaultColumnModelPtr, specifyConfig.getArg()));
        case SpecifyColumnModelType::IP:
            return shared_ptr<Model>(new IPModel(defaultColumnModelPtr));
        case SpecifyColumnModelType::Number:
            return shared_ptr<Model>(new NumberModel<unsigned int>(defaultColumnModelPtr));
        case SpecifyColumnModelType::LongNumber:
            return shared_ptr<Model>(new NumberModel<unsigned long long>(defaultColumnModelPtr));
        case SpecifyColumnModelType::FixPrecisionNumber:
            {
            unsigned int precision = atoi(specifyConfig.getArg().c_str());
            return shared_ptr<Model>(new FixPrecisionNumberModel(defaultColumnModelPtr, precision));
            }
        default:
            return shared_ptr<Model>();
    }
}
