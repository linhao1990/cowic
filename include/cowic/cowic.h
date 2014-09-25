//cowic.h
#ifndef _COWIC_H
#define _COWIC_H

#include <string>
#include <vector>
#include <memory>

#include "model.h"
#include "helper.h"

using std::string;
using std::vector;
using std::shared_ptr;

class Cowic{
public:
    Cowic();
    void train(const string& seedFile, const string& modelFile);
    void loadModel(const string& modelFile);
    string compress(const string& entry);
    string decompress(const string& compressedEntry);
private:
    void initModelPtr();
    void trainModel(const vector<string>& lines);
    void saveModel(const string& filename) const;
    BitArray doCompress(const string& line);
    string doDecompress(BitArray& code);

    shared_ptr<Model> modelPtr;
};

#endif
