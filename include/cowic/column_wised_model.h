//column_wised_model.h

#ifndef _COLUMN_WISED_MODEL_H_
#define _COLUMN_WISED_MODEL_H_

#include <memory>

#include "model.h"

typedef std::shared_ptr<Model> ModelPtr;

class ColumnWisedModel : public Model{
public:
    ColumnWisedModel();
    void updateColumn(const string& columnStr, int column);
    /* Dump each model in order.
     * Format:
     * <MODEL_NAME>
     * <#MODEL_NUM>
     * <#MODEL1_BYTE_LEN>
     * <MODEL1_DUMP_STR>
     * ...
     * <#MODELN_BYTE_LEN>
     * <MODELN_DUMP_STR>
     * */
    string dump() const;
    void parse(string& dumpStr);
    BitArray compressColumn(const string& columnStr, int column); 
    int decompressColumn(BinaryCode& code, string& columnStr, int column) const;
    string getModelName() const;
private:
    ModelPtr& getModel(int column);
    const ModelPtr& getModel(int column) const;
    ModelPtr& getModelCreateIfNotExist(unsigned int column);
    vector<ModelPtr> modelPtrs;
};

#endif
