/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//timestamp_model.h

#ifndef _TIMESTAMP_MODEL_H_
#define _TIMESTAMP_MODEL_H_

#include <memory>

#include "model.h"
#include "dictionary_model.h"

using std::shared_ptr;

class TimestampModel : public Model{
public:
    TimestampModel();
    TimestampModel(const shared_ptr<DictionaryModel>& dictModelPtrVal, const string& formatVal);
    void updateColumn(const string& columnStr, int column);
    /* <MODEL_NAME>
     * <#MODEL_BYTE_LEN>
     * <MODEL_DUMP_STR>
     * */
    string dump() const;
    void parse(string& dumpStr);
    BitArray compressMayEmptyWord(const string& word);
    BitArray compressMayEmptyColumn(const string& word, int column);
    int decompressMayEmptyWord(BinaryCode& code, string& word) const;
    int decompressMayEmptyColumn(BinaryCode& code, string& word, int column) const;
    BitArray compressColumn(const string& columnStr, int column);
    int decompressColumn(BinaryCode& code, string& columnStr, int column) const;
    string getModelName() const;
private:
    /* The model used to process string when the timestamp cannot be parsed successfully.
     */
    shared_ptr<DictionaryModel> dictModelPtr;
    string format;
    // used to decide whether it's proper to using timestamp model
    static int totalNum;
    static int illegalNum;
    static const double thresholdRatio;
    void showWarningMessage() const;
};

#endif
