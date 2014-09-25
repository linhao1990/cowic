/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//column_wised_model.cpp

#include <sstream>

#include "column_wised_model.h"
#include "compressor_factory.h"
#include "helper.h"
#include "spliter.h"

ColumnWisedModel::ColumnWisedModel() : Model(){
}

const ModelPtr& ColumnWisedModel::getModel(int column) const{
    return modelPtrs[column - 1];
}

ModelPtr& ColumnWisedModel::getModel(int column) {
    return const_cast<ModelPtr&>(
                (static_cast<const ColumnWisedModel&>(*this)).getModel(column)
            );
}

ModelPtr& ColumnWisedModel::getModelCreateIfNotExist(unsigned int column){
    while(modelPtrs.size() < column){
        ModelPtr ptr = CompressorFactory::createColumnModel(modelPtrs.size() + 1); 
        modelPtrs.push_back(ptr);
    }
    return getModel(column);
}

void ColumnWisedModel::updateColumn(const string& columnStr, int column){
    ModelPtr columnModelPtr = getModelCreateIfNotExist(column);
    columnModelPtr->updateColumn(columnStr, column);
}

string ColumnWisedModel::dump() const{
    std::stringstream sstream;
    sstream << getModelName() << "\n" 
            << modelPtrs.size() << "\n";
    for(unsigned int i = 0; i < modelPtrs.size(); i++){
        string modelDumpStr = modelPtrs[i]->dump();
        sstream << modelDumpStr.size() << "\n"
                << modelDumpStr;
    }
    return sstream.str();
}

void ColumnWisedModel::parse(string& dumpStr){
    int modelNum = parseFirstLine2Int(dumpStr);
    for(int column = 1; column <= modelNum; column++){
        int modelByteLen = parseFirstLine2Int(dumpStr);
        string modelDumpStr = cutNByte(dumpStr, modelByteLen);
        string columnModelName = cutFirstLine(modelDumpStr);
        ModelPtr ptr = CompressorFactory::createColumnModel(columnModelName);
        ptr->parse(modelDumpStr);
        modelPtrs.push_back(ptr);
    }
}

BitArray ColumnWisedModel::compressColumn(const string& columnStr, int column){
    if(column > modelPtrs.size()){
        // exception case: column is larger than appered in train stage
        // no model can handle it, just output the raw string
        // output the size so we can know the length of raw code.
        unsigned int strLen = columnStr.size();
        return BitArray(strLen) + BitArray(columnStr);
    }
    ModelPtr columnModelPtr = getModel(column);
    return columnModelPtr->compressColumn(columnStr, column);
}

int ColumnWisedModel::decompressColumn(BinaryCode& code, string& columnStr, int column) const{
    if(column > modelPtrs.size()){
        unsigned int length;
        int ret = decodeInt(code, length);
        if(ret < 0)
            return ret;
        return decodePlainStr(code, columnStr, length);
    }
    return getModel(column)->decompressColumn(code, columnStr, column);
}

string ColumnWisedModel::getModelName() const{
    return "ColumnWised";
}
