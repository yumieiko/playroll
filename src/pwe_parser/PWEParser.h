//
// Created by sol on 6/24/2025.
//

#ifndef PWEPARSER_H
#define PWEPARSER_H
#include <string>
#include <fstream>
#include "../base64/base64.h"


class PWEParser {
public:
    void pack(std::string configfile, std::string outputfile);
    void unpack(std::string pwefile, std::string outputfile);
};



#endif //PWEPARSER_H
