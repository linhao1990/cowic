/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//timestamp_model.cpp

#include <sstream>

#include "timestamp.h"
#include "timestamp_model.h"
#include "helper.h"
#include "compressor_factory.h"

int TimestampModel::totalNum = 0;
int TimestampModel::illegalNum = 0;
const double TimestampModel::thresholdRatio = 0.1;

TimestampModel::TimestampModel() : Model(), dictModelPtr(), format(){
}

TimestampModel::TimestampModel(const shared_ptr<DictionaryModel>& dictModelPtrVal, const string& formatVal) 
    : Model(), dictModelPtr(dictModelPtrVal), format(formatVal){
    }

void TimestampModel::updateColumn(const string& columnStr, int column){
    // Try to parse timestamp
    Timestamp ts = Timestamp::parse(columnStr, format);
    // Dispatch to dictModelPtr when fail
    if(ts == Timestamp::illegal){
        dictModelPtr->updateColumn(columnStr, column);
        ++illegalNum;
    } else{
        // update timezone, and after str
        if(!ts.getTimezone().empty())
            dictModelPtr->updateWord(ts.getTimezone());
        if(!ts.getAfter().empty())
            dictModelPtr->updateColumn(ts.getAfter(), column);
    }
    ++totalNum;
}

void TimestampModel::showWarningMessage() const{
    bool tooManyError = (illegalNum + 0.0) / totalNum > thresholdRatio;
    if(tooManyError){
        cout << "\nWarning! Cannot parse timestamp column correctly!" << endl
             <<"\tIn training set. Total Entry Num:" << totalNum << " Illegal Entry Num:" << illegalNum << endl
             << "\tPlease check you configuration and make sure timestamp format is correct!" << endl;
    }
}

string TimestampModel::dump() const{
    showWarningMessage();
    std::stringstream sstream;
    string modelDumpStr = dictModelPtr->dump();
    sstream << getModelName() << "\n" 
            << format.size() << "\n"
            << format
            << modelDumpStr.size() << "\n"
            << modelDumpStr;
    return sstream.str();
}

void TimestampModel::parse(string& dumpStr){
    int formatLen = parseFirstLine2Int(dumpStr);
    format = cutNByte(dumpStr, formatLen);
    int modelByteLen = parseFirstLine2Int(dumpStr);
    string modelDumpStr = cutNByte(dumpStr, modelByteLen);
    string dictModelName = cutFirstLine(modelDumpStr);
    dictModelPtr = CompressorFactory::createDictColumnModel(dictModelName);
    dictModelPtr->parse(modelDumpStr);
} 

BitArray TimestampModel::compressMayEmptyWord(const string& word){
    BitArray markCode;
    if(word.size() > 0){
        markCode.push_back(true);
        return markCode + dictModelPtr->compressWord(word);
    } else{
        markCode.push_back(false);
        return markCode;
    }
}

BitArray TimestampModel::compressMayEmptyColumn(const string& word, int column){
    BitArray markCode;
    if(word.size() > 0){
        markCode.push_back(true);
        return markCode + dictModelPtr->compressColumn(word, column);
    } else{
        markCode.push_back(false);
        return markCode;
    }
}

int TimestampModel::decompressMayEmptyWord(BinaryCode& code, string& word) const{
    if(code.finish())
        return -1;
    bool mark = code.getFirstBitMoveToNext();
    if(mark){
        return dictModelPtr->decompressWord(code, word);
    } else{
        word = "";
        return 0;
    }
}

int TimestampModel::decompressMayEmptyColumn(BinaryCode& code, string& word, int column) const{
    if(code.finish())
        return -1;
    bool mark = code.getFirstBitMoveToNext();
    if(mark){
        return dictModelPtr->decompressColumn(code, word, column);
    } else{
        word = "";
        return 0;
    }
}

BitArray TimestampModel::compressColumn(const string& columnStr, int column){
    // Try to parse timestamp
    Timestamp ts = Timestamp::parse(columnStr, format);
    // Dispatch to dictModelPtr when fail
    if(ts == Timestamp::illegal){
        BitArray legalTimestampMark;
        legalTimestampMark.push_back(false);
        return legalTimestampMark + dictModelPtr->compressColumn(columnStr, column);
    } else{
        BitArray legalTimestampMark;
        legalTimestampMark.push_back(true);
        BitArray numCode((unsigned int)ts.getNumericalTime());
        BitArray tzCode = compressMayEmptyWord(ts.getTimezone());
        BitArray afterCode = compressMayEmptyColumn(ts.getAfter(), column);
        return  legalTimestampMark + numCode + tzCode + afterCode;
    }
}

int TimestampModel::decompressColumn(BinaryCode& code, string& columnStr, int column) const{
    bool legalTimestampMark = code.getFirstBitMoveToNext();
    if(legalTimestampMark){
        unsigned int num;
        int stat = decodeInt(code, num);
        if(stat < 0)
            return stat;
        string timezone;
        decompressMayEmptyWord(code, timezone);
        string after;
        int ret = decompressMayEmptyColumn(code, after, column);
        columnStr = Timestamp(num, timezone, after).to_plain_str(format);
        return ret;
    } else{
        //decompress till meet column end
        return dictModelPtr->decompressColumn(code, columnStr, column);
    }
}

string TimestampModel::getModelName() const{
    return "Timestamp";
}
