#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "cowic.h"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

enum class Operation{
    Compress,
    Decompress,
    Train,
    Help
};

struct globalArgs_t {
    Operation op = Operation::Help;
    string seed, model, input, output;
} globalArgs;

///
/// @brief Prints usage_then_exit information and a custom error message.
/// @param s    custom error message to be printed
/// @param su   Show Usage information
///
void usage_then_exit(const std::string &s = "", bool su = true){
    if (!s.empty())
        std::cerr << "\nERROR: " << s << '\n';

    if (su)
    {
        std::cerr << "\nUsage:\n";
        std::cerr << "\tcompressor_cmd_tool -t seed -m model\n";
        std::cerr << "\tcompressor_cmd_tool -c raw -m model -o compressed_output\n";
        std::cerr << "\tcompressor_cmd_tool -d compressed_file -m model -o raw\n";
        std::cerr << "\tcompressor_cmd_tool -h\n\n";
        std::cerr << "Description:\n";
        std::cerr << "\t-t\tTrain the model with seed\n";
        std::cerr << "\t-c\tCompress raw file.\n";
        std::cerr << "\t-d\tDecompress file.\n";
        std::cerr << "\t-m\tModel file or model list\n";
        std::cerr << "\t\tWhen decompress join result of two compressed entry, requires a list of model.\n";
        std::cerr << "\t\tThe list is seperated by ',' and no space. The order of model shall be same with join stage.\n";
        std::cerr << "\t-o\tOutput file of the program.\n";
        std::cerr << "\t-h\tShow this help message.\n\n";
        std::cerr << "Examples:\n";
        std::cerr << "\tcompressor_cmd_tool -t log_seed.txt -m log.mdl\n";
        std::cerr << "\tcompressor_cmd_tool -c log.txt -m log.mdl -o compressed_log\n";
        std::cerr << "\tcompressor_cmd_tool -d compressed_log -m log.mdl -o log_raw.txt\n";
    }

    std::cerr << std::endl;
    exit (EXIT_FAILURE);
}

void parse_args(int argc, char **argv){
    int c;

    while ((c = getopt (argc, argv, "t:k:m:c:o:d:h")) != -1)
        switch (c)
        {
            case 't':
                globalArgs.op = Operation::Train;
                globalArgs.seed = optarg;
                break;
            case 'm':
                globalArgs.model = optarg;
                break;
            case 'c':
                globalArgs.op = Operation::Compress;
                globalArgs.input = optarg;
                break;
            case 'o':
                globalArgs.output = optarg;
                break;
            case 'd':
                globalArgs.op = Operation::Decompress;
                globalArgs.input = optarg;
                break;
            case 'h':
                globalArgs.op = Operation::Help;
                break;
            case '?':
                if (optopt == 'k')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
            default:
                usage_then_exit();
        }
}

inline void verify_arg_no_empty(const string& arg, const string& argname = ""){
    if(arg.size() == 0){
        usage_then_exit("Arg " + argname + " shall not be empty!", true);
    }
}

void train(const string& seedFilename, const string& modelFilename){
    verify_arg_no_empty(modelFilename, "model");

    Cowic trainer;
    trainer.train(seedFilename, modelFilename); 
}

void compress(const string& inputFilename, const string& modelFilename, const string& outputFilename){
    verify_arg_no_empty(inputFilename, "input");
    verify_arg_no_empty(modelFilename, "model");
    verify_arg_no_empty(outputFilename, "output");

    string dataFilename = outputFilename + ".dat";
    string indexFilename = outputFilename + ".idx";

    ifstream infile(inputFilename, std::ifstream::binary);
    ofstream datafile(dataFilename, std::ofstream::binary);
    ofstream indexfile(indexFilename, std::ofstream::binary);

    if(!infile.is_open()){
        usage_then_exit("File " + inputFilename + " could not be opened.", false);
    }

    if(!datafile.is_open()){
        usage_then_exit("File " + dataFilename + " could not be opened.", false);
    }

    if(!indexfile.is_open()){
        usage_then_exit("File " + indexFilename + " could not be opened.", false);
    }

    Cowic compressor;
    compressor.loadModel(modelFilename);

    string line;
    while(getline(infile, line)){
        string binaryStr = compressor.compress(line);

        datafile << binaryStr;

        string idxStr = formatIndex(binaryStr.size());
        indexfile<< idxStr;
    }

    infile.close();
    datafile.close();
    indexfile.close();
}

void openFiles(const string& inputFilename, const string& modelFilename, const string& outputFilename, ifstream& datafile, ifstream& indexfile, ofstream& outfile){
    verify_arg_no_empty(inputFilename, "input");
    verify_arg_no_empty(modelFilename, "model");
    verify_arg_no_empty(outputFilename, "output");

    string dataFilename = inputFilename + ".dat";
    string indexFilename = inputFilename + ".idx";

    datafile.open(dataFilename, std::ifstream::binary);
    indexfile.open(indexFilename, std::ifstream::binary);
    outfile.open(outputFilename, std::ofstream::binary);

    if(!datafile.is_open()){
        usage_then_exit("File " + dataFilename + " could not be opened.", false);
    }

    if(!indexfile.is_open()){
        usage_then_exit("File " + indexFilename + " could not be opened.", false);
    }

    if(!outfile.is_open()){
        usage_then_exit("File " + outputFilename + " could not be opened.", false);
    }
}

void decompress(const string& inputFilename, const string& modelFilename, const string& outputFilename){
    ifstream datafile, indexfile;
    ofstream outfile;
    openFiles(inputFilename, modelFilename, outputFilename, datafile, indexfile, outfile);

    Cowic compressor;
    compressor.loadModel(modelFilename);

    string line;
    int lineNum = 0, offset = 0, byteNum;
    while(getline(indexfile, line)){
        byteNum = atoi(line.c_str());
        string binaryStr = cutParseDatafile(offset, byteNum, datafile);
        string plainStr = compressor.decompress(binaryStr);
        outfile<<plainStr;
        
        if(plainStr.size() == 0)
            cout<<"LineNum:"<<lineNum<<endl;
        lineNum ++;
    }

    datafile.close();
    indexfile.close();
    outfile.close();
}

int main(int argc, char* argv[])
{
    parse_args(argc, argv);
    switch(globalArgs.op){
        case Operation::Train:
            train(globalArgs.seed, globalArgs.model);
            break;
        case Operation::Compress:
            compress(globalArgs.input, globalArgs.model, globalArgs.output);
            break;
        case Operation::Decompress:
            decompress(globalArgs.input, globalArgs.model, globalArgs.output);
            break;
        case Operation::Help:
            usage_then_exit();
            break;
        default:
            usage_then_exit();
    }
    return 0;
}
