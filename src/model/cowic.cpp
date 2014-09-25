/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//compressor.cpp

#include <iostream>
#include <fstream>
#include <sstream>

#include "cowic.h"
#include "spliter.h"
#include "compressor_factory.h"

const std::string MODEL_MAGIC_NUM ="7771234777";

Cowic::Cowic(){
}

vector<string> read_lines(const string& filename){
    ifstream infile(filename);
    if(!infile.is_open()){
        std::cerr << "File " + filename + " could not be opened." << std::endl;
        exit (EXIT_FAILURE);
    }

    vector<string> ret;
    string s;
    while(getline(infile, s)){
        ret.push_back(s);
    }
    return ret;
}

void Cowic::train(const string& seedFile, const string& modelFile){
    vector<string> lines = read_lines(seedFile);
    trainModel(lines);
    saveModel(modelFile);
}

void Cowic::trainModel(const vector<string>& lines){
    modelPtr = CompressorFactory::createModel();
    for(vector<string>::const_iterator lineIt = lines.begin(); lineIt != lines.end(); lineIt++){
        string line = (*lineIt) + EOL_STR;
        vector<string> columns;
        Spliter::splitLine2Columns(line, columns);
        for(unsigned int columnIndex = 0; columnIndex < columns.size(); columnIndex++){
            modelPtr->updateColumn(columns[columnIndex], columnIndex + 1);
        }
    }
}

void Cowic::saveModel(const string& modelFile) const{
    AuxiliaryFilename::initFilename(modelFile);
    string dumpStr = modelPtr->dump();

    std::ofstream outfile (modelFile, std::ofstream::binary);
    if(!outfile.is_open()){
        std::cerr << "\nERROR: " << "File " + modelFile + " could not be opened." << '\n';
        exit(EXIT_FAILURE);
    }

    outfile << MODEL_MAGIC_NUM << endl;
    outfile << dumpStr;
    outfile.close();
}

void checkMagicNum(string& content, const string& modelFile){
    size_t pos = content.find('\n');
    string magicNum = content.substr(0, pos);
    content = content.substr(pos + 1);

    if(magicNum != MODEL_MAGIC_NUM){
        std::cerr << "\nERROR: " << modelFile + "is not a legal model file." << '\n';
        exit(EXIT_FAILURE);
    }
}

void loadModelToPtr(const string& modelFile, shared_ptr<Model>& modelPtr){
    std::ifstream infile (modelFile, std::ifstream::binary);
    if(!infile.is_open()){
        std::cerr << "\nERROR: " << "File " + modelFile + " could not be opened." << '\n';
        exit(EXIT_FAILURE);
    }

    std::stringstream buffer;
    buffer << infile.rdbuf();
    string content = buffer.str();

    checkMagicNum(content, modelFile);

    string modelName = cutFirstLine(content);
    modelPtr = CompressorFactory::createModel(modelName);
    modelPtr->parse(content);
}

void Cowic::loadModel(const string& modelFile){
    loadModelToPtr(modelFile, modelPtr);
}

string Cowic::compress(const string& entry){
    BitArray code = doCompress(entry);
    return code.to_binary_str();
}

BitArray Cowic::doCompress(const string& lineNoEnd){
    string line = lineNoEnd + EOL_STR;
    BitArray lineCode;
    vector<string> columns;
    Spliter::splitLine2Columns(line, columns);
    for(unsigned int columnIndex = 0; columnIndex < columns.size(); columnIndex++){
        string columnStr = columns[columnIndex];
        int modelColumnIndex = columnIndex + 1; // in model, column index starts from 1
        BitArray columnCode = modelPtr->compressColumn(columnStr, modelColumnIndex);
        //cout<<columnCode.size()<<"|";
        lineCode += columnCode;
    }
    //cout<<endl;
    return lineCode;
}

bool replaceEOL(string& str, bool isLastModel = true){
    if(str.find(EOL_STR) != string::npos){
        if(isLastModel){
            replaceAll(str, EOL_STR, "\n");
        } else{
            replaceAll(str, EOL_STR, " ");
        }
        return true;
    }
    return false;
}

string Cowic::decompress(const string& compressedEntry){
    BitArray code(compressedEntry);
    return doDecompress(code);
}

string Cowic::doDecompress(BitArray& code){
    BinaryCode lineCode(std::move(code));
    string ret = "";

    int column = 1;
    while(!lineCode.finish()){
        string columnStr;
        int state = modelPtr->decompressColumn(lineCode, columnStr, column);
        if(state == -1){
            std::cerr << "\nERROR: " << "File corrupted. Cannot decode this entry." << '\n';
            return "";
        }
        bool isModelFinish = replaceEOL(columnStr);
        ret += columnStr;
        column++;
        if(isModelFinish)
            break;
    }

    return ret;
}
