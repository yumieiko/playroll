#ifndef PWEPARSER_H
#define PWEPARSER_H
#include <string>
#include <fstream>
#include <filesystem>
#include "../base64/base64.h"
#include <zip.h>
#include <vector>
#include <yaml-cpp/yaml.h>

class PWEParser {
public:
    void pack(std::string folder, std::string outputfile);
    void unpack(std::string pwefile, std::string outputfile);
};

#endif //PWEPARSER_H