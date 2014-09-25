/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//word_list_encoder.h

#ifndef _WORD_LIST_ENCODER_H_
#define _WORD_LIST_ENCODER_H_

#include <memory>

#include "number_encoder.h"

using std::shared_ptr;

class WordListEncoder : public Encoder{
public:
    WordListEncoder(const string& wordsFilenameValue);
    void buildCoder(const unordered_map<string, int>& wordFreqDict);
    bool isFresh(const string& word) const;
    /* The caller must gurantee word exists! */
    BitArray encode(const string& word) const;
    /* The caller must gurantee code can be decoded by word list.*/
    int decode(BinaryCode& code, string& word) const;
    void parse(string& dumpStr);
    string dump() const;
    void append(const string& word);
    bool updateThenCheckExist(const string& word);
private:
    // for decompress. give an index, find corresponding word
    vector<string> wordList;
    // for compress. give an word, find its index.
    // decrease search word time from O(N) to O(1)
    unordered_map<string, BitArray> word2CodeDict;
    // during compress step, when meeting a fresh word, decide whether to
    // store it in auxiliary word list. Store the word as a part of model.
    string wordsFilename;
    // encode number.
    shared_ptr<NumberEncoder> numberEncoderPtr;
    // record frequency of fresh word, move it to word list if frequency 
    // is larger than a threshold.
    unordered_map<string, int> freshWord2FreqDict;

    string dumpWordList2File(const string& wordsDumpStr) const;
    void append2File(const string& word);
    void append2WordList(const string& word);
    /* Parse word list*/
    void parseWordList(const string& wordListStr);
};

#endif
