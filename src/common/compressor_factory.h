/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//compressor_factory.h

#ifndef _COMPRESSOR_FACTORY_H_
#define _COMPRESSOR_FACTORY_H_

#include <memory>

#include "model.h"
#include "dictionary_model.h"
#include "encoder.h"
#include "fresh_encoder.h"
#include "number_encoder.h"
#include "compressor_config.h"

using std::shared_ptr;

class CompressorFactory{
public:
    static shared_ptr<Model> createModel(const ModelType& modelType);
    static shared_ptr<Model> createModel(const string& modelName);
    static shared_ptr<Model> createColumnModel(const ColumnModelType& columnModelType);
    static shared_ptr<Model> createColumnModel(const string& columnModelName);
    static shared_ptr<DictionaryModel> createDictColumnModel(const DictColumnModelType& modelType);
    static shared_ptr<DictionaryModel> createDictColumnModel(const string& dictModelName);
    static shared_ptr<Encoder> createDictEncoder(const DictEncoderType& encoderType);
    static shared_ptr<NumberEncoder> createNumberEncoder(const NumberEncoderType& encoderType);
    static shared_ptr<FreshEncoder> createFreshEncoder(const FreshEncoderType& encoderType);
    // The following read config to decide the object to create
    static shared_ptr<Model> createModel();
    static shared_ptr<Encoder> createDictEncoder();
    static shared_ptr<NumberEncoder> createNumberEncoder();
    static shared_ptr<FreshEncoder> createFreshEncoder();
    static shared_ptr<Model> createColumnModel(int column);
    static shared_ptr<Model> createSpecifyColumnModel(const SpecifyConfig& specifyConfig, const shared_ptr<DictionaryModel>& defaultColumnModelPtr);
private:
};

#endif
