/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//word_list_encoder.cpp

#include <fstream>
#include <sstream>

#include "word_list_encoder.h"
#include "compressor_factory.h"
#include "helper.h"

const int threshold = 2;

WordListEncoder::WordListEncoder(const string& wordsFilenameValue) : wordsFilename(wordsFilenameValue){
    numberEncoderPtr = CompressorFactory::createNumberEncoder();
    parse(wordsFilename);
}

void WordListEncoder::buildCoder(const unordered_map<string, int>& wordFreqDict){
    // do nothing
    // the encoder is builded word by word during compress stage
}

bool WordListEncoder::isFresh(const string& word) const{
    return word2CodeDict.find(word) == word2CodeDict.end();
}

BitArray WordListEncoder::encode(const string& word) const{
    unordered_map<string, BitArray>::const_iterator it = word2CodeDict.find(word);
    if(it == word2CodeDict.end()){
        //TODO shall not happen
        return BitArray();
    }
    return it->second;
}

int WordListEncoder::decode(BinaryCode& code, string& word) const{
    unsigned int index;
    int ret = numberEncoderPtr->decode(code, index);
    if(ret < 0)
        return ret;
    if(index > wordList.size())
        return -1;
    word = wordList[index - 1];
    return 0;
}

void WordListEncoder::parse(string& dumpStr){
    wordsFilename = dumpStr;
    std::ifstream infile(wordsFilename);

    if(!infile.is_open()){
        //file not exist. empty word list
        return;
    }

    std::stringstream buffer;
    buffer << infile.rdbuf();
    string wordListStr = buffer.str();

    parseWordList(wordListStr);
    infile.close();
}

void WordListEncoder::parseWordList(const string& wordListStr){
    wordList.clear();
    word2CodeDict.clear();
    vector<string> lines = split(wordListStr, '\n');
    for(auto it = lines.begin(); it != lines.end(); it++)
        append2WordList(*it);
}

string WordListEncoder::dump() const{
    string ret = "";
    for(auto it = wordList.begin(); it != wordList.end(); it++)
        ret += (*it) + "\n";
    return dumpWordList2File(ret);
}

string WordListEncoder::dumpWordList2File(const string& wordsDumpStr) const{
    std::ofstream outfile (wordsFilename);

    if(!outfile.is_open()){
        std::cerr << "\nERROR: " << "File " + wordsFilename + " could not be opened." << '\n';
        exit(EXIT_FAILURE);
    }

    outfile << wordsDumpStr;
    outfile.close();

    return wordsFilename;
}

void WordListEncoder::append2File(const string& word){
    std::ofstream outfile (wordsFilename, std::ofstream::app);

    if(!outfile.is_open()){
        std::cerr << "\nERROR: " << "File " + wordsFilename + " could not be opened." << '\n';
        exit(EXIT_FAILURE);
    }

    outfile << word << std::endl;
    outfile.close();
}

void WordListEncoder::append2WordList(const string& word){
    int index = wordList.size() + 1;
    wordList.push_back(word);
    BitArray code = numberEncoderPtr->encode(index);
    string wordCopy = word;
    word2CodeDict.insert(std::make_pair<string, BitArray>(std::move(wordCopy), std::move(code)));
}

void WordListEncoder::append(const string& word){
    //if(!isFresh(word))
    //    return;
    append2File(word);

    append2WordList(word);
}

bool WordListEncoder::updateThenCheckExist(const string& word){
    if(!isFresh(word))
        return true;
    // The word is a fresh word, update its frequency and check whether it reach threshold
    auto it = freshWord2FreqDict.find(word);
    if(it != freshWord2FreqDict.end()){
        it->second += 1;
        if(it->second >= threshold){
            append(it->first);
            freshWord2FreqDict.erase(it);
            return true;
        }
    }
    else{
        // insert new entry
        std::pair<string, int> wordFreqPair(word, 1);
        freshWord2FreqDict.insert(wordFreqPair);
    }
    return false;
}
